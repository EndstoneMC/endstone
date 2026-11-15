#include <cstdlib>
#include <fstream>
#include <thread>

#include "endstone/block/block_state.h"
#include "endstone/core/logger_factory.h"
#include "endstone/core/plugin/java_gameplay_internal.h"
#include "endstone/core/plugin/java_plugin_loader.h"
#include "endstone/core/plugin/plugin_manager.h"
#include "endstone/core/plugin/service_manager.h"
#include "endstone/core/scheduler/scheduler.h"
#include "endstone/zernyth_version.h"
#include "java_command_map.h"
#include "java_gameplay_mocks.h"
#include "mocks.h"
using namespace endstone;
using namespace endstone::core;
using testing::Return;
using testing::ReturnRef;

TEST(JavaEcosystemTest, DependenciesServicesConfigurationCommandsAndRollback)
{
    auto root = std::getenv("ZERNYTH_ECOSYSTEM_JARS");
    if (!root || !std::getenv("ZERNYTH_JVM_LIBRARY") || !std::getenv("ZERNYTH_JAVA_CLASSPATH")) {
        GTEST_SKIP() << "Set ecosystem JAR directory and JNI paths";
    }
    testing::NiceMock<MockServer> server;
    testing::NiceMock<GameplayMockConsole> console;
    EndstoneScheduler scheduler(server);
    EndstoneServiceManager services;
    GameplayCommands commands;
    EndstonePluginManager manager(server);
    bool permission = true;
    auto main = std::this_thread::get_id();
    ON_CALL(server, isPrimaryThread()).WillByDefault([&] { return main == std::this_thread::get_id(); });
    ON_CALL(server, getPluginManager()).WillByDefault(ReturnRef(manager));
    ON_CALL(server, getScheduler()).WillByDefault(ReturnRef(scheduler));
    ON_CALL(server, getServiceManager()).WillByDefault(ReturnRef(services));
    ON_CALL(server, getLogger()).WillByDefault(ReturnRef(LoggerFactory::getLogger("EcosystemTest")));
    ON_CALL(console, getName()).WillByDefault(Return("Console"));
    ON_CALL(console, asConsole()).WillByDefault(Return(&console));
    ON_CALL(console, hasPermission(testing::An<std::string>())).WillByDefault([&](std::string) { return permission; });
    manager.registerLoader(std::make_unique<JavaPluginLoader>(
        server, &commands, [&](const std::shared_ptr<Command> &c) { commands.entries.erase(c->getName()); }));
    auto jar = [&](const std::string &name) {
        return (std::filesystem::path(root) / ("runtime-" ZERNYTH_ENGINE_VERSION "-ecosystem-" + name + ".jar"))
            .string();
    };
    ASSERT_EQ(manager.loadPlugin(jar("missing")), nullptr);
    ASSERT_EQ(manager.loadPlugin(jar("badload")), nullptr);
    auto loaded = manager.loadPlugins(std::vector<std::string>{
        jar("hard"), jar("consumer"), jar("provider"), jar("early"), jar("cyclea"), jar("cycleb"), jar("missing")});
    ASSERT_EQ(loaded.size(), 4);
    EXPECT_EQ(loaded[0]->getName(), "eco_early");
    EXPECT_EQ(loaded[1]->getName(), "eco_provider");
    EXPECT_EQ(loaded[2]->getName(), "eco_consumer");
    EXPECT_EQ(loaded[3]->getName(), "eco_hard");
    auto *provider = manager.getPlugin("eco_provider"), *consumer = manager.getPlugin("eco_consumer"),
         *hard = manager.getPlugin("eco_hard");
    ASSERT_NE(provider, nullptr);
    ASSERT_NE(consumer, nullptr);
    ASSERT_NE(hard, nullptr);
    manager.enablePlugin(*hard);
    EXPECT_FALSE(hard->isEnabled());  // Dependency exists, but is disabled.
    manager.enablePlugins();
    ASSERT_TRUE(provider->isEnabled());
    ASSERT_TRUE(consumer->isEnabled());
    ASSERT_TRUE(hard->isEnabled());
    auto phase = [&](std::string name) {
        ASSERT_TRUE(commands.dispatch(console, "ecotest " + name)) << name;
    };
    // BDS attaches its server thread for individual JNI callbacks. Reattachment
    // creates a new Java Thread wrapper while preserving the native thread identity.
    ASSERT_EQ(endstone::core::java::details::jvm->DetachCurrentThread(), JNI_OK);
    phase("check");
    phase("worker");
    phase("class-conflict");
    phase("temporary-service");
    phase("alias-rollback");
    EXPECT_FALSE(commands.getCommand("tempalias"));
    EXPECT_TRUE(commands.dispatch(console, "ecoalias check"));
    EXPECT_TRUE(commands.dispatch(console, "t count 42"));
    EXPECT_TRUE(commands.dispatch(console, "typed count invalid"));
    EXPECT_TRUE(commands.dispatch(console, "typed player"));
    permission = false;
    EXPECT_TRUE(commands.dispatch(console, "ecoalias must-not-execute"));
    permission = true;
    auto config = consumer->getDataFolder() / "config.yml";
    {
        std::ofstream out(config);
        out << "server: [";
    }
    phase("bad-reload");
    {
        std::ofstream out(config);
        out << "server:\n  name: saved\n  max-players: 42\n";
    }
    // Native disable cascades only hard dependencies. Soft consumer remains to test revoked proxies.
    manager.disablePlugin(*provider);
    EXPECT_FALSE(provider->isEnabled());
    EXPECT_FALSE(hard->isEnabled());
    EXPECT_TRUE(consumer->isEnabled());
    EXPECT_FALSE(commands.getCommand("ecohard"));
    phase("revoked");
    EXPECT_TRUE(services.getProviders("zernyth.java:dev.zernyth.fixture.shared.Greeting").empty());
    manager.disablePlugin(*consumer);
    EXPECT_FALSE(commands.getCommand("ecoalias"));
    EXPECT_FALSE(commands.getCommand("typed"));
    EXPECT_FALSE(commands.getCommand("t"));
    auto providerPath = std::filesystem::path(jar("provider"));
    auto hiddenPath = providerPath;
    hiddenPath += ".temporarily-hidden";
    std::filesystem::rename(providerPath, hiddenPath);
    manager.enablePlugin(*provider);
    EXPECT_FALSE(provider->isEnabled());
    std::filesystem::rename(hiddenPath, providerPath);
    manager.enablePlugin(*provider);
    manager.enablePlugin(*consumer);
    ASSERT_TRUE(consumer->isEnabled());
    phase("check");
    auto *bad = manager.loadPlugin(jar("badenable"));
    ASSERT_NE(bad, nullptr);
    manager.enablePlugin(*bad);
    EXPECT_FALSE(bad->isEnabled());
    EXPECT_FALSE(commands.getCommand("orphan"));
    auto providers = services.getProviders("zernyth.java:dev.zernyth.fixture.shared.Greeting");
    EXPECT_EQ(providers.size(), 2);  // Only the healthy provider's registrations survived.
    auto duplicates = manager.loadPlugins(std::vector<std::string>{jar("early"), jar("early")});
    EXPECT_TRUE(duplicates.empty());
    manager.clearPlugins();
    EXPECT_TRUE(commands.entries.empty());
    EXPECT_TRUE(services.getProviders("zernyth.java:dev.zernyth.fixture.shared.Greeting").empty());
}
