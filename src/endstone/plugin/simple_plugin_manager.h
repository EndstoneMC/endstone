//
// Created by Vincent on 25/08/2023.
//

#ifndef ENDSTONE_SIMPLE_PLUGIN_MANAGER_H
#define ENDSTONE_SIMPLE_PLUGIN_MANAGER_H

#include "endstone/command/simple_command_map.h"
#include "endstone/plugin/plugin_manager.h"

class SimplePluginManager : public PluginManager {

public:
    explicit SimplePluginManager(Server &server, SimpleCommandMap &command_map);

    void registerLoader(std::unique_ptr<PluginLoader> loader) override;
    Plugin *getPlugin(const std::string &name) const override;
    std::vector<Plugin *> getPlugins() const override;
    bool isPluginEnabled(const std::string &name) const override;
    bool isPluginEnabled(Plugin *plugin) const override;
    Plugin *loadPlugin(const std::filesystem::path &file) override;
    std::vector<Plugin *> loadPlugins(const std::filesystem::path &directory) override;
    void enablePlugin(Plugin &plugin) const override;
    void disablePlugin(Plugin &plugin) const override;
    void disablePlugins() override;
    void clearPlugins() override;

private:
    Server &server_;
    std::map<std::string, std::unique_ptr<PluginLoader>> file_associations_;
    std::vector<std::unique_ptr<Plugin>> plugins_;
    std::map<std::string, Plugin *> lookup_names_;
    SimpleCommandMap &command_map_;
};

#endif // ENDSTONE_SIMPLE_PLUGIN_MANAGER_H
