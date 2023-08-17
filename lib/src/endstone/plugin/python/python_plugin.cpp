//
// Created by Vincent on 17/08/2023.
//

#include "endstone/plugin/python/python_plugin.h"

PluginDescription &PyPlugin::getDescription() const
{
    PYBIND11_OVERLOAD_PURE_NAME(PluginDescription &, Plugin, "get_description", getDescription);
}

void PyPlugin::onLoad()
{
    PYBIND11_OVERLOAD_PURE_NAME(void, Plugin, "on_load", onLoad);
}

void PyPlugin::onEnable()
{
    PYBIND11_OVERLOAD_PURE_NAME(void, Plugin, "on_enable", onEnable);
}

void PyPlugin::onDisable()
{
    PYBIND11_OVERLOAD_PURE_NAME(void, Plugin, "on_disable", onDisable);
}

Logger &PyPlugin::getLogger()
{
    PYBIND11_OVERLOAD_PURE_NAME(Logger &, Plugin, "get_logger", onDisable);
}

bool PyPlugin::isEnabled() const
{
    PYBIND11_OVERLOAD_PURE_NAME(bool, Plugin, "is_enabled", isEnabled);
}
