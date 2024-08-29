

# File plugin\_description.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**plugin**](dir_53ee43673b2467e53c4cb8c30a2e7d89.md) **>** [**plugin\_description.h**](plugin__description_8h.md)

[Go to the documentation of this file](plugin__description_8h.md)


```C++
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

    [[nodiscard]] std::string getName() const
    {
        return name_;
    }

    [[nodiscard]] std::string getVersion() const
    {
        return version_;
    }

    [[nodiscard]] std::string getFullName() const
    {
        return full_name_;
    }

    [[nodiscard]] std::string getAPIVersion() const
    {
        return api_version_;
    }

    [[nodiscard]] std::string getDescription() const
    {
        return description_;
    }

    [[nodiscard]] PluginLoadOrder getLoad() const
    {
        return load_;
    }

    [[nodiscard]] std::vector<std::string> getAuthors() const
    {
        return authors_;
    }

    [[nodiscard]] std::vector<std::string> getContributors() const
    {
        return contributors_;
    }

    [[nodiscard]] std::string getWebsite() const
    {
        return website_;
    }

    [[nodiscard]] std::string getPrefix() const
    {
        return prefix_;
    }

    [[nodiscard]] std::vector<std::string> getProvides() const
    {
        return provides_;
    }

    [[nodiscard]] std::vector<std::string> getDepend() const
    {
        return depend_;
    }

    [[nodiscard]] std::vector<std::string> getSoftDepend() const
    {
        return soft_depend_;
    }

    [[nodiscard]] std::vector<std::string> getLoadBefore() const
    {
        return load_before_;
    }

    [[nodiscard]] PermissionDefault getDefaultPermission() const
    {
        return default_permission_;
    }

    [[nodiscard]] std::vector<Command> getCommands() const
    {
        return commands_;
    }

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
```


