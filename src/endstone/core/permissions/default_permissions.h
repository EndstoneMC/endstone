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

#include <string>
#include <string_view>

#include "endstone/core/server.h"
#include "endstone/permissions/permission.h"

namespace endstone::core {
class DefaultPermissions {
    constexpr static std::string_view ROOT = "endstone";

public:
    static Permission &registerPermission(std::unique_ptr<Permission> perm);
    static Permission &registerPermission(std::unique_ptr<Permission> perm, Permission &parent);
    static Permission &registerPermission(std::string_view name, std::string_view desc);
    static Permission &registerPermission(std::string_view name, std::string_view desc, Permission &parent);
    static Permission &registerPermission(std::string_view name, std::string_view desc, PermissionDefault def,
                                          Permission &parent);

    static void registerCorePermissions();
};

class CommandPermissions {
    constexpr static std::string_view ROOT = "endstone.command";

public:
    static Permission &registerPermission(std::string_view name, std::string_view cmd, std::string_view desc,
                                          Permission &parent);
    static Permission &registerPermission(std::string_view name, std::string_view cmd, std::string_view desc,
                                          PermissionDefault def, Permission &parent);
    static Permission &registerPermissions(Permission &parent);
};

class BroadcastPermissions {
    constexpr static std::string_view ROOT = "endstone.broadcast";

public:
    static Permission &registerPermissions(Permission &parent);
};

class MinecraftDefaultPermissions {
    constexpr static std::string_view ROOT = "minecraft";

public:
    static void registerCorePermissions();
};

class MinecraftCommandPermissions {
    constexpr static std::string_view ROOT = "minecraft.command";

public:
    static Permission &registerPermissions(Permission &parent);
};
}  // namespace endstone::core
