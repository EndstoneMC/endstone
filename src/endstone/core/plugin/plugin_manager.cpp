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

#include "endstone/core/plugin/plugin_manager.h"

#include <algorithm>
#include <filesystem>
#include <memory>
#include <ranges>
#include <regex>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <boost/algorithm/string/predicate.hpp>

#include "endstone/command/command_map.h"
#include "endstone/command/plugin_command.h"
#include "endstone/core/logger_factory.h"
#include "endstone/event/event.h"
#include "endstone/event/event_handler.h"
#include "endstone/event/handler_list.h"
#include "endstone/level/dimension.h"
#include "endstone/level/level.h"
#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_loader.h"
#include "endstone/scheduler/scheduler.h"
#include "endstone/server.h"

namespace fs = std::filesystem;

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

std::unordered_set<endstone::NotNull<endstone::Permissible>> lockAndPrune(auto &subs)
{
    std::unordered_set<endstone::NotNull<endstone::Permissible>> result;
    for (auto it = subs.begin(); it != subs.end();) {
        if (auto permissible = it->lock()) {
            result.emplace(std::move(permissible));
            ++it;
        }
        else {
            it = subs.erase(it);
        }
    }
    return result;
}
}  // namespace

namespace endstone::core {

EndstonePluginManager::EndstonePluginManager(Server &server) : server_(server)
{
    default_perms_[PermissionLevel::Default] = {};
    default_perms_[PermissionLevel::Operator] = {};
    default_perms_[PermissionLevel::Console] = {};
}

EndstonePluginManager::~EndstonePluginManager()
{
    lookup_names_.clear();
    {
        decltype(event_handlers_) handlers;
        {
            std::unique_lock lock(event_handlers_mtx_);
            handlers.swap(event_handlers_);
        }
    }
    permissions_.clear();
    default_perms_[PermissionLevel::Default].clear();
    default_perms_[PermissionLevel::Operator].clear();
    default_perms_[PermissionLevel::Console].clear();
    while (!plugin_loaders_.empty()) {
        plugin_loaders_.pop_back();
    }
    while (!plugins_.empty()) {
        plugins_.pop_back();
    }
}

void EndstonePluginManager::registerLoader(std::unique_ptr<PluginLoader> loader)
{
    plugin_loaders_.push_back(std::move(loader));
}

Nullable<Plugin> EndstonePluginManager::getPlugin(const std::string &name) const
{
    auto it = lookup_names_.find(name);
    if (it != lookup_names_.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<NotNull<Plugin>> EndstonePluginManager::getPlugins() const
{
    return plugins_;
}

bool EndstonePluginManager::isPluginEnabled(const std::string &name) const
{
    return isPluginEnabled(getPlugin(name));
}

bool EndstonePluginManager::isPluginEnabled(const Nullable<Plugin> &plugin) const
{
    if (!plugin) {
        return false;
    }

    const auto it = std::ranges::find_if(plugins_, [&plugin](const auto &p) { return p == plugin; });
    return it != plugins_.end() && plugin->isEnabled();
}

PluginLoader *EndstonePluginManager::resolvePluginLoader(const std::string &file) const
{
    for (const auto &loader : plugin_loaders_) {
        for (const auto &pattern : loader->getPluginFileFilters()) {
            if (std::regex r(pattern); std::regex_search(file, r)) {
                return loader.get();
            }
        }
    }
    return nullptr;
}

void EndstonePluginManager::initPlugin(Plugin &plugin, PluginLoader &loader, const std::filesystem::path &base_folder)
{
    auto plugin_name = plugin.getDescription().getName();
    auto prefix = plugin.getDescription().getPrefix();
    if (prefix.empty()) {
        prefix = toCamelCase(plugin_name);
    }

    plugin.loader_ = &loader;
    plugin.server_ = &server_;
    plugin.logger_ = &LoggerFactory::getLogger(prefix);
    plugin.data_folder_ = base_folder / plugin_name;
}

Nullable<Plugin> EndstonePluginManager::loadPlugin(std::string file)
{
    auto *loader = resolvePluginLoader(file);
    if (!loader) {
        return nullptr;
    }
    const auto plugin = loader->loadPlugin(file);
    if (!plugin) {
        return nullptr;
    }
    initPlugin(*plugin, *loader, fs::path(file).parent_path());  // dependency injection
    return loadPlugin(NotNull<Plugin>(plugin));
}

Nullable<Plugin> EndstonePluginManager::loadPlugin(const NotNull<Plugin> &plugin)
{
    const auto &description = plugin->getDescription();
    auto plugin_name = description.getName();
    const static std::regex valid_name{"^[a-z0-9_]+$"};
    if (!std::regex_match(plugin_name, valid_name)) {
        server_.getLogger().error("Could not load plugin '{}': Plugin name contains invalid characters.", plugin_name);
        server_.getLogger().error(
            "A valid plugin name should only contain lowercase letters, numbers and underscores.");
        return nullptr;
    }

    if (plugin_name.starts_with("endstone")) {
        server_.getLogger().error("Could not load plugin '{}': Plugin name must not start with 'endstone'.",
                                  plugin_name);
        return nullptr;
    }

    if (boost::iequals(plugin_name, "endstone") || boost::iequals(plugin_name, "minecraft") ||
        boost::iequals(plugin_name, "mojang")) {
        server_.getLogger().error("Could not load plugin '{}': Restricted name.", plugin_name);
        return nullptr;
    }

    // Check for duplicate names.
    if (lookup_names_.contains(plugin_name)) {
        server_.getLogger().error("Could not load plugin '{}': Another plugin with the same name has been loaded.",
                                  plugin_name);
        return nullptr;
    }

    for (const auto &depend : description.getDepend()) {
        if (const auto dependency = server_.getPluginManager().getPlugin(depend); !dependency) {
            server_.getLogger().error("Could not load plugin '{}': Unknown dependency '{}'. Please download and "
                                      "install it to run this plugin.",
                                      plugin_name, depend);
            return nullptr;
        }
    }

    plugin->getLogger().info("Loading {}", plugin->getDescription().getFullName());
    try {
        plugin->onLoad();
    }
    catch (std::exception &e) {
        plugin->getLogger().error("Error occurred when loading {}", plugin->getDescription().getFullName());
        plugin->getLogger().error(e.what());
        return nullptr;
    }

    // All checks passed, add to the plugin list and lookup map
    plugins_.push_back(plugin);
    lookup_names_.insert_or_assign(plugin->getDescription().getName(), plugin);
    for (const auto &provide : plugin->getDescription().getProvides()) {
        lookup_names_.emplace(provide, plugin);
    }
    return plugin;
}

std::vector<NotNull<Plugin>> EndstonePluginManager::loadPlugins(std::string directory)
{
    // Create a copy of the current plugin loaders.
    // This is necessary because some plugins might register new loaders during their `onLoad` phase,
    // which could modify the `plugin_loaders_` vector and invalidate the iterator.
    std::vector<PluginLoader *> loaders;
    for (const auto &plugin_loader : plugin_loaders_) {
        loaders.emplace_back(plugin_loader.get());
    }

    std::vector<NotNull<Plugin>> plugins;
    for (const auto &loader : loaders) {
        for (const auto &plugin : loader->loadPlugins(directory)) {
            initPlugin(*plugin, *loader, directory);  // dependency injection
            plugins.push_back(plugin);
        }
    }
    return loadPlugins(plugins);
}

std::vector<NotNull<Plugin>> EndstonePluginManager::loadPlugins(std::vector<std::string> files)
{
    std::vector<NotNull<Plugin>> plugins;
    for (const auto &file : files) {
        auto *loader = resolvePluginLoader(file);
        if (!loader) {
            continue;
        }
        const auto plugin = loader->loadPlugin(file);
        if (!plugin) {
            continue;
        }
        initPlugin(*plugin, *loader, fs::path(file).parent_path());  // dependency injection
        plugins.emplace_back(plugin);
    }
    return loadPlugins(plugins);
}

std::vector<NotNull<Plugin>> EndstonePluginManager::loadPlugins(const std::vector<NotNull<Plugin>> &candidates)
{
    std::vector<NotNull<Plugin>> result;
    std::unordered_map<std::string, NotNull<Plugin>> plugins;
    std::vector<std::string> loaded_plugins;
    std::unordered_map<std::string, std::string> plugins_provided;
    std::unordered_map<std::string, std::vector<std::string>> dependencies;
    std::unordered_map<std::string, std::vector<std::string>> soft_dependencies;

    for (const auto &candidate : candidates) {
        const auto &description = candidate->getDescription();

        // Check if two plugins have the same name
        if (auto it = plugins.find(description.getName()); it != plugins.end()) {
            server_.getLogger().error("Ambiguous plugin name '{}', which is the name of another plugin.",
                                      description.getName());
        }
        plugins.insert_or_assign(description.getName(), candidate);

        // Check if the plugin's name is one of the provides of another plugin
        if (auto it = plugins_provided.find(description.getName()); it != plugins_provided.end()) {
            server_.getLogger().warning("Ambiguous plugin name '{}'. It is also provided by '{}'.",
                                        description.getName(), it->second);
            plugins_provided.erase(it);
        }

        for (const auto &provided : description.getProvides()) {
            // Check if any of the provides is the name of another plugin
            if (auto it = plugins.find(provided); it != plugins.end()) {
                server_.getLogger().warning("Plugin '{}' provides '{}', which is the name of another plugin.",
                                            description.getName(), provided);
                continue;
            }
            // Check if more than one plugin provides the same name
            if (auto it = plugins_provided.find(provided); it != plugins_provided.end()) {
                server_.getLogger().warning("'{}' is provided by both '{}' and '{}'.", provided, description.getName(),
                                            it->second);
            }
            plugins_provided[provided] = description.getName();
        }

        // Register soft dependencies
        if (auto soft_deps = description.getSoftDepend(); !soft_deps.empty()) {
            if (auto it = soft_dependencies.find(description.getName()); it != soft_dependencies.end()) {
                it->second.insert(it->second.end(), soft_deps.begin(), soft_deps.end());
            }
            else {
                soft_dependencies[description.getName()] = soft_deps;
            }
            for (const auto &depend : soft_deps) {
                // dependency_graph.add_edge(description.getName(), depend);
            }
        }

        // Register hard dependencies
        if (auto deps = description.getDepend(); !deps.empty()) {
            if (auto it = dependencies.find(description.getName()); it != dependencies.end()) {
                it->second.insert(it->second.end(), deps.begin(), deps.end());
            }
            else {
                dependencies[description.getName()] = deps;
            }
            for (const auto &depend : deps) {
                // dependency_graph.add_edge(description.getName(), depend);
            }
        }

        // Process LoadBefore by adding this plugin as a soft dependency for each target
        for (const auto &load_before : description.getLoadBefore()) {
            if (auto it = soft_dependencies.find(load_before); it != soft_dependencies.end()) {
                it->second.emplace_back(load_before);
            }
            else {
                soft_dependencies[load_before] = {description.getName()};
            }
            // dependency_graph.add_edge(load_before, description.getName());
        }
    }

    // Resolve dependency and load plugins
    while (!plugins.empty()) {
        bool missing_dependency = true;

        // Iterate over the current plugins.
        for (auto it = plugins.begin(); it != plugins.end();) {
            const auto &plugin_name = it->first;
            const auto plugin = it->second;

            // Check hard dependencies: remove those already loaded.
            bool unknown_dependency = false;
            if (dependencies.contains(plugin_name)) {
                auto &deps = dependencies[plugin_name];
                for (auto dep_it = deps.begin(); dep_it != deps.end();) {
                    const auto &dependency = *dep_it;
                    if (std::ranges::find(loaded_plugins, dependency) != loaded_plugins.end()) {
                        dep_it = deps.erase(dep_it);
                    }
                    else if (!plugins.contains(dependency) && !plugins_provided.contains(dependency)) {
                        unknown_dependency = true;
                        server_.getLogger().error(
                            "Could not load plugin '{}': Unknown dependency '{}'. Please download "
                            "and install it to run this plugin.",
                            plugin_name, dependency);
                        break;
                    }
                    else {
                        ++dep_it;  // check for next deps
                    }
                }
                if (deps.empty()) {
                    dependencies.erase(plugin_name);
                }
            }
            if (unknown_dependency) {
                soft_dependencies.erase(plugin_name);
                dependencies.erase(plugin_name);
                it = plugins.erase(it);
                continue;
            }

            // Check soft dependencies
            if (soft_dependencies.contains(plugin_name)) {
                auto &soft_deps = soft_dependencies[plugin_name];
                for (auto dep_it = soft_deps.begin(); dep_it != soft_deps.end();) {
                    const auto &soft_dependency = *dep_it;
                    if (!plugins.contains(soft_dependency) && !plugins_provided.contains(soft_dependency)) {
                        // soft depend is no longer around
                        dep_it = soft_deps.erase(dep_it);
                    }
                    else {
                        ++dep_it;  // check for next deps
                    }
                }
                if (soft_deps.empty()) {
                    soft_dependencies.erase(plugin_name);
                }
            }

            if (!dependencies.contains(plugin_name) && !soft_dependencies.contains(plugin_name) &&
                plugins.contains(plugin_name)) {
                missing_dependency = false;
                // we are clear to load!
                if (const auto loaded_plugin = loadPlugin(plugin)) {
                    result.push_back(loaded_plugin);
                    loaded_plugins.push_back(plugin_name);
                    auto provides = loaded_plugin->getDescription().getProvides();
                    loaded_plugins.insert(loaded_plugins.end(), provides.begin(), provides.end());
                }
                it = plugins.erase(it);
            }
            else {
                ++it;
            }
        }

        if (missing_dependency) {
            // Try a second pass: load plugins that have no hard dependencies (ignoring soft dependencies).
            for (auto it = plugins.begin(); it != plugins.end();) {
                const auto &plugin_name = it->first;
                const auto plugin = it->second;

                if (!dependencies.contains(plugin_name)) {
                    soft_dependencies.erase(plugin_name);
                    missing_dependency = false;

                    if (const auto loaded_plugin = loadPlugin(plugin)) {
                        result.push_back(loaded_plugin);
                        loaded_plugins.push_back(plugin_name);
                        auto provides = loaded_plugin->getDescription().getProvides();
                        loaded_plugins.insert(loaded_plugins.end(), provides.begin(), provides.end());
                    }
                    it = plugins.erase(it);
                    break;
                }
                ++it;
            }

            // If there are still plugins left, a circular dependency is assumed.
            if (missing_dependency) {
                soft_dependencies.clear();
                dependencies.clear();
                for (const auto &plugin : plugins) {
                    server_.getLogger().error("Could not load '{}': circular dependency detected.", plugin.first);
                }
                plugins.clear();
            }
        }
    }

    return result;
}

void EndstonePluginManager::enablePlugin(const NotNull<Plugin> &plugin) const
{
    if (plugin->isEnabled()) {
        return;
    }
    auto &command_map = server_.getCommandMap();
    for (const auto &command : plugin->getDescription().getCommands()) {
        if (const auto existing = command_map.getCommand(command.getName());
            existing && existing->getName() == command.getName() && existing->is<PluginCommand>() &&
            existing.cast<PluginCommand>()->getPlugin() == plugin) {
            continue;
        }
        command_map.registerCommand<PluginCommand>(command, plugin);
    }
    plugin->getPluginLoader().enablePlugin(plugin);
}

void EndstonePluginManager::enablePlugins() const
{
    for (const auto &plugin : plugins_) {
        enablePlugin(plugin);
    }
}

void EndstonePluginManager::disablePlugin(const NotNull<Plugin> &plugin)
{
    if (plugin->isEnabled()) {
        plugin->getPluginLoader().disablePlugin(plugin);
        server_.getScheduler().cancelTasks(plugin);
        server_.getServiceManager().unregisterAll(plugin);
        std::vector<NotNull<Dimension>> dimensions;
        try {
            dimensions = server_.getLevel().getDimensions();
        }
        catch (const std::runtime_error &) {
        }
        for (const auto &dimension : dimensions) {
            dimension->removePluginChunkTickets(plugin);
        }
        std::vector<NotNull<EventHandler>> removed;
        {
            std::shared_lock lock(event_handlers_mtx_);
            for (auto &[name, handler] : event_handlers_) {
                auto handlers = handler.unregister(plugin);
                removed.insert(removed.end(), handlers.begin(), handlers.end());
            }
        }
    }
}

void EndstonePluginManager::disablePlugins()
{
    for (auto it = plugins_.rbegin(); it != plugins_.rend(); ++it) {
        disablePlugin(*it);
    }
}

void EndstonePluginManager::clearPlugins()
{
    disablePlugins();
    lookup_names_.clear();
    // TODO: recreate dependency graph
    {
        decltype(event_handlers_) handlers;
        {
            std::unique_lock lock(event_handlers_mtx_);
            handlers.swap(event_handlers_);
        }
    }

    permissions_.clear();
    default_perms_[PermissionLevel::Default].clear();
    default_perms_[PermissionLevel::Operator].clear();
    default_perms_[PermissionLevel::Console].clear();

    while (!plugin_loaders_.empty()) {
        plugin_loaders_.pop_back();
    }
    while (!plugins_.empty()) {
        plugins_.pop_back();
    }
}

void EndstonePluginManager::callEvent(const NotNull<Event> &event)
{
    if (event->isAsynchronous() && server_.isPrimaryThread()) {
        server_.getLogger().error("{} cannot be triggered asynchronously from server thread.", event->getEventName());
        return;
    }

    if (!event->isAsynchronous() && !server_.isPrimaryThread()) {
        server_.getLogger().error("{} must be triggered synchronously from server thread.", event->getEventName());
        return;
    }

    const auto name = event->getEventName();
    std::vector<NotNull<Plugin>> owners;
    std::vector<NotNull<EventHandler>> handlers;
    {
        std::shared_lock lock(event_handlers_mtx_);
        const auto it = event_handlers_.find(name);
        if (it == event_handlers_.end()) {
            return;
        }
        handlers = it->second.getHandlers();
        for (const auto &handler : handlers) {
            if (auto owner = handler->getPlugin()) {
                owners.emplace_back(std::move(owner));
            }
        }
    }

    for (const auto &handler : handlers) {
        const auto plugin = handler->getPlugin();
        if (!plugin || !plugin->isEnabled()) {
            continue;
        }

        try {
            handler->callEvent(event);
        }
        catch (std::exception &e) {
            server_.getLogger().error("Could not pass event {} to plugin {}. {}", event->getEventName(),
                                      plugin->getDescription().getFullName(), e.what());
        }
    }
}

void EndstonePluginManager::registerEvent(std::string event, std::function<void(const NotNull<Event> &)> executor,
                                          EventPriority priority, const NotNull<Plugin> &plugin, bool ignore_cancelled)
{
    if (!plugin->isEnabled()) {
        server_.getLogger().error("Plugin {} attempted to register listener for event {} while not enabled.",
                                  plugin->getDescription().getFullName(), event);
        return;
    }

    const NotNull<EventHandler> handler =
        std::make_shared<EventHandler>(event, executor, priority, plugin, ignore_cancelled);
    Nullable<EventHandler> registered;
    {
        std::unique_lock lock(event_handlers_mtx_);
        registered = event_handlers_.emplace(event, event).first->second.registerHandler(handler);
    }
    if (!registered) {
        server_.getLogger().error("Plugin {} failed to register listener for event {}: Handler type mismatch",
                                  plugin->getDescription().getFullName(), event);
        return;
    }
}

bool EndstonePluginManager::isEventRegistered(const std::string_view event) const
{
    std::shared_lock lock(event_handlers_mtx_);
    const auto it = event_handlers_.find(event);
    return it != event_handlers_.end() && !it->second.empty();
}

Nullable<Permission> EndstonePluginManager::getPermission(std::string name) const
{
    std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
    const auto it = permissions_.find(name);
    if (it == permissions_.end()) {
        return nullptr;
    }
    return it->second;
}

NotNull<Permission> EndstonePluginManager::addPermission(NotNull<Permission> perm)
{
    auto name = perm->getName();
    std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
    if (permissions_.contains(name)) {
        throw std::runtime_error("The permission '" + name + "' is already defined.");
    }

    perm->init(*this);
    const auto it = permissions_.emplace(name, std::move(perm)).first;
    calculatePermissionDefault(it->second);
    return it->second;
}

void EndstonePluginManager::removePermission(const NotNull<Permission> &perm)
{
    removePermission(perm->getName());
}

void EndstonePluginManager::removePermission(std::string name)
{
    std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
    permissions_.erase(name);
}

std::vector<NotNull<Permission>> EndstonePluginManager::getDefaultPermissions(PermissionLevel level) const
{
    std::vector<NotNull<Permission>> permissions;
    for (const auto &perm : default_perms_.at(level).get<0>()) {
        permissions.emplace_back(perm);
    }
    return permissions;
}

void EndstonePluginManager::recalculatePermissionDefaults(const NotNull<Permission> &perm)
{
    if (getPermission(perm->getName())) {
        default_perms_.at(PermissionLevel::Default).get<1>().erase(perm);
        default_perms_.at(PermissionLevel::Operator).get<1>().erase(perm);
        default_perms_.at(PermissionLevel::Console).get<1>().erase(perm);
        calculatePermissionDefault(perm);
    }
}

void EndstonePluginManager::calculatePermissionDefault(const NotNull<Permission> &perm)
{
    switch (perm->getDefault()) {
    case PermissionDefault::Console:
        default_perms_.at(PermissionLevel::Console).emplace_back(perm);
        dirtyPermissibles(PermissionLevel::Console);
        break;
    case PermissionDefault::Operator:
        default_perms_.at(PermissionLevel::Operator).emplace_back(perm);
        default_perms_.at(PermissionLevel::Console).emplace_back(perm);
        dirtyPermissibles(PermissionLevel::Operator);
        dirtyPermissibles(PermissionLevel::Console);
        break;
    case PermissionDefault::NotOperator:
        default_perms_.at(PermissionLevel::Default).emplace_back(perm);
        dirtyPermissibles(PermissionLevel::Default);
        break;
    case PermissionDefault::True:
        default_perms_.at(PermissionLevel::Default).emplace_back(perm);
        default_perms_.at(PermissionLevel::Operator).emplace_back(perm);
        default_perms_.at(PermissionLevel::Console).emplace_back(perm);
        dirtyPermissibles(PermissionLevel::Default);
        dirtyPermissibles(PermissionLevel::Operator);
        dirtyPermissibles(PermissionLevel::Console);
        break;
    case PermissionDefault::False:
    default:
        break;
    }
}

void EndstonePluginManager::dirtyPermissibles(PermissionLevel level) const
{
    const auto permissibles = getDefaultPermSubscriptions(level);
    for (const auto &permissible : permissibles) {
        permissible->recalculatePermissions();
    }
}

void EndstonePluginManager::subscribeToPermission(std::string permission, const NotNull<Permissible> &permissible)
{
    auto &name = permission;
    std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
    std::lock_guard lock(subs_mtx_);
    perm_subs_[name].insert(permissible.get());
}

void EndstonePluginManager::unsubscribeFromPermission(std::string permission, const NotNull<Permissible> &permissible)
{
    auto &name = permission;
    std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
    std::lock_guard lock(subs_mtx_);
    if (const auto it = perm_subs_.find(name); it != perm_subs_.end()) {
        it->second.erase(permissible.get());
        if (it->second.empty()) {
            perm_subs_.erase(it);
        }
    }
}

std::unordered_set<NotNull<Permissible>> EndstonePluginManager::getPermissionSubscriptions(std::string permission) const
{
    auto &name = permission;
    std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
    std::unordered_set<NotNull<Permissible>> subs;
    {
        std::lock_guard lock(subs_mtx_);
        if (const auto it = perm_subs_.find(name); it != perm_subs_.end()) {
            subs = lockAndPrune(it->second);
            if (it->second.empty()) {
                perm_subs_.erase(it);
            }
        }
    }
    return subs;
}

void EndstonePluginManager::subscribeToDefaultPerms(PermissionLevel level, const NotNull<Permissible> &permissible)
{
    std::lock_guard lock(subs_mtx_);
    def_subs_[level].insert(permissible.get());
}

void EndstonePluginManager::unsubscribeFromDefaultPerms(PermissionLevel level, const NotNull<Permissible> &permissible)
{
    std::lock_guard lock(subs_mtx_);
    if (const auto it = def_subs_.find(level); it != def_subs_.end()) {
        it->second.erase(permissible.get());
        if (it->second.empty()) {
            def_subs_.erase(it);
        }
    }
}

std::unordered_set<NotNull<Permissible>> EndstonePluginManager::getDefaultPermSubscriptions(PermissionLevel level) const
{
    std::unordered_set<NotNull<Permissible>> subs;
    {
        std::lock_guard lock(subs_mtx_);
        if (const auto it = def_subs_.find(level); it != def_subs_.end()) {
            subs = lockAndPrune(it->second);
            if (it->second.empty()) {
                def_subs_.erase(it);
            }
        }
    }
    return subs;
}

std::unordered_set<NotNull<Permission>> EndstonePluginManager::getPermissions() const
{
    std::unordered_set<NotNull<Permission>> perms;
    for (const auto &entry : permissions_) {
        perms.insert(entry.second);
    }
    return perms;
}

}  // namespace endstone::core
