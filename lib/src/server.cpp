//
// Created by Vincent on 31/07/2023.
//

#include "server.h"
#include "plugin.h"

Server::Server() : pluginManager(std::make_unique<PluginManager>(*this))
{
}

PluginManager &Server::getPluginManager() const
{
    return *pluginManager;
}

void Server::enablePlugins() const
{
    auto &plugins = getPluginManager().getPlugins();
    for (const auto &plugin : plugins)
    {
        //        BasePlugin *cls = plugin.cast<BasePlugin *>();
        //        printf("%p\n", cls);

        py::gil_scoped_acquire acquire;
        plugin.attr("on_enable")();

        //        if (!plugin.attr("is_enabled")().cast<bool>())
        //        {
        //            enablePlugin(plugin);
        //        }
    }
}
void Server::enablePlugin(const py::object &plugin) const
{
    // TODO:
}
