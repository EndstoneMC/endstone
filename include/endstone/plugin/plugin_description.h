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

#include <string>
#include <utility>
#include <vector>

#include <fmt/format.h>

#include "endstone/command/command.h"
#include "endstone/detail/plugin/plugin_description_builder.h"
#include "endstone/endstone.h"
#include "endstone/permissions/permission.h"
#include "endstone/plugin/plugin_load_order.h"

namespace endstone {

class PluginDescription {
public:
    PluginDescription(std::string name, std::string version, std::string description = "",
                      PluginLoadOrder load = PluginLoadOrder::PostWorld, std::vector<std::string> authors = {},
                      std::vector<std::string> contributors = {}, std::string website = "", std::string prefix = "",
                      std::vector<std::string> provides = {}, std::vector<std::string> depend = {},
                      std::vector<std::string> soft_depend = {}, std::vector<std::string> load_before = {},
                      PermissionDefault default_permission = PermissionDefault::Operator,
                      std::vector<Command> commands = {}, std::vector<Permission> permissions = {})
        : description_(std::move(description)), load_(load), authors_(std::move(authors)),
          contributors_(std::move(contributors)), website_(std::move(website)), prefix_(std::move(prefix)),
          provides_(std::move(provides)), depend_(std::move(depend)), soft_depend_(std::move(soft_depend)),
          load_before_(std::move(load_before)), default_permission_(default_permission), commands_(std::move(commands)),
          permissions_(std::move(permissions))
    {
        name_ = std::move(name);
        std::replace(name_.begin(), name_.end(), ' ', '_');
        version_ = std::move(version);
        full_name_ = fmt::format("{} v{}", name_, version_);
        api_version_ = ENDSTONE_API_VERSION;
    }

    /**
     * Gives the name of the plugin. This name is a unique identifier for plugins.
     *
     * @return the name of the plugin
     */
    [[nodiscard]] std::string getName() const
    {
        return name_;
    }

    /**
     * Gives the version of the plugin.
     *
     * @return the version of the plugin
     */
    [[nodiscard]] std::string getVersion() const
    {
        return version_;
    }

    /**
     * Returns the name of a plugin, including the version.
     *
     * @return a descriptive name of the plugin and respective version
     */
    [[nodiscard]] std::string getFullName() const
    {
        return full_name_;
    }

    /**
     * Gives the API version which this plugin is designed to support.
     *
     * @return the API version supported by the plugin
     */
    [[nodiscard]] std::string getAPIVersion() const
    {
        return api_version_;
    }

    /**
     * Gives a human-friendly description of the functionality the plugin provides.
     * @return description of this plugin, or empty if not specified
     */
    [[nodiscard]] std::string getDescription() const
    {
        return description_;
    }

    /**
     * Gives the phase of server startup that the plugin should be loaded.
     *
     * @return the phase when the plugin should be loaded
     */
    [[nodiscard]] PluginLoadOrder getLoad() const
    {
        return load_;
    }

    /**
     * Gives the list of authors for the plugin.
     *
     * @return an immutable list of the plugin's authors
     */
    [[nodiscard]] std::vector<std::string> getAuthors() const
    {
        return authors_;
    }

    /**
     * Gives the list of contributors for the plugin.
     *
     * @return an immutable list of the plugin's contributions
     */
    [[nodiscard]] std::vector<std::string> getContributors() const
    {
        return contributors_;
    }

    /**
     * Gives the plugin's or plugin's author's website.
     *
     * @return the website of this plugin, or empty if not specified
     */
    [[nodiscard]] std::string getWebsite() const
    {
        return website_;
    }

    /**
     * Gives the token to prefix plugin-specific logging messages with.
     *
     * @return the prefixed logging token, or empty if not specified
     */
    [[nodiscard]] std::string getPrefix() const
    {
        return prefix_;
    }

    /**
     * Gives the list of other plugin APIs which this plugin provides. These are usable for other plugins to depend on.
     *
     * @return immutable list of the plugin APIs which this plugin provides
     */
    [[nodiscard]] std::vector<std::string> getProvides() const
    {
        return provides_;
    }

    /**
     * Gives a list of other plugins that the plugin requires.
     *
     * @return immutable list of the plugin's dependencies
     */
    [[nodiscard]] std::vector<std::string> getDepend() const
    {
        return depend_;
    }

    /**
     * Gives a list of other plugins that the plugin requires for full functionality.
     *
     * @return immutable list of the plugin's preferred dependencies
     */
    [[nodiscard]] std::vector<std::string> getSoftDepend() const
    {
        return soft_depend_;
    }

    /**
     * Gets the list of plugins that should consider this plugin a soft-dependency.
     *
     * @return immutable list of plugins that should consider this plugin a soft-dependency
     */
    [[nodiscard]] std::vector<std::string> getLoadBefore() const
    {
        return load_before_;
    }

    /**
     * Gives the default value of permissions registered for the plugin.
     *
     * @return the default value for the plugin's permissions
     */
    [[nodiscard]] PermissionDefault getDefaultPermission() const
    {
        return default_permission_;
    }

    /**
     * Gives the list of commands the plugin will register at runtime.
     *
     * @return the commands this plugin will register
     */
    [[nodiscard]] std::vector<Command> getCommands() const
    {
        return commands_;
    }

    /**
     * Gives the list of permissions the plugin will register at runtime, immediately proceeding enabling.
     *
     * @return the permissions this plugin will register
     */
    [[nodiscard]] std::vector<Permission> getPermissions() const
    {
        return permissions_;
    }

private:
    std::string name_;
    std::string version_;
    std::string full_name_;
    std::string api_version_;
    std::string description_;
    PluginLoadOrder load_ = PluginLoadOrder::PostWorld;
    std::vector<std::string> authors_;
    std::vector<std::string> contributors_;
    std::string website_;
    std::string prefix_;
    std::vector<std::string> provides_;
    std::vector<std::string> depend_;
    std::vector<std::string> soft_depend_;
    std::vector<std::string> load_before_;
    PermissionDefault default_permission_ = PermissionDefault::Operator;
    std::vector<Command> commands_;
    std::vector<Permission> permissions_;
};

}  // namespace endstone
