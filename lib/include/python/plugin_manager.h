//
// Created by Vincent on 22/07/2023.
//

#ifndef ENDSTONE_PLUGIN_MANAGER_H
#define ENDSTONE_PLUGIN_MANAGER_H

#include <utility>

#include "endstone.h"
#include "plugin.h"

class Server;

class PluginManager
{
  public:
    explicit PluginManager(Server &server){};
    virtual ~PluginManager() = default;

    virtual Plugin *loadPlugin(const std::string &path) = 0;
    virtual std::vector<Plugin *> loadPlugins(const std::string &directory) = 0;
    //    virtual void enablePlugin(const Plugin *plugin) const = 0;
    //    virtual void enablePlugins() const = 0;
    //    virtual void disablePlugin(const Plugin *plugin) const = 0;
    //    virtual void disablePlugins() const = 0;
    //    virtual void clearPlugins() = 0;
    //    virtual const std::vector<Plugin *> &getPlugins() const = 0;
};

class PyPluginManager : public PluginManager
{
    using PluginManager::PluginManager;
    Plugin *loadPlugin(const std::string &path) override;
    std::vector<Plugin *> loadPlugins(const std::string &directory) override;
};

#endif // ENDSTONE_PLUGIN_MANAGER_H
