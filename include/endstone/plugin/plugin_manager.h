//
// Created by Vincent on 20/08/2023.
//

#ifndef ENDSTONE_PLUGIN_MANAGER_H
#define ENDSTONE_PLUGIN_MANAGER_H

#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_loader.h"

class PluginManager {
public:
    virtual ~PluginManager() = default;
    virtual void registerLoader(std::unique_ptr<PluginLoader> loader) = 0;
    virtual Plugin *getPlugin(const std::string &name) const = 0;
    virtual std::vector<Plugin *> getPlugins() const = 0;
    virtual bool isPluginEnabled(const std::string &name) const = 0;
    virtual bool isPluginEnabled(Plugin *plugin) const = 0;
    virtual Plugin *loadPlugin(const std::filesystem::path &file) = 0;
    virtual std::vector<Plugin *> loadPlugins(const std::filesystem::path &directory) = 0;
    virtual void enablePlugin(Plugin &plugin) const = 0;
    virtual void disablePlugin(Plugin &plugin) const = 0;
    virtual void disablePlugins() = 0;
    virtual void clearPlugins() = 0;
};

#endif // ENDSTONE_PLUGIN_MANAGER_H
