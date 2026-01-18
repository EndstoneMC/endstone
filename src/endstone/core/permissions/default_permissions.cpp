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

namespace endstone::core {
Permission &DefaultPermissions::registerPermission(std::unique_ptr<Permission> perm)
{
    const auto &server = EndstoneServer::getInstance();
    auto *result = server.getPluginManager().getPermission(perm->getName());
    if (result == nullptr) {
        result = &server.getPluginManager().addPermission(std::move(perm));
    }
    return *result;
}

Permission &DefaultPermissions::registerPermission(std::unique_ptr<Permission> perm, Permission &parent)
{
    parent.getChildren()[perm->getName()] = true;
    return registerPermission(std::move(perm));
}

Permission &DefaultPermissions::registerPermission(std::string_view name, std::string_view desc)
{
    return registerPermission(
        std::make_unique<Permission>(std::string(name), std::string(desc), PermissionDefault::False));
}

Permission &DefaultPermissions::registerPermission(std::string_view name, std::string_view desc, Permission &parent)
{
    auto &perm = registerPermission(name, desc);
    parent.getChildren()[perm.getName()] = true;
    return perm;
}

Permission &DefaultPermissions::registerPermission(std::string_view name, std::string_view desc, PermissionDefault def,
                                                   Permission &parent)
{
    auto &perm = registerPermission(std::make_unique<Permission>(std::string(name), std::string(desc), def));
    parent.getChildren()[perm.getName()] = true;
    return perm;
}

void DefaultPermissions::registerCorePermissions()
{
    auto &parent = registerPermission(ROOT, "Gives the user the ability to use all Endstone utilities and commands");
    CommandPermissions::registerPermissions(parent);
    BroadcastPermissions::registerPermissions(parent);
    parent.recalculatePermissibles();
}

Permission &CommandPermissions::registerPermission(std::string_view name, std::string_view cmd, std::string_view desc,
                                                   Permission &parent)
{
    return registerPermission(name, cmd, desc, PermissionDefault::True, parent);
}

Permission &CommandPermissions::registerPermission(std::string_view name, std::string_view cmd, std::string_view desc,
                                                   PermissionDefault def, Permission &parent)
{
    const auto &server = EndstoneServer::getInstance();
    const auto &registry = server.getServer().getMinecraft()->getCommands().getRegistry();
    const auto &signature = *registry.findCommand(std::string(cmd));
    if (signature.permission_level >= CommandPermissionLevel::Internal) {
        def = PermissionDefault::False;
    }
    else if (signature.permission_level >= CommandPermissionLevel::Host) {
        def = PermissionDefault::Console;
    }
    else if (signature.permission_level > CommandPermissionLevel::Any) {
        def = PermissionDefault::Operator;
    }
    return DefaultPermissions::registerPermission(name, desc, def, parent);
}

Permission &CommandPermissions::registerPermissions(Permission &parent)
{
    const static std::string PREFIX = std::string(ROOT) + ".";
    auto &commands =
        DefaultPermissions::registerPermission(ROOT, "Gives the user the ability to use all Endstone command", parent);
    registerPermission(PREFIX + "ban", "ban", "Allows the user to ban players.", PermissionDefault::Operator, commands);
    registerPermission(PREFIX + "banip", "ban-ip", "Allows the user to ban IP addresses.", PermissionDefault::Operator,
                       commands);
    registerPermission(PREFIX + "banlist", "banlist", "Allows the user to list all the banned ips or players.",
                       PermissionDefault::Operator, commands);

    registerPermission(PREFIX + "unban", "pardon", "Allows the user to unban players.", PermissionDefault::Operator,
                       commands);
    registerPermission(PREFIX + "unbanip", "pardon-ip", "Allows the user to unban IP addresses.",
                       PermissionDefault::Operator, commands);
    registerPermission(PREFIX + "plugins", "plugins",
                       "Allows the user to view the list of plugins running on this server", PermissionDefault::True,
                       commands);
    registerPermission(PREFIX + "reload", "reload",
                       "Allows the user to reload the configuration and plugins of the server",
                       PermissionDefault::Operator, commands);
    registerPermission(PREFIX + "seed", "seed", "Allows the user to view the seed of the level.",
                       PermissionDefault::Operator, commands);
    registerPermission(PREFIX + "status", "status", "Allows the user to view the status of the server",
                       PermissionDefault::Operator, commands);
    registerPermission(PREFIX + "version", "version", "Allows the user to view the version of the server",
                       PermissionDefault::True, commands);
#ifdef ENDSTONE_WITH_DEVTOOLS
    registerPermission(PREFIX + "devtools", "devtools", "Allows the user to open the DevTools.",
                       PermissionDefault::Console, commands);
#endif
    commands.recalculatePermissibles();
    return commands;
}

Permission &BroadcastPermissions::registerPermissions(Permission &parent)
{
    const static std::string PREFIX = std::string(ROOT) + ".";
    auto &broadcasts =
        DefaultPermissions::registerPermission(ROOT, "Allows the user to receive all broadcast messages");
    DefaultPermissions::registerPermission(PREFIX + "admin", "Allows the user to receive administrative broadcasts",
                                           PermissionDefault::Operator, parent);
    DefaultPermissions::registerPermission(PREFIX + "user", "Allows the user to receive user broadcasts",
                                           PermissionDefault::True, parent);
    broadcasts.recalculatePermissibles();
    return broadcasts;
}

void MinecraftDefaultPermissions::registerCorePermissions()
{
    auto &parent = DefaultPermissions::registerPermission(
        ROOT, "Gives the user the ability to use all vanilla utilities and commands");
    MinecraftCommandPermissions::registerPermissions(parent);
    parent.recalculatePermissibles();
}

Permission &MinecraftCommandPermissions::registerPermissions(Permission &parent)
{
    const static std::string PREFIX = std::string(ROOT) + ".";
    Permission &commands = DefaultPermissions::registerPermission(
        ROOT, "Gives the user the ability to use all vanilla minecraft commands", parent);
    CommandPermissions::registerPermission(PREFIX + "help", "help", "Allows the user to access Vanilla command help.",
                                           commands);
    CommandPermissions::registerPermission(PREFIX + "list", "list", "Allows the user to list all online players.",
                                           commands);
    CommandPermissions::registerPermission(PREFIX + "me", "me", "Allows the user to perform a chat action.", commands);
    CommandPermissions::registerPermission(PREFIX + "tell", "tell",
                                           "Allows the user to privately message another player.", commands);
    const auto &server = EndstoneServer::getInstance();
    const auto &registry = server.getServer().getMinecraft()->getCommands().getRegistry();
    for (const auto &[name, signature] : registry.signatures_) {
        auto command = server.getCommandMap().getCommand(name);
        auto permissions = command->getPermissions();
        auto perm = PREFIX + signature.name;
        if (permissions.size() != 1 || permissions.at(0) != perm) {
            continue;
        }
        CommandPermissions::registerPermission(
            perm, signature.name,
            fmt::format("Allows the user to use the /{} command provided by mojang.", signature.name), commands);
    }
    commands.recalculatePermissibles();
    return commands;
}
}  // namespace endstone::core
