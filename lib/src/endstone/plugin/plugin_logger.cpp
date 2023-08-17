//
// Created by Vincent on 17/08/2023.
//

#include "endstone/plugin/plugin_logger.h"
#include "endstone/plugin/plugin.h"

PluginLogger::PluginLogger(const Plugin &plugin) : Logger(plugin.getDescription().getName())
{
    auto prefix = plugin.getDescription().getPrefix();
    if (prefix.has_value())
    {
        pluginName_ = prefix.value();
    }
    else
    {
        pluginName_ = plugin.getDescription().getName();
    }
}

void PluginLogger::log(LogLevel level, const char *format, ...) const
{
    Logger::log(level, "[%s] %s", pluginName_.c_str(), format);
}
