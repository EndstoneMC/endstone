//
// Created by Vincent on 31/07/2023.
//

#include "endstone/server.h"
#include "endstone/plugin/python/python_plugin_loader.h"

Server::Server() : logger_(Logger::getLogger("Server"))
{
}

void Server::loadPlugins()
{
    // TODO: add C++ plugin loader
    pluginManager_->registerLoader(std::make_unique<PythonPluginLoader>("endstone.plugin", "ZipPluginLoader"));
    pluginManager_->registerLoader(std::make_unique<PythonPluginLoader>("endstone.plugin", "SourcePluginLoader"));
    auto pluginFolder = std::filesystem::current_path() / "plugins";

    if (exists(pluginFolder))
    {
        auto plugins = pluginManager_->loadPlugins(pluginFolder);
        for (const auto &plugin : plugins)
        {
            try
            {
                plugin->getLogger().info("Loading %s", plugin->getDescription().getFullName().c_str());
                plugin->onLoad();
            }
            catch (std::exception &e)
            {
                logger_.error("Error occurred when initializing %s: %s",
                              plugin->getDescription().getFullName().c_str(),
                              e.what());
            }
        }
    }
    else
    {
        create_directories(pluginFolder);
    }
}

void Server::enablePlugins()
{
    auto plugins = pluginManager_->getPlugins();
    for (const auto &plugin : plugins)
    {
        if (!plugin->isEnabled())
        {
            pluginManager_->enablePlugin(*plugin);
        }
    }
}

void Server::disablePlugins()
{
    pluginManager_->disablePlugins();
}

const Logger &Server::getLogger()
{
    return logger_;
}
