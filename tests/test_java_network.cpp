#include <cstdlib>
#include <filesystem>
#include <thread>

#include "bedrock/network/packet.h"
#include "endstone/block/block_state.h"
#include "endstone/core/logger_factory.h"
#include "endstone/core/plugin/java_plugin_loader.h"
#include "endstone/core/plugin/plugin_manager.h"
#include "endstone/core/plugin/service_manager.h"
#include "endstone/core/scheduler/scheduler.h"
#include "endstone/event/player/player_join_event.h"
#include "endstone/event/player/player_quit_event.h"
#include "endstone/event/server/packet_receive_event.h"
#include "endstone/event/server/packet_send_event.h"
#include "java_command_map.h"
#include "java_gameplay_mocks.h"
#include "mocks.h"
using namespace endstone;
using namespace endstone::core;
using testing::Return;
using testing::ReturnRef;

TEST(JavaNetworkTest, MetadataFiltersConnectionsAndCleanup)
{
    auto jar = std::getenv("ZERNYTH_NETWORK_TEST_JAR"), peerJar = std::getenv("ZERNYTH_NETWORK_PEER_TEST_JAR");
    if (!jar || !peerJar || !std::getenv("ZERNYTH_JVM_LIBRARY") || !std::getenv("ZERNYTH_JAVA_CLASSPATH")) {
        GTEST_SKIP() << "Set network fixture and JVM variables";
    }
    testing::NiceMock<MockServer> server;
    testing::NiceMock<GameplayMockPlayer> player;
    testing::NiceMock<GameplayMockConsole> console;
    EndstoneScheduler scheduler(server);
    EndstoneServiceManager services;
    GameplayCommands commands;
    EndstonePluginManager manager(server);
    UUID uuid;
    UUID peerUuid;
    peerUuid.begin()[0] = 1;
    testing::NiceMock<GameplayMockPlayer> peerPlayer;
    bool peerOnline = false;
    bool online = true;
    int protocol = 2225;
    std::uint64_t runtime = 420;
    std::string release = "26.60";
    auto main = std::this_thread::get_id();
    ON_CALL(server, isPrimaryThread()).WillByDefault([&] { return std::this_thread::get_id() == main; });
    ON_CALL(server, getLogger()).WillByDefault(ReturnRef(LoggerFactory::getLogger("NetworkTest")));
    ON_CALL(server, getPluginManager()).WillByDefault(ReturnRef(manager));
    ON_CALL(server, getScheduler()).WillByDefault(ReturnRef(scheduler));
    ON_CALL(server, getServiceManager()).WillByDefault(ReturnRef(services));
    ON_CALL(server, getMinecraftVersion()).WillByDefault([&] { return release; });
    ON_CALL(server, getProtocolVersion()).WillByDefault([&] { return protocol; });
    ON_CALL(server, getOnlinePlayers()).WillByDefault([&] {
        std::vector<endstone::Player *> players;
        if (online) {
            players.push_back(&player);
        }
        if (peerOnline) {
            players.push_back(&peerPlayer);
        }
        return players;
    });
    ON_CALL(server, getPlayer(testing::An<UUID>())).WillByDefault([&](UUID id) -> endstone::Player * {
        if (online && id == uuid) {
            return &player;
        }
        return peerOnline && id == peerUuid ? &peerPlayer : nullptr;
    });
    ON_CALL(peerPlayer, getUniqueId()).WillByDefault(Return(peerUuid));
    ON_CALL(peerPlayer, getId()).WillByDefault(Return(43));
    ON_CALL(peerPlayer, getRuntimeId()).WillByDefault(Return(500));
    ON_CALL(player, getUniqueId()).WillByDefault(Return(uuid));
    ON_CALL(player, getId()).WillByDefault(Return(42));
    ON_CALL(player, getRuntimeId()).WillByDefault([&] { return runtime; });
    ON_CALL(console, getName()).WillByDefault(Return("Console"));
    ON_CALL(console, asConsole()).WillByDefault(Return(&console));
    manager.registerLoader(
        std::make_unique<JavaPluginLoader>(server, &commands, [&](const std::shared_ptr<Command> &command) {
            commands.entries.erase(command->getName());
        }));
    std::filesystem::remove("network-late.marker");
    auto *plugin = manager.loadPlugin(jar);
    ASSERT_NE(plugin, nullptr);
    manager.enablePlugin(*plugin);
    ASSERT_TRUE(plugin->isEnabled());
    auto phase = [&](std::string name) {
        ASSERT_TRUE(commands.dispatch(console, "nettest " + name)) << name;
    };
    phase("init");
    phase("invalid");
    phase("foreign-filter");
    phase("worker");
    std::string payload = "secret";
    PacketReceiveEvent inbound(&player, static_cast<int>(MinecraftPacketIds::Text), payload, {}, 0);
    inbound.setCancelled(true);
    manager.callEvent(inbound);
    EXPECT_TRUE(inbound.isCancelled());
    EXPECT_EQ(inbound.getPayload(), payload);
    PacketSendEvent outgoing(&player, static_cast<int>(MinecraftPacketIds::Text), payload, {}, 0);
    manager.callEvent(outgoing);
    EXPECT_FALSE(outgoing.isCancelled());
    EXPECT_EQ(outgoing.getPayload(), payload);
    PacketSendEvent form(nullptr, static_cast<int>(MinecraftPacketIds::ShowModalForm), payload, {}, 1);
    manager.callEvent(form);
    PacketReceiveEvent unknown(nullptr, 1023, payload, {}, 0);
    manager.callEvent(unknown);
    PacketReceiveEvent invalidId(nullptr, -1, payload, {}, 0);
    manager.callEvent(invalidId);
    PacketSendEvent invalidSub(nullptr, 9, payload, {}, 4);
    manager.callEvent(invalidSub);
    std::thread worker([&] { manager.callEvent(unknown); });
    worker.join();
    phase("check");
    // Snapshots outlive the backing payload; Java never receives the raw bytes.
    payload.assign("changed");
    phase("self");
    manager.callEvent(unknown);
    manager.callEvent(unknown);
    phase("self-check");
    phase("slow");
    manager.callEvent(unknown);
    phase("slow-check");
    phase("version-count");
    protocol = 1;
    manager.callEvent(unknown);
    phase("unsupported");
    protocol = 2225;
    release = "other";
    manager.callEvent(unknown);
    phase("unsupported");
    release = "26.60";
    phase("version-unchanged");
    peerOnline = true;
    phase("multiple-connections");
    PacketSendEvent peerPacket(&peerPlayer, 9, payload, {}, 0);
    manager.callEvent(peerPacket);
    phase("multiple-player-packet");
    PlayerQuitEvent peerQuit(peerPlayer, std::nullopt);
    manager.callEvent(peerQuit);
    peerOnline = false;
    phase("multiple-removed");
    PlayerQuitEvent quit(player, std::nullopt);
    manager.callEvent(quit);
    online = false;
    phase("disconnect");
    online = true;
    PlayerJoinEvent join(player, std::nullopt);
    manager.callEvent(join);
    phase("rejoin");
    phase("remember-connection");
    runtime = 421;
    phase("runtime-changed");
    runtime = 420;
    phase("runtime-retired");
    phase("close");
    phase("limit");
    auto *peer = manager.loadPlugin(peerJar);
    ASSERT_NE(peer, nullptr);
    manager.enablePlugin(*peer);
    ASSERT_TRUE(peer->isEnabled());
    ASSERT_TRUE(commands.dispatch(console, "netpeer peer"));
    // Disable while Endstone is iterating its handler snapshot. Java must not be called afterward.
    manager.registerManagedEvent(
        PacketReceiveEvent::NAME, [&](Event &) { manager.disablePlugin(*plugin); }, EventPriority::Lowest, *plugin,
        false);
    manager.callEvent(unknown);
    EXPECT_FALSE(plugin->isEnabled());
    EXPECT_FALSE(std::filesystem::exists("network-late.marker"));
    ASSERT_TRUE(commands.dispatch(console, "netpeer peer-check"));
    manager.disablePlugin(*peer);
    EXPECT_FALSE(manager.isEventRegistered<PacketReceiveEvent>());
    EXPECT_FALSE(manager.isEventRegistered<PacketSendEvent>());
    EXPECT_FALSE(manager.isEventRegistered<PlayerQuitEvent>());
    EXPECT_FALSE(manager.isEventRegistered<PlayerJoinEvent>());
    manager.callEvent(unknown);
    EXPECT_FALSE(std::filesystem::exists("network-late.marker"));
    EXPECT_FALSE(commands.getCommand("nettest"));
    manager.clearPlugins();
}
