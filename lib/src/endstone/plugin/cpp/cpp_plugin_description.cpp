//
// Created by Vincent on 17/08/2023.
//

#include "endstone/plugin/cpp/cpp_plugin_description.h"

CppPluginDescription::CppPluginDescription(std::string name, std::string version)
    : name_(std::move(name)),
      version_(std::move(version))
{
}

std::string CppPluginDescription::getName()
{
    return name_;
}

std::string CppPluginDescription::getVersion()
{
    return version_;
}

std::optional<std::string> CppPluginDescription::getDescription()
{
    return {};
}

std::optional<std::vector<std::string>> CppPluginDescription::getAuthors()
{
    return {};
}

std::optional<std::string> CppPluginDescription::getPrefix()
{
    return {};
}

std::string CppPluginDescription::getFullName()
{
    return getName() + " v" + getVersion();
}