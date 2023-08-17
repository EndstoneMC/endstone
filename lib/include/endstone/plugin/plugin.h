//
// Created by Vincent on 21/07/2023.
//

#ifndef ENDSTONE_PLUGIN_H
#define ENDSTONE_PLUGIN_H

#include "endstone/logger.h"
#include "plugin_description.h"

class Plugin
{
  public:
    explicit Plugin() = default;
    virtual ~Plugin() = default;

    virtual PluginDescription &getDescription() const = 0;
    virtual void onLoad() = 0;
    virtual void onEnable() = 0;
    virtual void onDisable() = 0;
    virtual Logger &getLogger() = 0;
    virtual bool isEnabled() const = 0;
};

#endif // ENDSTONE_PLUGIN_H
