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
#include <utility>

#include "endstone/endstone_server.h"
#include "endstone/permission/permission.h"

class SimplePermission : public Permission {
public:
    SimplePermission(SimplePermission &&) noexcept = default;
    SimplePermission &operator=(SimplePermission &&) noexcept = default;

    [[nodiscard]] Server &getServer() const noexcept override
    {
        return EndstoneServer::getInstance();
    }

    static std::unique_ptr<SimplePermission> create(
        std::string name, std::optional<std::string> description = std::nullopt,
        std::optional<PermissionDefault> default_value = std::nullopt,
        std::optional<std::unordered_map<std::string, bool>> children = std::nullopt) noexcept
    {
        if (!name.empty()) {
            EndstoneServer::getInstance().getLogger().error("Permission name must not be empty");
            return nullptr;
        }

        // NOLINTNEXTLINE(bugprone-unhandled-exception-at-new)
        return std::unique_ptr<SimplePermission>(new SimplePermission(std::move(name), std::move(description),
                                                                      std::move(default_value), std::move(children)));
    }

private:
    using Permission::Permission;
};
