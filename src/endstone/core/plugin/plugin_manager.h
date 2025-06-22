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

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index_container.hpp>

#include "endstone/event/handler_list.h"
#include "endstone/permissions/permission.h"
#include "endstone/permissions/permission_level.h"
#include "endstone/plugin/plugin_loader.h"
#include "endstone/plugin/plugin_manager.h"
#include "endstone/server.h"

namespace endstone::core {

class EndstonePluginManager : public PluginManager {
public:
    explicit EndstonePluginManager(Server &server);

    /** Plugin loading */
    void registerLoader(std::unique_ptr<PluginLoader> loader) override;
    [[nodiscard]] Plugin *getPlugin(const std::string &name) const override;
    [[nodiscard]] std::vector<Plugin *> getPlugins() const override;
    [[nodiscard]] bool isPluginEnabled(const std::string &name) const override;
    bool isPluginEnabled(Plugin *plugin) const override;
    Plugin *loadPlugin(std::string file) override;
    std::vector<Plugin *> loadPlugins(std::string directory) override;
    std::vector<Plugin *> loadPlugins(std::vector<std::string> files) override;
    void enablePlugin(Plugin &plugin) const override;
    void enablePlugins() const override;
    void disablePlugin(Plugin &plugin) override;
    void disablePlugins() override;
    void clearPlugins() override;

    /** Event system */
    void callEvent(Event &event) override;
    void registerEvent(std::string event, std::function<void(Event &)> executor, EventPriority priority, Plugin &plugin,
                       bool ignore_cancelled) override;

    /** Permission system */
    [[nodiscard]] Permission *getPermission(std::string name) const override;
    Permission *addPermission(std::unique_ptr<Permission> perm) override;
    void removePermission(Permission &perm) override;
    void removePermission(std::string name) override;
    [[nodiscard]] std::vector<Permission *> getDefaultPermissions(PermissionLevel level) const override;
    void recalculatePermissionDefaults(Permission &perm) override;
    void subscribeToPermission(std::string permission, Permissible &permissible) override;
    void unsubscribeFromPermission(std::string permission, Permissible &permissible) override;
    [[nodiscard]] std::unordered_set<Permissible *> getPermissionSubscriptions(std::string permission) const override;
    void subscribeToDefaultPerms(PermissionLevel level, Permissible &permissible) override;
    void unsubscribeFromDefaultPerms(PermissionLevel level, Permissible &permissible) override;
    [[nodiscard]] std::unordered_set<Permissible *> getDefaultPermSubscriptions(PermissionLevel level) const override;
    [[nodiscard]] std::unordered_set<Permission *> getPermissions() const override;

private:
    friend class EndstoneServer;

    template <typename T>
    using linked_hash_set = boost::multi_index::multi_index_container<
        T, boost::multi_index::indexed_by<boost::multi_index::sequenced<>,
                                          boost::multi_index::hashed_unique<boost::multi_index::identity<T>>>>;

    Plugin *loadPlugin(Plugin &plugin);
    std::vector<Plugin *> loadPlugins(std::vector<Plugin *>);
    void initPlugin(Plugin &plugin, PluginLoader &loader, const std::filesystem::path &base_folder);
    void calculatePermissionDefault(Permission &perm);
    void dirtyPermissibles(PermissionLevel level) const;
    [[nodiscard]] PluginLoader *resolvePluginLoader(const std::string &file) const;
    Server &server_;
    std::vector<std::unique_ptr<PluginLoader>> plugin_loaders_;
    std::vector<Plugin *> plugins_;
    std::unordered_map<std::string, Plugin *> lookup_names_;
    std::unordered_map<std::string, HandlerList> event_handlers_;
    std::unordered_map<std::string, std::unique_ptr<Permission>> permissions_;
    std::unordered_map<PermissionLevel, linked_hash_set<Permission *>> default_perms_;
    std::unordered_map<std::string, std::unordered_map<Permissible *, bool>> perm_subs_;
    std::unordered_map<PermissionLevel, std::unordered_map<Permissible *, bool>> def_subs_;
};

}  // namespace endstone::core
