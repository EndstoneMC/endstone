// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "endstone/detail/plugin/plugin_manager.h"

#include <algorithm>
#include <filesystem>
#include <memory>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "endstone/detail/logger_factory.h"
#include "endstone/detail/util/error.h"
#include "endstone/event/event.h"
#include "endstone/event/event_handler.h"
#include "endstone/event/handler_list.h"
#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_loader.h"
#include "endstone/scheduler/scheduler.h"
#include "endstone/server.h"

namespace fs = std::filesystem;

namespace endstone::detail {

EndstonePluginManager::EndstonePluginManager(Server &server)
    : server_(server), default_perms_({{true, {}}, {false, {}}})
{
}

void EndstonePluginManager::registerLoader(std::unique_ptr<PluginLoader> loader)
{
    plugin_loaders_.push_back(std::move(loader));
}

Plugin *EndstonePluginManager::getPlugin(const std::string &name) const
{
    auto it = lookup_names_.find(name);
    if (it != lookup_names_.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<Plugin *> EndstonePluginManager::getPlugins() const
{
    std::vector<Plugin *> plugins;
    plugins.reserve(plugins_.size());
    for (const auto &plugin : plugins_) {
        plugins.push_back(plugin);
    }
    return plugins;
}

bool EndstonePluginManager::isPluginEnabled(const std::string &name) const
{
    return isPluginEnabled(getPlugin(name));
}

bool EndstonePluginManager::isPluginEnabled(Plugin *plugin) const
{
    if (!plugin) {
        return false;
    }

    // Check if the plugin exists in the vector
    auto it = std::find_if(plugins_.begin(), plugins_.end(), [plugin](const auto &p) { return p == plugin; });

    // If plugin is in the vector and is enabled, return true
    return it != plugins_.end() && plugin->isEnabled();
}

Plugin *EndstonePluginManager::loadPlugin(std::string file)
{
    Plugin *result = nullptr;
    for (const auto &loader : plugin_loaders_) {
        for (const auto &pattern : loader->getPluginFileFilters()) {
            if (std::regex r(pattern); std::regex_search(file, r)) {
                if (auto *plugin = loader->loadPlugin(file); plugin) {
                    if (initPlugin(*plugin, *loader, fs::path(file).parent_path())) {
                        result = plugin;
                    }
                    break;
                }
            }
        }
    }
    return result;
}

std::vector<Plugin *> EndstonePluginManager::loadPlugins(std::string directory)
{
    std::vector<Plugin *> loaded_plugins;

    // TODO(plugin): handling logic for depend, soft_depend, load_before and provides

    // Create a copy of the current plugin loaders.
    // This is necessary because some plugins might register new loaders during their `onLoad` phase,
    // which could modify the `plugin_loaders_` vector and invalidate the iterator.
    std::vector<PluginLoader *> loaders;
    for (const auto &plugin_loader : plugin_loaders_) {
        loaders.emplace_back(plugin_loader.get());
    }

    // Iterate over the copied loaders to load plugins from the specified directory.
    for (const auto &loader : loaders) {
        auto plugins = loader->loadPlugins(directory);
        for (auto *plugin : plugins) {
            if (!plugin) {
                continue;
            }
            if (initPlugin(*plugin, *loader, fs::path(directory))) {
                loaded_plugins.push_back(plugin);
            }
        }
    }

    return loaded_plugins;
}

std::vector<Plugin *> EndstonePluginManager::loadPlugins(std::vector<std::string> files)
{
    std::vector<Plugin *> loaded_plugins;

    // TODO(plugin): handling logic for depend, soft_depend, load_before and provides
    for (const auto &file : files) {
        if (auto *plugin = loadPlugin(file)) {
            loaded_plugins.push_back(plugin);
        }
    }

    return loaded_plugins;
}

void EndstonePluginManager::enablePlugin(Plugin &plugin) const
{
    if (!plugin.isEnabled()) {
        plugin.getPluginLoader().enablePlugin(plugin);
    }
}

void EndstonePluginManager::enablePlugins() const
{
    for (const auto &plugin : plugins_) {
        enablePlugin(*plugin);
    }
}

void EndstonePluginManager::disablePlugin(Plugin &plugin)
{
    if (plugin.isEnabled()) {
        plugin.getPluginLoader().disablePlugin(plugin);
        server_.getScheduler().cancelTasks(plugin);
        for (auto &[name, handler] : event_handlers_) {
            handler.unregister(plugin);
        }
    }
}

void EndstonePluginManager::disablePlugins()
{
    for (const auto &plugin : plugins_) {
        disablePlugin(*plugin);
    }
}

void EndstonePluginManager::clearPlugins()
{
    disablePlugins();
    plugins_.clear();
    lookup_names_.clear();
    // TODO: recreate dependency graph
    event_handlers_.clear();
    plugin_loaders_.clear();
    permissions_.clear();
    default_perms_[true].clear();
    default_perms_[false].clear();
}

void EndstonePluginManager::callEvent(Event &event)
{
    if (event.isAsynchronous() && server_.isPrimaryThread()) {
        server_.getLogger().error("{} cannot be triggered asynchronously from server thread.", event.getEventName());
        return;
    }

    if (!event.isAsynchronous() && !server_.isPrimaryThread()) {
        server_.getLogger().error("{} must be triggered synchronously from server thread.", event.getEventName());
        return;
    }

    auto &handler_list = event_handlers_.emplace(event.getEventName(), event.getEventName()).first->second;
    for (const auto &handler : handler_list.getHandlers()) {
        auto &plugin = handler->getPlugin();
        if (!plugin.isEnabled()) {
            continue;
        }

        try {
            handler->callEvent(event);
        }
        catch (std::exception &e) {
            server_.getLogger().error("Could not pass event {} to plugin {}. {}", event.getEventName(),
                                      plugin.getDescription().getFullName(), e.what());
        }
    }
}

Result<void> EndstonePluginManager::registerEvent(std::string event, std::function<void(Event &)> executor,
                                                  EventPriority priority, Plugin &plugin, bool ignore_cancelled)
{
    if (!plugin.isEnabled()) {
        return nonstd::make_unexpected(
            make_error("Plugin {} attempted to register listener for event {} while not enabled.",
                       plugin.getDescription().getFullName(), event));
    }

    auto &handler_list = event_handlers_.emplace(event, event).first->second;
    const auto *handler = handler_list.registerHandler(
        std::make_unique<EventHandler>(event, executor, priority, plugin, ignore_cancelled));
    if (!handler) {
        return nonstd::make_unexpected(
            make_error("Plugin {} failed to register listener for event {}: Handler type mismatch",
                       plugin.getDescription().getFullName(), event));
    }
    return {};
}

Permission *EndstonePluginManager::getPermission(std::string name) const
{
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
    const auto it = permissions_.find(name);
    if (it == permissions_.end()) {
        return nullptr;
    }
    return it->second.get();
}

Permission *EndstonePluginManager::addPermission(std::unique_ptr<Permission> perm)
{
    if (perm == nullptr) {
        server_.getLogger().error("The permission cannot be nullptr");
        return nullptr;
    }

    auto name = perm->getName();
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
    if (getPermission(name) != nullptr) {
        server_.getLogger().error("The permission {} is already defined!", name);
        return nullptr;
    }

    perm->init(*this);
    auto it = permissions_.emplace(name, std::move(perm)).first;
    calculatePermissionDefault(*it->second);
    return it->second.get();
}

void EndstonePluginManager::removePermission(Permission &perm)
{
    removePermission(perm.getName());
}

void EndstonePluginManager::removePermission(std::string name)
{
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
    permissions_.erase(name);
}

std::unordered_set<Permission *> EndstonePluginManager::getDefaultPermissions(bool op) const
{
    return default_perms_.at(op);
}

void EndstonePluginManager::recalculatePermissionDefaults(Permission &perm)
{
    if (getPermission(perm.getName()) != nullptr) {
        default_perms_.at(true).erase(&perm);
        default_perms_.at(false).erase(&perm);
        calculatePermissionDefault(perm);
    }
}

void EndstonePluginManager::calculatePermissionDefault(Permission &perm)
{
    if (perm.getDefault() == PermissionDefault::Operator || perm.getDefault() == PermissionDefault::True) {
        default_perms_.at(true).insert(&perm);
        dirtyPermissibles(true);
    }

    if (perm.getDefault() == PermissionDefault::NotOperator || perm.getDefault() == PermissionDefault::True) {
        default_perms_.at(false).insert(&perm);
        dirtyPermissibles(false);
    }
}

void EndstonePluginManager::dirtyPermissibles(bool op) const
{
    auto permissibles = getDefaultPermSubscriptions(op);
    for (auto *p : permissibles) {
        p->recalculatePermissions();
    }
}

void EndstonePluginManager::subscribeToPermission(std::string permission, Permissible &permissible)
{
    auto &name = permission;
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
    auto &map = perm_subs_.emplace(name, std::unordered_map<Permissible *, bool>()).first->second;
    map[&permissible] = true;
}

void EndstonePluginManager::unsubscribeFromPermission(std::string permission, Permissible &permissible)
{
    auto &name = permission;
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
    auto it = perm_subs_.find(name);
    if (it != perm_subs_.end()) {
        auto &map = it->second;
        map.erase(&permissible);
        if (map.empty()) {
            perm_subs_.erase(name);
        }
    }
}

std::unordered_set<Permissible *> EndstonePluginManager::getPermissionSubscriptions(std::string permission) const
{
    auto &name = permission;
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });

    auto it = perm_subs_.find(name);
    if (it != perm_subs_.end()) {
        std::unordered_set<Permissible *> subs;
        const auto &map = it->second;
        for (const auto &entry : map) {
            subs.insert(entry.first);
        }
        return subs;
    }
    return {};
}

void EndstonePluginManager::subscribeToDefaultPerms(bool op, Permissible &permissible)
{
    auto &map = def_subs_.emplace(op, std::unordered_map<Permissible *, bool>()).first->second;
    map[&permissible] = true;
}

void EndstonePluginManager::unsubscribeFromDefaultPerms(bool op, Permissible &permissible)
{
    auto it = def_subs_.find(op);
    if (it != def_subs_.end()) {
        auto &map = it->second;
        map.erase(&permissible);
        if (map.empty()) {
            def_subs_.erase(op);
        }
    }
}

namespace {
std::string toCamelCase(const std::string &input)
{
    std::string output;
    bool capitalize = true;

    for (auto ch : input) {
        if (ch == '_') {
            capitalize = true;
        }
        else if (capitalize) {
            unsigned char uc = std::toupper(static_cast<unsigned char>(ch));
            output += static_cast<char>(uc);
            capitalize = false;
        }
        else {
            output += ch;
        }
    }
    return output;
}
}  // namespace

bool EndstonePluginManager::initPlugin(Plugin &plugin, PluginLoader &loader, const std::filesystem::path &base_folder)
{
    const static std::regex valid_name{"^[a-z0-9_]+$"};
    auto plugin_name = plugin.getDescription().getName();
    if (!std::regex_match(plugin_name, valid_name)) {
        server_.getLogger().error("Could not load plugin '{}': Plugin name contains invalid characters.", plugin_name);
        server_.getLogger().error(
            "A valid plugin name should only contain lowercase letters, numbers and underscores.");
        return false;
    }

    if (plugin_name.rfind("endstone", 0) == 0) {
        server_.getLogger().error("Could not load plugin '{}': Plugin name must not start with 'endstone'.",
                                  plugin_name);
        return false;
    }

    auto prefix = plugin.getDescription().getPrefix();
    if (prefix.empty()) {
        prefix = toCamelCase(plugin_name);
    }

    plugin.loader_ = &loader;
    plugin.server_ = &server_;
    plugin.logger_ = &LoggerFactory::getLogger(prefix);
    plugin.data_folder_ = base_folder / plugin_name;

    plugins_.push_back(&plugin);
    lookup_names_[plugin_name] = &plugin;

    plugin.getLogger().info("Loading {}", plugin.getDescription().getFullName());
    try {
        plugin.onLoad();
    }
    catch (std::exception &e) {
        plugin.getLogger().error("Error occurred when loading {}", plugin.getDescription().getFullName());
        plugin.getLogger().error(e.what());
    }
    return true;
}

std::unordered_set<Permissible *> EndstonePluginManager::getDefaultPermSubscriptions(bool op) const
{
    auto it = def_subs_.find(op);
    if (it != def_subs_.end()) {
        std::unordered_set<Permissible *> subs;
        const auto &map = it->second;
        for (const auto &entry : map) {
            subs.insert(entry.first);
        }
        return subs;
    }
    return {};
}

std::unordered_set<Permission *> EndstonePluginManager::getPermissions() const
{
    std::unordered_set<Permission *> perms;
    for (const auto &entry : permissions_) {
        perms.insert(entry.second.get());
    }
    return perms;
}

}  // namespace endstone::detail
