#include <chrono>
#include <cstdlib>
#include <exception>
#include <optional>
#include <thread>

#include "endstone/block/block_state.h"
#include "endstone/core/logger_factory.h"
#include "endstone/core/plugin/java_gameplay_internal.h"
#include "endstone/core/plugin/java_plugin_loader.h"
#include "endstone/core/plugin/service_manager.h"
#include "endstone/core/scheduler/scheduler.h"
#include "endstone/event/player/player_join_event.h"
#include "endstone/event/server/packet_send_event.h"
#include "java_command_map.h"
#include "java_gameplay_mocks.h"
#include "mocks.h"

namespace {
jint JNICALL ping(JNIEnv *, jclass, jint value)
{
    return value + 1;
}
}  // namespace
TEST(JavaPerformanceTest, ReproducibleProductionBridgeHarness)
{
    using namespace endstone;
    using namespace endstone::core;
    auto jar = std::getenv("ZERNYTH_BENCHMARK_JAR");
    if (!jar || !std::getenv("ZERNYTH_PERF_OUTPUT")) {
        GTEST_SKIP() << "Set benchmark JAR/output and JNI paths";
    }
    testing::NiceMock<MockServer> server;
    testing::NiceMock<GameplayMockPlayer> player;
    testing::NiceMock<GameplayMockConsole> console;
    EndstoneScheduler scheduler(server);
    EndstoneServiceManager services;
    GameplayCommands commands;
    EndstonePluginManager manager(server);
    ON_CALL(server, isPrimaryThread()).WillByDefault(testing::Return(true));
    ON_CALL(server, getLogger()).WillByDefault(testing::ReturnRef(LoggerFactory::getLogger("Benchmark")));
    ON_CALL(server, getPluginManager()).WillByDefault(testing::ReturnRef(manager));
    ON_CALL(server, getScheduler()).WillByDefault(testing::ReturnRef(scheduler));
    ON_CALL(server, getServiceManager()).WillByDefault(testing::ReturnRef(services));
    ON_CALL(server, getMinecraftVersion()).WillByDefault(testing::Return("26.60"));
    ON_CALL(server, getProtocolVersion()).WillByDefault(testing::Return(2225));
    ON_CALL(server, getOnlinePlayers()).WillByDefault(testing::Return(std::vector<endstone::Player *>{&player}));
    ON_CALL(server, getPlayer(testing::An<UUID>())).WillByDefault(testing::Return(&player));
    ON_CALL(player, getName()).WillByDefault(testing::Return("Benchmark"));
    ON_CALL(player, getUniqueId()).WillByDefault(testing::Return(UUID{}));
    ON_CALL(console, asConsole()).WillByDefault(testing::Return(&console));
    manager.registerLoader(std::make_unique<JavaPluginLoader>(
        server, &commands, [&](const std::shared_ptr<Command> &c) { commands.entries.erase(c->getName()); }));
    auto *plugin = manager.loadPlugin(jar);
    ASSERT_NE(plugin, nullptr);
    manager.enablePlugin(*plugin);
    ASSERT_TRUE(plugin->isEnabled());
    java::details::Env env;
    auto e = env.e;
    auto cls = e->FindClass("dev/zernyth/runtime/BenchmarkHarness");
    java::details::callbackCheck(e);
    JNINativeMethod method{const_cast<char *>("ping"), const_cast<char *>("(I)I"), reinterpret_cast<void *>(ping)};
    ASSERT_EQ(e->RegisterNatives(cls, &method, 1), JNI_OK);
    auto basic = e->GetStaticMethodID(cls, "basic", "(Ljava/lang/String;)V");
    e->CallStaticVoidMethod(cls, basic, e->NewStringUTF(jar));
    java::details::callbackCheck(e);
    for (const auto &name : {"players", "strings", "services", "registration", "schedule"}) {
        ASSERT_TRUE(commands.dispatch(console, std::string("perfbench ") + name));
    }
    auto record = e->GetStaticMethodID(cls, "record", "(Ljava/lang/String;[JI)V");
    auto allocated = e->GetStaticMethodID(cls, "threadAllocatedBytes", "()J");
    auto recordAllocated = e->GetStaticMethodID(cls, "recordAllocatedBytes", "(JI)V");
    java::details::callbackCheck(e);
    auto measure = [&](const char *name, int count, auto action, bool worker = false, bool retained = false) {
        jlong samples[15]{};
        std::exception_ptr failure;
        jlong allocatedBytes = -1;
        auto run = [&] {
            try {
                // Retention is benchmark-scoped and ends before this owned worker exits.
                std::optional<java::details::Env> attachment;
                if (retained) {
                    attachment.emplace();
                }
                for (int w = 0; w < 3; ++w) {
                    for (int i = 0; i < count; ++i) {
                        action();
                    }
                }
                jlong before = -1;
                if (!worker) {
                    before = e->CallStaticLongMethod(cls, allocated);
                    java::details::callbackCheck(e);
                }
                for (auto &sample : samples) {
                    auto start = std::chrono::steady_clock::now();
                    for (int i = 0; i < count; ++i) {
                        action();
                    }
                    sample =
                        std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start)
                            .count();
                }
                if (!worker) {
                    auto after = e->CallStaticLongMethod(cls, allocated);
                    java::details::callbackCheck(e);
                    if (before >= 0 && after >= before) {
                        allocatedBytes = after - before;
                    }
                }
            }
            catch (...) {
                failure = std::current_exception();
            }
        };
        if (worker) {
            std::thread thread(run);
            thread.join();
        }
        else {
            run();
        }
        if (failure) {
            std::rethrow_exception(failure);
        }
        auto values = e->NewLongArray(15);
        e->SetLongArrayRegion(values, 0, 15, samples);
        e->CallStaticVoidMethod(cls, record, e->NewStringUTF(name), values, count);
        java::details::callbackCheck(e);
        e->CallStaticVoidMethod(cls, recordAllocated, allocatedBytes, count * 15);
        java::details::callbackCheck(e);
    };
    auto scope = [] {
        java::details::Env frame;
    };
    measure("jni-frame-already-attached", 1000, scope);
    measure("jni-frame-attach-detach", 200, scope, true);
    measure("jni-frame-worker-batch-attached", 1000, scope, true, true);
    auto *nativeListener =
        manager.registerManagedEvent("BenchmarkNativeEvent", [](Event &) {}, EventPriority::Normal, *plugin, false);
    class EventProbe final : public Event {
    public:
        std::string getEventName() const override { return "BenchmarkNativeEvent"; }
    } nativeEvent;
    measure("native-event-dispatch", 2000, [&] { manager.callEvent(nativeEvent); });
    manager.unregisterManagedEvent(*nativeListener);
    PlayerJoinEvent join(player, std::nullopt);
    measure("production-java-event-dispatch", 2000, [&] { manager.callEvent(join); });
    PacketSendEvent packet(&player, 9, "", {}, 0);
    measure("production-packet-listener", 2000, [&] { manager.callEvent(packet); });
    PacketSendEvent anonymous(nullptr, 1023, "", {}, 0);
    measure("production-packet-anonymous", 2000, [&] { manager.callEvent(anonymous); });

    std::unordered_map<UUID, endstone::Player *> players{{UUID{}, &player}};
    std::vector<std::unique_ptr<testing::NiceMock<GameplayMockPlayer>>> peers;
    std::vector<std::unique_ptr<PacketSendEvent>> traffic;
    for (int i = 0; i < 64; ++i) {
        UUID uuid{};
        uuid.begin()[15] = static_cast<std::uint8_t>(i + 1);
        auto peer = std::make_unique<testing::NiceMock<GameplayMockPlayer>>();
        ON_CALL(*peer, getUniqueId()).WillByDefault(testing::Return(uuid));
        ON_CALL(*peer, getId()).WillByDefault(testing::Return(100 + i));
        ON_CALL(*peer, getRuntimeId()).WillByDefault(testing::Return(200 + i));
        players.emplace(uuid, peer.get());
        traffic.push_back(std::make_unique<PacketSendEvent>(peer.get(), 9, "", SocketAddress{}, 0));
        peers.push_back(std::move(peer));
    }
    ON_CALL(server, getPlayer(testing::An<UUID>())).WillByDefault([&](UUID uuid) -> endstone::Player * {
        auto it = players.find(uuid);
        return it == players.end() ? nullptr : it->second;
    });
    std::size_t next = 0;
    measure("production-packet-64-players", 2000, [&] { manager.callEvent(*traffic[next++ % traffic.size()]); });
    ASSERT_TRUE(commands.dispatch(console, "perfbench packet-listeners"));
    measure("production-packet-four-listeners", 2000, [&] { manager.callEvent(packet); });

    int tick = 0;
    std::uint64_t executed = 0;
    measure("native-scheduler-enqueue-heartbeat", 200, [&] {
        scheduler.runTask(*plugin, [&] { ++executed; });
        scheduler.mainThreadHeartbeat(++tick);
    });
    measure("plugin-disable-enable-cleanup", 2, [&] {
        manager.disablePlugin(*plugin);
        manager.enablePlugin(*plugin);
    });
    manager.clearPlugins();
    auto finish = e->GetStaticMethodID(cls, "finish", "()V");
    e->CallStaticVoidMethod(cls, finish);
    java::details::callbackCheck(e);
}
