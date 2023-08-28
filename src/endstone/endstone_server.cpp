//
// Created by Vincent on 28/08/2023.
//

#include "endstone_server.h"

#include "endstone/plugin/cpp/cpp_plugin_loader.h"
#include "endstone/plugin/python/python_plugin_loader.h"
#include "endstone/plugin/simple_plugin_manager.h"
#include "logger_factory.h"

EndstoneServer::EndstoneServer()
    : logger_(LoggerFactory::getLogger("Server")), command_map_(std::make_shared<SimpleCommandMap>(*this)),
      plugin_manager_(std::make_unique<SimplePluginManager>(*this, command_map_))
{
}

void EndstoneServer::loadPlugins()
{
    try {
        plugin_manager_->registerLoader(std::make_unique<PythonPluginLoader>("endstone.plugin", "ZipPluginLoader"));
        plugin_manager_->registerLoader(std::make_unique<PythonPluginLoader>("endstone.plugin", "SourcePluginLoader"));
        plugin_manager_->registerLoader(std::make_unique<CppPluginLoader>());

        auto pluginFolder = std::filesystem::current_path() / "plugins";

        if (exists(pluginFolder)) {
            auto plugins = plugin_manager_->loadPlugins(pluginFolder);
            for (const auto &plugin : plugins) {
                try {
                    plugin->getLogger()->info("Loading {}", plugin->getDescription().getFullName());
                    plugin->onLoad();
                }
                catch (std::exception &e) {
                    logger_->error("Error occurred when initializing {}: {}", plugin->getDescription().getFullName(),
                                   e.what());
                }
            }
        }
        else {
            create_directories(pluginFolder);
        }
    }
    catch (std::exception &e) {
        logger_->error("Error occurred when trying to load plugins: {}", e.what());
    }
}

void EndstoneServer::enablePlugins()
{
    auto plugins = plugin_manager_->getPlugins();
    for (const auto &plugin : plugins) {
        plugin_manager_->enablePlugin(*plugin);
    }
}

void EndstoneServer::disablePlugins()
{
    plugin_manager_->disablePlugins();
}

std::shared_ptr<Logger> EndstoneServer::getLogger()
{
    return logger_;
}
