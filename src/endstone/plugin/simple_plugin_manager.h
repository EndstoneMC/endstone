//
// Created by Vincent on 25/08/2023.
//

#ifndef ENDSTONE_SIMPLE_PLUGIN_MANAGER_H
#define ENDSTONE_SIMPLE_PLUGIN_MANAGER_H

#include "endstone/command/simple_command_map.h"
#include "endstone/plugin/plugin_manager.h"

class SimplePluginManager : public PluginManager {

public:
    explicit SimplePluginManager(Server &server, SimpleCommandMap &&command_map);

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
    std::map<std::string, std::shared_ptr<PluginLoader>> file_associations_;
    std::vector<std::unique_ptr<Plugin>> plugins_;
    std::map<std::string, Plugin *> lookup_names_;
    SimpleCommandMap &command_map_;
};

#endif // ENDSTONE_SIMPLE_PLUGIN_MANAGER_H
