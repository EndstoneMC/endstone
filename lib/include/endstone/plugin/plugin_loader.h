//
// Created by Vincent on 17/08/2023.
//

#ifndef ENDSTONE_PLUGIN_LOADER_H
#define ENDSTONE_PLUGIN_LOADER_H

#include "plugin.h"

class PluginLoader
{
  public:
    virtual ~PluginLoader() = default;
    virtual Plugin *loadPlugin(const std::string &file) const = 0;
    virtual std::vector<std::string> getPluginFilters() const noexcept = 0;
    virtual void enablePlugin(Plugin &plugin) const = 0;
    virtual void disablePlugin(Plugin &plugin) const = 0;
};

#endif // ENDSTONE_PLUGIN_LOADER_H
