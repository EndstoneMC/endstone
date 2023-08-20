//
// Created by Vincent on 17/08/2023.
//

#ifndef ENDSTONE_PLUGIN_LOADER_H
#define ENDSTONE_PLUGIN_LOADER_H

class Plugin;

class PluginLoader
{
  public:
    virtual ~PluginLoader() = default;
    virtual Plugin *loadPlugin(const std::string &file) = 0;
    virtual std::vector<std::string> getPluginFilters() = 0;
    virtual void enablePlugin(const Plugin &plugin) = 0;
    virtual void disablePlugin(const Plugin &plugin) = 0;
};

#endif // ENDSTONE_PLUGIN_LOADER_H
