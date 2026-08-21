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
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index_container.hpp>

#include "endstone/event/handler_list.h"
#include "endstone/permissions/permission.h"
#include "endstone/permissions/permission_level.h"
#include "endstone/plugin/plugin_manager.h"
#include "endstone/server.h"

namespace endstone {
class PluginLoader;

namespace core {

class EndstonePluginManager : public PluginManager {
public:
    explicit EndstonePluginManager(Server &server);
    ~EndstonePluginManager() override;

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
    [[nodiscard]] bool isEventRegistered(std::string_view event) const;

    template <typename EventType>
    [[nodiscard]] bool isEventRegistered() const
    {
        return isEventRegistered(EventType::NAME);
    }

    /** Permission system */
    [[nodiscard]] Nullable<Permission> getPermission(std::string name) const override;
    NotNull<Permission> addPermission(NotNull<Permission> perm) override;
    void removePermission(const NotNull<Permission> &perm) override;
    void removePermission(std::string name) override;
    [[nodiscard]] std::vector<NotNull<Permission>> getDefaultPermissions(PermissionLevel level) const override;
    void recalculatePermissionDefaults(const NotNull<Permission> &perm) override;
    void subscribeToPermission(std::string permission, const NotNull<Permissible> &permissible) override;
    void unsubscribeFromPermission(std::string permission, const NotNull<Permissible> &permissible) override;
    [[nodiscard]] std::unordered_set<NotNull<Permissible>> getPermissionSubscriptions(
        std::string permission) const override;
    void subscribeToDefaultPerms(PermissionLevel level, const NotNull<Permissible> &permissible) override;
    void unsubscribeFromDefaultPerms(PermissionLevel level, const NotNull<Permissible> &permissible) override;
    [[nodiscard]] std::unordered_set<NotNull<Permissible>> getDefaultPermSubscriptions(
        PermissionLevel level) const override;
    [[nodiscard]] std::unordered_set<NotNull<Permission>> getPermissions() const override;

private:
    friend class EndstoneServer;

    template <typename T>
    using linked_hash_set = boost::multi_index::multi_index_container<
        T, boost::multi_index::indexed_by<
               boost::multi_index::sequenced<>,
               boost::multi_index::hashed_unique<boost::multi_index::identity<T>, std::hash<T>>>>;

    struct StringHash {
        using is_transparent = void;  // NOLINT(readability-identifier-naming)
        [[nodiscard]] std::size_t operator()(std::string_view name) const noexcept
        {
            return std::hash<std::string_view>{}(name);
        }
    };

    Plugin *loadPlugin(Plugin &plugin);
    std::vector<Plugin *> loadPlugins(std::vector<Plugin *>);
    void initPlugin(Plugin &plugin, PluginLoader &loader, const std::filesystem::path &base_folder);
    using PermissibleSubscriptions = std::set<std::weak_ptr<Permissible>, std::owner_less<std::weak_ptr<Permissible>>>;
    void calculatePermissionDefault(const NotNull<Permission> &perm);
    void dirtyPermissibles(PermissionLevel level) const;
    [[nodiscard]] PluginLoader *resolvePluginLoader(const std::string &file) const;
    Server &server_;
    std::vector<std::unique_ptr<PluginLoader>> plugin_loaders_;
    std::vector<Plugin *> plugins_;
    std::unordered_map<std::string, Plugin *> lookup_names_;
    std::unordered_map<std::string, HandlerList, StringHash, std::equal_to<>> event_handlers_;
    std::unordered_map<std::string, NotNull<Permission>> permissions_;
    std::unordered_map<PermissionLevel, linked_hash_set<NotNull<Permission>>> default_perms_;
    mutable std::unordered_map<std::string, PermissibleSubscriptions> perm_subs_;
    mutable std::unordered_map<PermissionLevel, PermissibleSubscriptions> def_subs_;
};

}  // namespace core
}  // namespace endstone
