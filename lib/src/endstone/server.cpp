//
// Created by Vincent on 31/07/2023.
//

#include "endstone/server.h"
#include "endstone/logger_factory.h"
#include "endstone/plugin/cpp/cpp_plugin_loader.h"
#include "endstone/plugin/python/python_plugin_loader.h"

Server::Server() : logger_(LoggerFactory::getLogger("Server")), pluginManager_(std::make_unique<PluginManager>(*this))
{
}

void Server::loadPlugins()
{
    try
    {
        pluginManager_->registerLoader(std::make_unique<PythonPluginLoader>("endstone.plugin", "ZipPluginLoader"));
        pluginManager_->registerLoader(std::make_unique<PythonPluginLoader>("endstone.plugin", "SourcePluginLoader"));
        pluginManager_->registerLoader(std::make_unique<CppPluginLoader>());

        auto pluginFolder = std::filesystem::current_path() / "plugins";

        if (exists(pluginFolder))
        {
            auto plugins = pluginManager_->loadPlugins(pluginFolder);
            for (const auto &plugin : plugins)
            {
                try
                {
                    plugin->getLogger()->info("Loading {}", plugin->getDescription().getFullName());
                    plugin->onLoad();
                }
                catch (std::exception &e)
                {
                    logger_->error(
                        "Error occurred when initializing {}: {}", plugin->getDescription().getFullName(), e.what());
                }
            }
        }
        else
        {
            create_directories(pluginFolder);
        }
    }
    catch (std::exception &e)
    {
        logger_->error("Error occurred when trying to load plugins: {}", e.what());
    }
}

void Server::enablePlugins()
{
    auto plugins = pluginManager_->getPlugins();
    for (const auto &plugin : plugins)
    {
        pluginManager_->enablePlugin(*plugin);
    }
}

void Server::disablePlugins()
{
    pluginManager_->disablePlugins();
}

std::shared_ptr<Logger> Server::getLogger()
{
    return logger_;
}
