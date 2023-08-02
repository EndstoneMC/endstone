//
// Created by Vincent on 22/07/2023.
//

#include "plugin_manager.h"
#include "logger.h"

#include <iostream>
#include <toml++/toml.h>

PluginManager::PluginManager(Server &server) : _server(server)
{
}

std::vector<py::object> PluginManager::load_plugins(const std::filesystem::path &directory)
{
    ENDSTONE_INFO(SERVER, "Loading plugins...")

    std::vector<py::object> loaded_plugins;

    for (const auto &entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.is_directory())
        {
            auto &plugin_path = entry.path();
            if (std::filesystem::exists(plugin_path / "plugin.toml"))
            {
                auto plugin = load_plugin(plugin_path);
                if (plugin)
                {
                    loaded_plugins.push_back(plugin);
                }
            }
        }
    }

    return loaded_plugins;
}

py::object PluginManager::load_plugin(const std::filesystem::path &path)
{
    //    auto state = PyGILState_Ensure();
    auto config = toml::parse_file((path / "plugin.toml").string());

    std::string main = config["main"].value_or("");
    ENDSTONE_INFO(SERVER, "Loading plugin %s...", main.c_str());
    //    printf("Loading plugin %s ...\n", main.c_str());

    auto pos = main.find_last_of('.');
    auto module_name = path.filename().string() + "." + main.substr(0, pos);
    auto class_name = main.substr(pos + 1);

    py::gil_scoped_acquire acquire{};

    auto module = py::module::import(module_name.c_str());
    auto plugin = module.attr(class_name.c_str())();
    if (plugin)
    {
        plugins.push_back(plugin);
    }
    //    PyGILState_Release(state);
    return plugin;
}
const std::vector<py::object> &PluginManager::getPlugins() const
{
    return plugins;
}
