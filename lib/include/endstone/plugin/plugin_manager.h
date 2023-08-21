//
// Created by Vincent on 20/08/2023.
//

#ifndef ENDSTONE_PLUGIN_MANAGER_H
#define ENDSTONE_PLUGIN_MANAGER_H

#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_loader.h"

class Server;

class PluginManager
{
  public:
    explicit PluginManager(Server &server);

    void registerLoader(const std::shared_ptr<PluginLoader> &loader);
    Plugin *getPlugin(const std::string &name) const;
    std::vector<Plugin *> getPlugins() const;
    bool isPluginEnabled(const std::string &name) const;
    bool isPluginEnabled(Plugin *plugin) const;
    Plugin *loadPlugin(const std::filesystem::path &file);
    std::vector<Plugin *> loadPlugins(const std::filesystem::path &directory);
    void enablePlugin(Plugin &plugin) const;
    void disablePlugin(Plugin &plugin) const;
    void disablePlugins();
    void clearPlugins();

  private:
    Server &server_;
    std::map<std::string, std::shared_ptr<PluginLoader>> fileAssociations_;
    std::vector<std::unique_ptr<Plugin>> plugins_;
    std::map<std::string, Plugin *> lookupNames_;
};

#endif // ENDSTONE_PLUGIN_MANAGER_H
