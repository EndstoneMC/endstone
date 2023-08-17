//
// Created by Vincent on 17/08/2023.
//

#include "endstone/plugin/python/python_plugin_description.h"

std::string PyPluginDescription::getName()
{
    PYBIND11_OVERRIDE_PURE_NAME(std::string, PluginDescription, "get_name", getName);
}

std::string PyPluginDescription::getVersion()
{
    PYBIND11_OVERRIDE_PURE_NAME(std::string, PluginDescription, "get_version", getVersion);
}

std::optional<std::string> PyPluginDescription::getDescription()
{
    PYBIND11_OVERRIDE_PURE_NAME(std::optional<std::string>, PluginDescription, "get_description", getDescription);
}

std::optional<std::vector<std::string>> PyPluginDescription::getAuthors()
{
    PYBIND11_OVERRIDE_PURE_NAME(std::optional<std::vector<std::string>>, PluginDescription, "get_authors", getAuthors);
}

std::optional<std::string> PyPluginDescription::getPrefix()
{
    PYBIND11_OVERRIDE_PURE_NAME(std::optional<std::string>, PluginDescription, "get_prefix", getPrefix);
}

std::string PyPluginDescription::getFullName()
{
    return getName() + " v" + getVersion();
}