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
#include <vector>

#include <fmt/format.h>

#include "endstone/command/command.h"
#include "endstone/detail/plugin/plugin_metadata.h"
#include "endstone/endstone.h"
#include "endstone/permissions/permission.h"
#include "endstone/plugin/plugin_load_order.h"

namespace endstone {

class PluginDescription {
public:
    PluginDescription(std::string name, std::string version, const detail::PluginMetadata &metadata = {})
    {
        name_ = std::move(name);
        std::replace(name_.begin(), name_.end(), ' ', '_');
        version_ = std::move(version);
        full_name_ = fmt::format("{} v{}", name_, version_);
        api_version_ = ENDSTONE_API_VERSION;
        metadata_ = metadata;
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
        return metadata_.description;
    }

    /**
     * Gives the phase of server startup that the plugin should be loaded.
     *
     * @return the phase when the plugin should be loaded
     */
    [[nodiscard]] PluginLoadOrder getLoad() const
    {
        return metadata_.load;
    }

    /**
     * Gives the list of authors for the plugin.
     *
     * @return an immutable list of the plugin's authors
     */
    [[nodiscard]] std::vector<std::string> getAuthors() const
    {
        return metadata_.authors;
    }

    /**
     * Gives the list of contributors for the plugin.
     *
     * @return an immutable list of the plugin's contributions
     */
    [[nodiscard]] std::vector<std::string> getContributors() const
    {
        return metadata_.contributors;
    }

    /**
     * Gives the plugin's or plugin's author's website.
     *
     * @return the website of this plugin, or empty if not specified
     */
    [[nodiscard]] std::string getWebsite() const
    {
        return metadata_.website;
    }

    /**
     * Gives the token to prefix plugin-specific logging messages with.
     *
     * @return the prefixed logging token, or empty if not specified
     */
    [[nodiscard]] std::string getPrefix() const
    {
        return metadata_.prefix;
    }

    /**
     * Gives the list of other plugin APIs which this plugin provides. These are usable for other plugins to depend on.
     *
     * @return immutable list of the plugin APIs which this plugin provides
     */
    [[nodiscard]] std::vector<std::string> getProvides() const
    {
        return metadata_.provides;
    }

    /**
     * Gives a list of other plugins that the plugin requires.
     *
     * @return immutable list of the plugin's dependencies
     */
    [[nodiscard]] std::vector<std::string> getDepend() const
    {
        return metadata_.depend;
    }

    /**
     * Gives a list of other plugins that the plugin requires for full functionality.
     *
     * @return immutable list of the plugin's preferred dependencies
     */
    [[nodiscard]] std::vector<std::string> getSoftDepend() const
    {
        return metadata_.soft_depend;
    }

    /**
     * Gets the list of plugins that should consider this plugin a soft-dependency.
     *
     * @return immutable list of plugins that should consider this plugin a soft-dependency
     */
    [[nodiscard]] std::vector<std::string> getLoadBefore() const
    {
        return metadata_.load_before;
    }

    /**
     * Gives the default value of permissions registered for the plugin.
     *
     * @return the default value for the plugin's permissions
     */
    [[nodiscard]] PermissionDefault getDefaultPermission() const
    {
        return metadata_.default_permission;
    }

    /**
     * Gives the list of commands the plugin will register at runtime, immediately proceeding enabling.
     *
     * @return the commands this plugin will register
     */
    [[nodiscard]] std::vector<Command> getCommands() const
    {
        return metadata_.getCommands();
    }

    /**
     * Gives the list of permissions the plugin will register at runtime, immediately proceeding enabling.
     *
     * @return the permissions this plugin will register
     */
    [[nodiscard]] std::vector<Permission> getPermissions() const
    {
        return metadata_.getPermissions();
    }

private:
    std::string name_;
    std::string version_;
    std::string full_name_;
    std::string api_version_;
    detail::PluginMetadata metadata_;
};

}  // namespace endstone
