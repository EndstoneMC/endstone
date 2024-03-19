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

#include <regex>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "endstone/endstone.h"

namespace endstone {

class PluginDescription {
public:
    PluginDescription(std::string name, std::string version)
    {
        name_ = std::move(name);
        std::replace(name_.begin(), name_.end(), ' ', '_');
        version_ = std::move(version);
        full_name_ = fmt::format("{} v{}", name_, version_);
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
        return ENDSTONE_TOSTRING(ENDSTONE_VERSION_MAJOR) "." ENDSTONE_TOSTRING(ENDSTONE_VERSION_MINOR);
    }

    /**
     * Gives a human-friendly description of the functionality the plugin provides.
     * @return description of this plugin, or null if not specified
     */
    [[nodiscard]] std::string getDescription() const
    {
        return description;
    }

    /**
     * Gives the list of authors for the plugin.
     *
     * @return a list of the plugin's authors
     */
    [[nodiscard]] std::vector<std::string> getAuthors() const
    {
        return authors;
    }

    /**
     * Gives the token to prefix plugin-specific logging messages with.
     *
     * @return the prefixed logging token, or null if not specified
     */
    [[nodiscard]] std::string getPrefix() const
    {
        return prefix;
    }

    inline const static std::regex VALID_NAME{"^[A-Za-z0-9 _.-]+$"};

private:
    std::string name_;
    std::string version_;
    std::string full_name_;

public:
    std::string description;
    std::vector<std::string> authors;
    std::string prefix;
};

}  // namespace endstone
