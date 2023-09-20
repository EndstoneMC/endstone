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
#include <mutex>
#include <optional>
#include <regex>
#include <string>
#include <vector>

#include "endstone/command/command.h"
#include "endstone/permission/permission.h"
#include "endstone/permission/permission_default.h"

class PluginDescription {
public:
    PluginDescription(const std::string &name, const std::string &version) noexcept
    {
        name_ = name;
        std::replace(name_.begin(), name_.end(), ' ', '_');
        version_ = version;
        full_name_ = name_ + " v" + version;
    }

    virtual ~PluginDescription() noexcept = default;
    // Delete the copy constructor and copy assignment operator
    PluginDescription(const PluginDescription &) = delete;
    PluginDescription &operator=(const PluginDescription &) = delete;

    [[nodiscard]] const std::string &getName() const noexcept
    {
        return name_;
    }

    [[nodiscard]] const std::string &getVersion() const noexcept
    {
        return version_;
    }

    [[nodiscard]] const std::string &getFullName() const noexcept
    {
        return full_name_;
    }

    [[nodiscard]] virtual std::optional<std::string> getDescription() const noexcept
    {
        return std::nullopt;
    }

    [[nodiscard]] virtual std::optional<std::vector<std::string>> getAuthors() const noexcept
    {
        return std::nullopt;
    }

    [[nodiscard]] virtual std::optional<std::string> getPrefix() const noexcept
    {
        return std::nullopt;
    }

    [[nodiscard]] virtual std::vector<std::shared_ptr<Command>> getCommands() const noexcept
    {
        return {};
    }

    [[nodiscard]] virtual std::vector<std::shared_ptr<Permission>> getPermissions() const noexcept
    {
        return {};
    }

    [[nodiscard]] virtual std::optional<PermissionDefault> getDefaultPermission() const noexcept
    {
        return std::nullopt;
    }

    inline const static std::regex ValidName{"^[A-Za-z0-9 _.-]+$"};

private:
    std::string name_;
    std::string version_;
    std::string full_name_;
};
