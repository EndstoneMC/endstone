

# File command.h

[**File List**](files.md) **>** [**command**](dir_5c7b2dbfabcd1115569d1e20a260545c.md) **>** [**command.h**](command_8h.md)

[Go to the documentation of this file](command_8h.md)


```C++
// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <algorithm>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "endstone/command/command_map.h"
#include "endstone/command/command_sender.h"

namespace endstone {

class PluginCommand;

class Command {
public:
    explicit Command(std::string name, std::string description = "", std::vector<std::string> usages = {},
                     std::vector<std::string> aliases = {}, std::vector<std::string> permissions = {})
    {
        setName(std::move(name));
        setDescription(std::move(description));
        setUsages(std::move(usages));
        setAliases(std::move(aliases));
        setPermissions(std::move(permissions));
    }

    virtual ~Command() = default;

    [[nodiscard]] virtual bool execute(CommandSender &sender, const std::vector<std::string> &args) const
    {
        return false;
    }

    [[nodiscard]] std::string getName() const
    {
        return name_;
    }

    void setName(std::string name)
    {
        if (!isRegistered()) {
            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
            name_ = std::move(name);
        }
    }

    [[nodiscard]] std::string getDescription() const
    {
        return description_;
    }

    void setDescription(std::string description)
    {
        if (!isRegistered()) {
            description_ = std::move(description);
        }
    }

    [[nodiscard]] std::vector<std::string> getAliases() const
    {
        return aliases_;
    }

    template <typename... Alias>
    void setAliases(Alias... aliases)
    {
        if (!isRegistered()) {
            std::vector<std::string> all_aliases = {aliases...};
            aliases_.clear();
            for (auto alias : all_aliases) {
                std::transform(alias.begin(), alias.end(), alias.begin(),
                               [](unsigned char c) { return std::tolower(c); });
                aliases_.push_back(alias);
            }
        }
    }

    [[nodiscard]] std::vector<std::string> getUsages() const
    {
        return usages_;
    }

    template <typename... Usage>
    void setUsages(Usage... usages)
    {
        if (!isRegistered()) {
            std::vector<std::string> all_usages = {usages...};
            if (all_usages.empty()) {
                all_usages.push_back("/" + getName());
            }
            usages_ = std::move(all_usages);
        }
    }

    [[nodiscard]] std::vector<std::string> getPermissions() const
    {
        return permissions_;
    }

    template <typename... Permission>
    void setPermissions(Permission... permissions)
    {
        permissions_ = std::move(std::vector<std::string>{permissions...});
    }

    [[nodiscard]] bool testPermission(const CommandSender &target) const
    {
        if (testPermissionSilently(target)) {
            return true;
        }

        target.sendErrorMessage("You do not have permission to perform this command.");
        return false;
    }

    [[nodiscard]] bool testPermissionSilently(const CommandSender &target) const
    {
        if (permissions_.empty()) {
            return true;
        }

        return std::any_of(permissions_.begin(), permissions_.end(),
                           [&target](const auto &p) { return target.hasPermission(p); });
    }

    bool registerTo(CommandMap &command_map)
    {
        if (allowChangesFrom(command_map)) {
            command_map_ = &command_map;
            return true;
        }

        return false;
    }

    bool unregisterFrom(CommandMap &command_map)
    {
        if (allowChangesFrom(command_map)) {
            command_map_ = nullptr;
            return true;
        }

        return false;
    }

    [[nodiscard]] bool isRegistered() const
    {
        return command_map_ != nullptr;
    }

    [[nodiscard]] virtual PluginCommand *asPluginCommand()
    {
        return nullptr;
    }

private:
    bool allowChangesFrom(CommandMap &command_map)
    {
        return (!isRegistered() || command_map_ == &command_map);
    }

    std::string name_;
    std::string description_;
    std::vector<std::string> aliases_;
    std::vector<std::string> usages_;
    std::vector<std::string> permissions_;
    CommandMap *command_map_ = nullptr;
};
}  // namespace endstone
```


