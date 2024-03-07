// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// required if we want to assert in release mode
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <filesystem>
#include <memory>

#include "endstone/detail/logger_factory.h"
#include "endstone/detail/plugin/cpp_plugin_loader.h"
#include "endstone/detail/plugin/plugin_manager.h"
#include "endstone/logger.h"
#include "endstone/plugin/plugin.h"
#include "endstone/server.h"

namespace fs = std::filesystem;

namespace endstone::detail {

class TestServer : public Server {
public:
    explicit TestServer() : logger_(LoggerFactory::getLogger("TestServer"))
    {
        plugin_manager_ = std::make_unique<EndstonePluginManager>(*this);
        plugin_manager_->registerLoader(std::make_unique<CppPluginLoader>(*this));
    };
    ~TestServer() override = default;

    [[nodiscard]] Logger &getLogger() const override
    {
        return logger_;
    }

    [[nodiscard]] PluginManager &getPluginManager() const override
    {
        return *plugin_manager_;
    }

    void loadPlugins()
    {
        auto plugin_dir = fs::current_path() / "plugins";
        plugin_manager_->loadPlugins(plugin_dir.string());
    }

    void enablePlugins() const
    {
        plugin_manager_->enablePlugins();
    }

    void disablePlugins() const
    {
        plugin_manager_->disablePlugins();
    }

    [[nodiscard]] PluginCommand *getPluginCommand(std::string name) const override
    {
        return nullptr;
    }
    [[nodiscard]] PluginCommand *registerPluginCommand(std::shared_ptr<PluginCommand> command) const override
    {
        return nullptr;
    }

    [[nodiscard]] std::string getName() const override
    {
        return "TestServer";
    }

    [[nodiscard]] std::string getVersion() const override
    {
        return "test(endstone)";
    }

    [[nodiscard]] std::string getMinecraftVersion() const override
    {
        return "test(minecraft)";
    }

private:
    Logger &logger_;
    std::unique_ptr<EndstonePluginManager> plugin_manager_;
};
}  // namespace endstone::detail

using endstone::detail::TestServer;

void testLogger(TestServer &server)
{
    auto &logger = server.getLogger();
    logger.debug("Hello World!");
    logger.info("Hello World!");
    logger.warning("Hello World!");
    logger.error("Hello World!");
    logger.critical("Hello World!");
}

auto constexpr TestPluginName = "TestPlugin";

void testPluginLoading(TestServer &server)
{
    auto &plugin_manager = server.getPluginManager();
    assert(plugin_manager.getPlugins().empty());
    assert(plugin_manager.getPlugin(TestPluginName) == nullptr);

    server.loadPlugins();
    assert(plugin_manager.getPlugins().size() == 1);
    assert(plugin_manager.getPlugin(TestPluginName) != nullptr);
}

void testPluginEnabling(TestServer &server)
{
    auto &plugin_manager = server.getPluginManager();

    auto *plugin = plugin_manager.getPlugin(TestPluginName);
    assert(plugin != nullptr);
    assert(plugin_manager.isPluginEnabled(plugin) == false);
    assert(plugin_manager.isPluginEnabled(TestPluginName) == false);

    server.enablePlugins();
    assert(plugin_manager.isPluginEnabled(plugin) == true);
    assert(plugin_manager.isPluginEnabled(TestPluginName) == true);
}

void testPluginDisabling(TestServer &server)
{
    auto &plugin_manager = server.getPluginManager();

    server.disablePlugins();

    auto *plugin = plugin_manager.getPlugin(TestPluginName);
    assert(plugin != nullptr);
    assert(plugin_manager.isPluginEnabled(plugin) == false);
    assert(plugin_manager.isPluginEnabled(TestPluginName) == false);
}

int main()
{
    TestServer server;
    testLogger(server);
    testPluginLoading(server);
    testPluginEnabling(server);
    testPluginDisabling(server);
    server.getLogger().info("All tests passed. Bye!");
}
