//
// Created by Vincent on 17/08/2023.
//

#ifndef ENDSTONE_PLUGIN_LOGGER_H
#define ENDSTONE_PLUGIN_LOGGER_H

#include "endstone/logger.h"

class Plugin;

class PluginLogger : public Logger
{
  public:
    explicit PluginLogger(const Plugin &plugin);
    void log(LogLevel level, const char *format, ...) const override;

  private:
    std::string pluginName_;
};

#endif // ENDSTONE_PLUGIN_LOGGER_H
