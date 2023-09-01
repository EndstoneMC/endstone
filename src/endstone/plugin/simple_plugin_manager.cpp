//
// Created by Vincent on 25/08/2023.
//

#include "simple_plugin_manager.h"

#include <utility>

//
// Created by Vincent on 20/08/2023.
//

#include "endstone/command/plugin_command.h"
#include "endstone/plugin/plugin_manager.h"
#include "endstone/server.h"

SimplePluginManager::SimplePluginManager(Server &server, SimpleCommandMap &command_map)
    : server_(server), command_map_(command_map)
{
}

void SimplePluginManager::registerLoader(std::unique_ptr<PluginLoader> loader)
{
    auto patterns = loader->getPluginFileFilters();
    for (const auto &pattern : patterns) {
        file_associations_[pattern] = std::move(loader);
    }
}

Plugin *SimplePluginManager::getPlugin(const std::string &name) const
{
    auto it = lookup_names_.find(name);
    if (it != lookup_names_.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<Plugin *> SimplePluginManager::getPlugins() const
{
    std::vector<Plugin *> plugins;
    plugins.reserve(plugins_.size());
    for (const auto &plugin : plugins_) {
        plugins.push_back(plugin.get());
    }
    return plugins;
}

bool SimplePluginManager::isPluginEnabled(const std::string &name) const
{
    return isPluginEnabled(getPlugin(name));
}

bool SimplePluginManager::isPluginEnabled(Plugin *plugin) const
{
    if (!plugin) {
        return false;
    }

    // Check if the plugin exists in the vector
    auto it = std::find_if(plugins_.begin(), plugins_.end(), [plugin](const std::unique_ptr<Plugin> &p) {
        return p.get() == plugin;
    });

    // If plugin is in the vector and is enabled, return true
    return it != plugins_.end() && plugin->isEnabled();
}

Plugin *SimplePluginManager::loadPlugin(const std::filesystem::path &file)
{
    if (!exists(file)) {
        throw std::runtime_error("Provided file does not exist.");
    }

    for (const auto &[pattern, loader] : file_associations_) {
        std::regex r(pattern);
        if (std::regex_search(file.string(), r)) {
            auto plugin = loader->loadPlugin(file.string());

            if (plugin) {
                auto plugin_ptr = plugin.get();
                lookup_names_[plugin->getDescription().getName()] = plugin_ptr;
                plugins_.push_back(std::move(plugin));
                return plugin_ptr;
            }
        }
    }

    return nullptr;
}

std::vector<Plugin *> SimplePluginManager::loadPlugins(const std::filesystem::path &directory)
{
    if (!std::filesystem::exists(directory)) {
        throw std::runtime_error("Provided directory does not exist.");
    }

    if (!std::filesystem::is_directory(directory)) {
        throw std::runtime_error("Provided path is not a directory.");
    }

    std::vector<Plugin *> loadedPlugins;

    for (const auto &entry : std::filesystem::directory_iterator(directory)) {
        std::filesystem::path file;

        // If it's a regular file, try to load it as a plugin.
        if (std::filesystem::is_regular_file(entry.status())) {
            file = entry.path();
        }
        // If it's a subdirectory, look for a plugin.toml inside it.
        else if (std::filesystem::is_directory(entry.status())) {
            file = entry.path() / "plugin.toml";
            if (!std::filesystem::exists(file) || !std::filesystem::is_regular_file(file)) {
                continue;
            }
        }
        else {
            continue;
        }

        try {
            auto plugin = loadPlugin(file);
            if (plugin) {
                loadedPlugins.push_back(plugin);
            }
        }
        catch (std::exception &e) {
            server_.getLogger()->error("Could not load '{}' in folder '{}': {}", file.string(), directory.string(),
                                       e.what());
        }
    }

    return loadedPlugins;
}

void SimplePluginManager::enablePlugin(Plugin &plugin) const
{
    if (!plugin.isEnabled()) {
        try {
            auto commands = plugin.getDescription().getCommands();

            if (!commands.empty()) {
                auto plugin_commands = std::vector<std::shared_ptr<Command>>(commands.size());
                std::transform(commands.begin(), commands.end(), plugin_commands.begin(),
                               [&plugin](const auto &command) {
                                   return std::make_shared<PluginCommand>(*command, plugin);
                               });

                command_map_.registerAll(plugin.getDescription().getName(), plugin_commands);
            }

            plugin.getPluginLoader().enablePlugin(plugin);
        }
        catch (std::exception &e) {
            server_.getLogger()->error("Error occurred (in the plugin loader) while enabling {}: {}",
                                       plugin.getDescription().getFullName(), e.what());
        }
    }
}

void SimplePluginManager::disablePlugin(Plugin &plugin) const
{
    if (plugin.isEnabled()) {
        try {
            plugin.getPluginLoader().disablePlugin(plugin);
        }
        catch (std::exception &e) {
            server_.getLogger()->error("Error occurred (in the plugin loader) while disabling {}: {}",
                                       plugin.getDescription().getFullName(), e.what());
        }
    }
}

void SimplePluginManager::disablePlugins()
{
    for (const auto &plugin : plugins_) {
        disablePlugin(*plugin);
    }
}

void SimplePluginManager::clearPlugins()
{
    disablePlugins();
    plugins_.clear();
    lookup_names_.clear();
}
