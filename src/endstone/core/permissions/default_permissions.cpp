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

#include "endstone/core/permissions/default_permissions.h"

#include <entt/entt.hpp>

namespace endstone::core {

Permission *DefaultPermissions::registerPermission(std::unique_ptr<Permission> perm, Permission *parent)
{
    const auto &server = entt::locator<EndstoneServer>::value();
    auto *result = server.getPluginManager().getPermission(perm->getName());
    if (result == nullptr) {
        result = server.getPluginManager().addPermission(std::move(perm));
    }
    if (parent != nullptr && result != nullptr) {
        parent->getChildren()[result->getName()] = true;
    }
    return result;
}

Permission *DefaultPermissions::registerPermission(const std::string &name, Permission *parent, const std::string &desc,
                                                   PermissionDefault default_value,
                                                   const std::unordered_map<std::string, bool> &children)
{
    return registerPermission(std::make_unique<Permission>(name, desc, default_value, children), parent);
}

void DefaultPermissions::registerCorePermissions()
{
    auto *root =
        registerPermission("endstone", nullptr, "Gives the user the ability to use all Endstone utilities and commands",
                           PermissionDefault::Console);
    registerCommandPermissions(root);
    registerBroadcastPermissions(root);
    root->recalculatePermissibles();
}

void DefaultPermissions::registerMinecraftPermissions()
{
    auto *root =
        registerPermission("minecraft", nullptr, "Gives the user the ability to use all vanilla utilities and commands",
                           PermissionDefault::Console);
    auto *parent = registerPermission(root->getName() + ".command", root,
                                      "Gives the user the ability to use all vanilla minecraft commands",
                                      PermissionDefault::Console);
    registerPermission(parent->getName() + ".help", parent, "Allows the user to access Vanilla command help.",
                       PermissionDefault::True);
    registerPermission(parent->getName() + ".list", parent, "Allows the user to list all online players.",
                       PermissionDefault::True);
    registerPermission(parent->getName() + ".me", parent, "Allows the user to perform a chat action.",
                       PermissionDefault::True);
    registerPermission(parent->getName() + ".tell", parent, "Allows the user to privately message another player.",
                       PermissionDefault::True);
}

void DefaultPermissions::registerCommandPermissions(Permission *parent)
{
    auto *root =
        registerPermission(parent->getName() + ".command", parent,
                           "Gives the user the ability to use all Endstone command", PermissionDefault::Console);

    registerPermission(root->getName() + ".ban", root, "Allows the user to ban players.", PermissionDefault::Operator);
    registerPermission(root->getName() + ".banip", root, "Allows the user to ban IP addresses.",
                       PermissionDefault::Operator);
    registerPermission(root->getName() + ".banlist", root, "Allows the user to list all the banned ips or players.",
                       PermissionDefault::Operator);

    registerPermission(root->getName() + ".unban", root, "Allows the user to unban players.",
                       PermissionDefault::Operator);
    registerPermission(root->getName() + ".unbanip", root, "Allows the user to unban IP addresses.",
                       PermissionDefault::Operator);

    registerPermission(root->getName() + ".plugins", root,
                       "Allows the user to view the list of plugins running on this server", PermissionDefault::True);
    registerPermission(root->getName() + ".reload", root,
                       "Allows the user to reload the configuration and plugins of the server",
                       PermissionDefault::Operator);
    registerPermission(root->getName() + ".seed", root, "Allows the user to view the seed of the level.",
                       PermissionDefault::Operator);
    registerPermission(root->getName() + ".status", root, "Allows the user to view the status of the server",
                       PermissionDefault::Operator);
    registerPermission(root->getName() + ".version", root, "Allows the user to view the version of the server",
                       PermissionDefault::True);

#ifdef ENDSTONE_WITH_DEVTOOLS
    registerPermission(root->getName() + ".devtools", root, "Allows the user to open the DevTools.",
                       PermissionDefault::Console);
#endif
    root->recalculatePermissibles();
}

void DefaultPermissions::registerBroadcastPermissions(Permission *parent)
{
    auto *root = registerPermission(parent->getName() + ".broadcast", parent,
                                    "Allows the user to receive all broadcast messages", PermissionDefault::Operator);
    registerPermission(root->getName() + ".admin", root, "Allows the user to receive administrative broadcasts",
                       PermissionDefault::Operator);
    registerPermission(root->getName() + ".user", root, "Allows the user to receive user broadcasts",
                       PermissionDefault::True);
    root->recalculatePermissibles();
}

}  // namespace endstone::core
