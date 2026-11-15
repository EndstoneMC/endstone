#include <array>
#include <chrono>
#include <set>

#include "bedrock/network/packet.h"
#include "bedrock/shared_constants.h"
#include "endstone/core/plugin/java_gameplay_internal.h"
#include "endstone/event/server/packet_receive_event.h"
#include "endstone/event/server/packet_send_event.h"

namespace endstone::core::java::details {
namespace {
// Native public APIs report release/protocol, not the full four-part BDS build.
// This adapter never interprets payload bytes or claims a decoded packet layout.
bool compatible(Context &c)
{
    return c.server->getMinecraftVersion() == "26.60" && c.server->getProtocolVersion() == 2225 &&
           SharedConstants::MinorVersion == 26 && SharedConstants::PatchVersion == 60 &&
           SharedConstants::NetworkProtocolVersion == 2225;
}
struct Connection {
    UUID player;
    std::int64_t actor;
    std::uint64_t runtime;
    std::string playerText;
    bool active = true;
};
struct NetworkListener {
    Global callback;
    jmethodID method;
    std::vector<EventHandler *> handlers;
    std::set<int> ids;
    std::atomic<bool> active{true};
};
}  // namespace
struct NetworkState {
    std::unordered_map<jlong, std::shared_ptr<NetworkListener>> listeners;
    std::unordered_map<std::string, Connection> connections;
    std::unordered_map<UUID, std::string> connectionsByPlayer;
    std::set<UUID> disconnected;
    EventHandler *quit = nullptr, *join = nullptr;
    std::array<std::uint64_t, 7> metrics{};
    std::atomic<std::uint64_t> wrongThreadDrops{0};
    bool diagnostics = false;
};
namespace {
std::shared_ptr<NetworkState> state(const std::shared_ptr<Context> &c)
{
    if (c->network) {
        return c->network;
    }
    auto s = std::make_shared<NetworkState>();
    c->network = s;
    std::weak_ptr<NetworkState> weak = s;
    s->quit = manager(*c).registerManagedEvent(
        PlayerQuitEvent::NAME,
        [weak](Event &event) {
            if (auto s = weak.lock()) {
                auto uuid = static_cast<PlayerQuitEvent &>(event).getPlayer().getUniqueId();
                s->disconnected.insert(uuid);
                s->connectionsByPlayer.erase(uuid);
                for (auto &[key, entry] : s->connections) {
                    if (entry.player == uuid) {
                        entry.active = false;
                    }
                }
            }
        },
        EventPriority::Lowest, *c->plugin, false);
    s->join = manager(*c).registerManagedEvent(
        PlayerJoinEvent::NAME,
        [weak](Event &event) {
            if (auto s = weak.lock()) {
                s->disconnected.erase(static_cast<PlayerJoinEvent &>(event).getPlayer().getUniqueId());
            }
        },
        EventPriority::Lowest, *c->plugin, false);
    return s;
}
std::string connectionId(Context &c, NetworkState &s, Player &p)
{
    auto uuid = p.getUniqueId();
    if (s.disconnected.contains(uuid) || c.server->getPlayer(uuid) != &p) {
        return {};
    }
    auto actor = p.getId();
    auto runtime = p.getRuntimeId();
    if (auto cached = s.connectionsByPlayer.find(uuid); cached != s.connectionsByPlayer.end()) {
        auto entry = s.connections.find(cached->second);
        if (entry != s.connections.end() && entry->second.active && entry->second.player == uuid &&
            entry->second.actor == actor && entry->second.runtime == runtime) {
            return entry->first;
        }
        s.connectionsByPlayer.erase(cached);
    }
    // Retire old session entries when admitting a connection, outside the common packet path.
    std::erase_if(s.connections, [&](const auto &entry) {
        bool retired = !entry.second.active || (entry.second.player == uuid &&
                                                (entry.second.actor != actor || entry.second.runtime != runtime));
        if (retired) {
            auto indexed = s.connectionsByPlayer.find(entry.second.player);
            if (indexed != s.connectionsByPlayer.end() && indexed->second == entry.first) {
                s.connectionsByPlayer.erase(indexed);
            }
        }
        return retired;
    });
    if (s.connections.size() >= 4096) {
        throw std::runtime_error("Player connection view limit exceeded");
    }
    auto key = std::to_string(id());
    auto entry = s.connections.emplace(key, Connection{uuid, actor, runtime, uuid.str()}).first;
    try {
        s.connectionsByPlayer.insert_or_assign(uuid, key);
    }
    catch (...) {
        s.connections.erase(entry);
        throw;
    }
    return key;
}
bool active(Context &c, NetworkState &s, const std::string &key)
{
    auto it = s.connections.find(key);
    if (it == s.connections.end() || !it->second.active) {
        return false;
    }
    auto &entry = it->second;
    auto *p = c.server->getPlayer(entry.player);
    if (!p || s.disconnected.contains(entry.player) || p->getId() != entry.actor ||
        p->getRuntimeId() != entry.runtime) {
        entry.active = false;
        auto indexed = s.connectionsByPlayer.find(entry.player);
        if (indexed != s.connectionsByPlayer.end() && indexed->second == key) {
            s.connectionsByPlayer.erase(indexed);
        }
    }
    return entry.active;
}
template <typename E>
void observe(const std::weak_ptr<Context> &weak, const std::weak_ptr<NetworkState> &weakState,
             const std::weak_ptr<NetworkListener> &weakListener, E &event, int direction) noexcept
{
    auto c = weak.lock();
    auto s = weakState.lock();
    auto listener = weakListener.lock();
    if (!c || !s || !listener || !c->active || !listener->active) {
        return;
    }
    // Worker rejection precedes all Player and mutable server-state access.
    if (c->main.load() != std::this_thread::get_id()) {
        ++s->wrongThreadDrops;
        return;
    }
    try {
        int packetId = event.getPacketId(), sub = event.getSubClientId();
        if (packetId < 0 || packetId > 1023 || sub < 0 || sub > 3 ||
            (!listener->ids.empty() && !listener->ids.contains(packetId))) {
            return;
        }
        if (!c->server->isPrimaryThread() || !c->plugin->isEnabled() || !compatible(*c)) {
            return;
        }
        ++s->metrics[0];
        auto callback = listener->callback;  // Pin across reentrant close/disable.
        if (!callback) {
            return;
        }
        std::array<char, 37> uuid{};
        std::string connection;
        if (auto *p = event.getPlayer()) {
            connection = connectionId(*c, *s, *p);
            if (!connection.empty()) {
                // Own the ASCII copy before JNI attachment or Java callback reentrancy.
                const auto &cached = s->connections.at(connection).playerText;
                std::copy_n(cached.c_str(), uuid.size(), uuid.begin());
            }
        }
        bool timed = s->diagnostics;
        auto start = timed ? std::chrono::steady_clock::now() : std::chrono::steady_clock::time_point{};
        try {
            Env env;
            auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(
                              std::chrono::system_clock::now().time_since_epoch())
                              .count();
            // Both identifiers are generated ASCII values, not arbitrary packet text.
            auto playerText = env.e->NewStringUTF(uuid.data());
            checked(env.e);
            auto connectionText = env.e->NewStringUTF(connection.c_str());
            checked(env.e);
            ++s->metrics[1];
            env.e->CallVoidMethod(callback->value, listener->method, static_cast<jint>(packetId),
                                  static_cast<jint>(direction), static_cast<jlong>(millis),
                                  static_cast<jlong>(event.getPayload().size()), static_cast<jint>(sub),
                                  static_cast<jboolean>(event.isCancelled()), playerText, connectionText);
            if (env.e->ExceptionCheck()) {
                if (s->metrics[2] == 0 || (s->metrics[2] + 1) % 1000 == 0) {
                    env.e->ExceptionDescribe();
                }
                env.e->ExceptionClear();
                throw std::runtime_error("Java packet callback exception (stack traces sampled)");
            }
        }
        catch (const std::exception &error) {
            if (++s->metrics[2] == 1 || s->metrics[2] % 1000 == 0) {
                c->logger->error("Java packet listener failed ({} total): {}", s->metrics[2], error.what());
            }
        }
        if (timed) {
            auto elapsed = static_cast<std::uint64_t>(
                std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start).count());
            s->metrics[5] += elapsed;
            s->metrics[6] = std::max(s->metrics[6], elapsed);
            if (elapsed >= 10'000'000 && (++s->metrics[4] == 1 || s->metrics[4] % 100 == 0)) {
                c->logger->warning("Slow Java packet listener: {} us ({} slow calls)", elapsed / 1000, s->metrics[4]);
            }
        }
    }
    catch (const std::exception &error) {
        try {
            c->logger->error("Java packet observation failed: {}", error.what());
        }
        catch (...) { /* Even diagnostic failures must not escape this noexcept callback. */
        }
    }
    catch (...) { /* Observation must never alter native protocol processing. */
    }
}
void remove(Context &c, NetworkState &s, jlong token)
{
    auto it = s.listeners.find(token);
    if (it == s.listeners.end()) {
        return;
    }
    auto listener = it->second;
    s.listeners.erase(it);
    listener->active = false;
    listener->callback.reset();
    for (auto *handler : listener->handlers) {
        manager(c).unregisterManagedEvent(*handler);
    }
}
}  // namespace
void closeNetwork(Context &c)
{
    if (!c.network) {
        return;
    }
    auto s = std::move(c.network);
    while (!s->listeners.empty()) {
        try {
            remove(c, *s, s->listeners.begin()->first);
        }
        catch (const std::exception &error) {
            c.logger->error("Network listener cleanup failed: {}", error.what());
        }
    }
    for (auto *handler : {s->quit, s->join}) {
        if (!handler) {
            continue;
        }
        try {
            manager(c).unregisterManagedEvent(*handler);
        }
        catch (const std::exception &error) {
            c.logger->error("Connection cleanup failed: {}", error.what());
        }
    }
}
jlong networkListenerCount(Context &c)
{
    return c.network ? static_cast<jlong>(c.network->listeners.size()) : 0;
}
}  // namespace endstone::core::java::details
namespace endstone::core::java {
using namespace details;
namespace {
jobjectArray JNICALL networkInfo(JNIEnv *e, jclass, jlong owner)
{
    return boundary(e, [&] {
        auto c = context(owner, true);
        std::vector<std::string> values{c->server->getMinecraftVersion(),
                                        std::to_string(c->server->getProtocolVersion()), compatible(*c) ? "1" : "0"};
        if (compatible(*c)) {
            for (auto [name, packet] :
                 std::array{std::pair{"Text", MinecraftPacketIds::Text},
                            std::pair{"MovePlayer", MinecraftPacketIds::MovePlayer},
                            std::pair{"ShowModalForm", MinecraftPacketIds::ShowModalForm},
                            std::pair{"ModalFormResponse", MinecraftPacketIds::ModalFormResponse}}) {
                values.emplace_back(name);
                values.emplace_back(std::to_string(static_cast<int>(packet)));
            }
        }
        return strings(e, values);
    });
}
jlong JNICALL networkListen(JNIEnv *e, jclass, jlong owner, jint directions, jintArray ids, jint priority,
                            jobject callback)
{
    return boundary(e, [&] {
        auto c = context(owner, true);
        if (!compatible(*c)) {
            throw std::runtime_error("Unsupported BDS release/protocol for packet observation");
        }
        if (!callback || !ids || directions < 1 || directions > 3 || priority < 0 || priority > 5) {
            throw std::invalid_argument("Invalid packet registration");
        }
        auto count = e->GetArrayLength(ids);
        checked(e);
        if (count > 128) {
            throw std::runtime_error("Packet filter limit exceeded");
        }
        std::vector<jint> filters(count);
        e->GetIntArrayRegion(ids, 0, count, filters.data());
        checked(e);
        auto s = state(c);
        if (s->listeners.size() >= 64) {
            throw std::runtime_error("Packet listener limit exceeded (64 per plugin)");
        }
        auto listener = std::make_shared<NetworkListener>();
        for (auto value : filters) {
            if (value < 0 || value > 1023) {
                throw std::runtime_error("Invalid packet filter identifier");
            }
            listener->ids.insert(value);
        }
        auto cls = e->GetObjectClass(callback);
        checked(e);
        listener->method = e->GetMethodID(cls, "accept", "(IIJJIZLjava/lang/String;Ljava/lang/String;)V");
        checked(e);
        listener->callback = std::make_shared<Ref>(e, callback);
        auto token = id();
        s->listeners.emplace(token, listener);
        std::weak_ptr<Context> weak = c;
        std::weak_ptr<NetworkState> weakState = s;
        std::weak_ptr<NetworkListener> weakListener = listener;
        try {
            if (directions & 1) {
                listener->handlers.push_back(manager(*c).registerManagedEvent(
                    PacketReceiveEvent::NAME,
                    [weak, weakState, weakListener](Event &event) {
                        observe(weak, weakState, weakListener, static_cast<PacketReceiveEvent &>(event), 0);
                    },
                    static_cast<EventPriority>(priority), *c->plugin, false));
            }
            if (directions & 2) {
                listener->handlers.push_back(manager(*c).registerManagedEvent(
                    PacketSendEvent::NAME,
                    [weak, weakState, weakListener](Event &event) {
                        observe(weak, weakState, weakListener, static_cast<PacketSendEvent &>(event), 1);
                    },
                    static_cast<EventPriority>(priority), *c->plugin, false));
            }
        }
        catch (...) {
            remove(*c, *s, token);
            throw;
        }
        return token;
    });
}
void JNICALL networkRemove(JNIEnv *e, jclass, jlong owner, jlong token)
{
    boundary(e, [&] {
        auto c = context(owner, true);
        if (c->network) {
            remove(*c, *c->network, token);
        }
    });
}
jlongArray JNICALL networkMetrics(JNIEnv *e, jclass, jlong owner, jint diagnostics)
{
    return boundary(e, [&] {
        auto c = context(owner, true);
        auto s = state(c);
        if (diagnostics < -1 || diagnostics > 1) {
            throw std::invalid_argument("Invalid diagnostics mode");
        }
        if (diagnostics >= 0) {
            s->diagnostics = diagnostics != 0;
        }
        jlong values[8]{};
        for (int i = 0; i < 7; ++i) {
            values[i] = static_cast<jlong>(s->metrics[i]);
        }
        values[3] = static_cast<jlong>(s->wrongThreadDrops.load());
        values[7] = s->diagnostics;
        auto out = e->NewLongArray(8);
        checked(e);
        e->SetLongArrayRegion(out, 0, 8, values);
        checked(e);
        return out;
    });
}
jobjectArray JNICALL networkConnections(JNIEnv *e, jclass, jlong owner)
{
    return boundary(e, [&] {
        auto c = context(owner, true);
        if (!compatible(*c)) {
            throw std::runtime_error("Unsupported connection view protocol");
        }
        auto s = state(c);
        std::vector<std::string> values;
        for (auto *p : c->server->getOnlinePlayers()) {
            if (!p) {
                continue;
            }
            auto key = connectionId(*c, *s, *p);
            if (!key.empty()) {
                values.emplace_back(key);
                values.emplace_back(s->connections.at(key).playerText);
            }
        }
        return strings(e, values);
    });
}
jboolean JNICALL networkActive(JNIEnv *e, jclass, jlong owner, jstring key)
{
    return boundary(e, [&] {
        auto c = context(owner, true);
        auto value = text(e, key);
        if (value.size() > 32) {
            throw std::invalid_argument("Invalid connection identifier");
        }
        return static_cast<jboolean>(c->network && active(*c, *c->network, value));
    });
}
}  // namespace
void registerNetwork(JNIEnv *e)
{
    auto cls = e->FindClass("dev/zernyth/runtime/Native");
    callbackCheck(e);
#define METHOD(name, signature) \
    {const_cast<char *>(#name), const_cast<char *>(signature), reinterpret_cast<void *>(name)}
    JNINativeMethod methods[] = {METHOD(networkInfo, "(J)[Ljava/lang/String;"),
                                 METHOD(networkListen, "(JI[IILdev/zernyth/runtime/Native$PacketCallback;)J"),
                                 METHOD(networkRemove, "(JJ)V"),
                                 METHOD(networkMetrics, "(JI)[J"),
                                 METHOD(networkConnections, "(J)[Ljava/lang/String;"),
                                 METHOD(networkActive, "(JLjava/lang/String;)Z")};
#undef METHOD
    if (e->RegisterNatives(cls, methods, static_cast<jint>(std::size(methods))) != JNI_OK) {
        callbackCheck(e);
        throw std::runtime_error("Network RegisterNatives failed");
    }
}
}  // namespace endstone::core::java
