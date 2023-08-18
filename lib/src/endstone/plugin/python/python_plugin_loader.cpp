//
// Created by Vincent on 18/08/2023.
//

#include "endstone/plugin/python/python_plugin_loader.h"
#include "common.h"

Plugin *PyPluginLoader::loadPlugin(const std::string &file)
{
    PYBIND11_OVERRIDE_PURE_NAME(Plugin *, PluginLoader, "load_plugin", loadPlugin);
}

std::vector<std::string> PyPluginLoader::getPluginFilters()
{
    PYBIND11_OVERRIDE_PURE_NAME(std::vector<std::string>, PluginLoader, "get_plugin_filters", getPluginFilters);
}

void PyPluginLoader::enablePlugin(const Plugin &plugin)
{
    PYBIND11_OVERRIDE_PURE_NAME(void, PluginLoader, "enable_plugin", enablePlugin);
}

void PyPluginLoader::disablePlugin(const Plugin &plugin)
{
    PYBIND11_OVERRIDE_PURE_NAME(void, PluginLoader, "disable_plugin", disablePlugin);
}
