//
// Created by Vincent on 17/08/2023.
//

#include "endstone/plugin/cpp/cpp_plugin.h"

void CppPlugin::onLoad()
{
}

void CppPlugin::onEnable()
{
}

void CppPlugin::onDisable()
{
}

Logger &CppPlugin::getLogger()
{
    return *logger_;
}

bool CppPlugin::isEnabled() const
{
    return enabled_;
}

void CppPlugin::setEnabled(bool enabled)
{
    if (enabled_ != enabled)
    {
        enabled_ = enabled;

        if (enabled_)
        {
            onEnable();
        }
        else
        {
            onDisable();
        }
    }
}

std::shared_ptr<PluginLoader> CppPlugin::getPluginLoader() const
{
    return loader_.lock();
}
