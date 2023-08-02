//
// Created by Vincent on 31/07/2023.
//

#include "server.h"
#include "logger.h"
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
    ENDSTONE_INFO(SERVER, "Enabling plugins...")
    //    printf("Enabling plugins...\n");

    auto &plugins = getPluginManager().getPlugins();
    for (const auto &plugin : plugins)
    {
        // TODO: isEnabled check
        enablePlugin(plugin);
    }
}
void Server::enablePlugin(const py::object &plugin) const
{
    // TODO:

    //    auto state = PyGILState_Ensure();
    py::gil_scoped_acquire acquire{};
    plugin.attr("on_enable")();
    //    PyGILState_Release(state);
}
