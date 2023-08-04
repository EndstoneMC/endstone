//
// Created by Vincent on 22/07/2023.
//

#include "plugin_manager.h"
#include "logger.h"

#include <iostream>
#include <toml++/toml.h>

PluginManager::PluginManager(Server &server) : server_(server)
{
}

std::vector<py::object> PluginManager::loadPlugins(const std::filesystem::path &directory)
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
                auto plugin = loadPlugin(plugin_path);
                if (plugin)
                {
                    loaded_plugins.push_back(plugin);
                }
            }
        }
    }

    return loaded_plugins;
}

py::object PluginManager::loadPlugin(const std::filesystem::path &path)
{
    auto config = toml::parse_file((path / "plugin.toml").string());

    std::string main = config["main"].value_or("");
    ENDSTONE_INFO(SERVER, "Loading plugin %s...", main.c_str());

    auto pos = main.find_last_of('.');
    auto module_name = path.filename().string() + "." + main.substr(0, pos);
    auto class_name = main.substr(pos + 1);

    py::gil_scoped_acquire acquire{};

    auto module = py::module::import(module_name.c_str());
    auto plugin = module.attr(class_name.c_str())();
    if (plugin)
    {
        plugins_.push_back(plugin);
    }

    return plugin;
}

void PluginManager::enablePlugin(const py::object &plugin) const
{
    // TODO: check if already enabled
    py::gil_scoped_acquire acquire{};
    plugin.attr("on_enable")();
}

const std::vector<py::object> &PluginManager::getPlugins() const
{
    return plugins_;
}

void PluginManager::enablePlugins() const
{
    ENDSTONE_INFO(SERVER, "Enabling plugins...")
    for (const auto &plugin : plugins_)
    {
        enablePlugin(plugin);
    }
}

void PluginManager::disablePlugin(const py::object &plugin) const
{
    // TODO: check if already disabled
    py::gil_scoped_acquire acquire{};
    plugin.attr("on_disable")();
}

void PluginManager::disablePlugins() const
{
    ENDSTONE_INFO(SERVER, "Disabling plugins...")
    for (const auto &plugin : plugins_)
    {
        disablePlugin(plugin);
    }
}

void PluginManager::clearPlugins()
{
    disablePlugins();
    {
        py::gil_scoped_acquire acquire{};
        plugins_.clear();
    }
}
