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

#include "endstone/command/bedrock/bedrock_command_sender.h"
#include "endstone/permission/permissible_base.h"
#include "endstone/server.h"

class ServerCommandSender : public BedrockCommandSender {
public:
    using BedrockCommandSender::BedrockCommandSender;

    void sendMessage(const std::string &message) const noexcept override
    {
        getServer().getLogger().info(message);
    }

    [[nodiscard]] PermissibleRole getRole() const noexcept override
    {
        return PermissibleRole::Owner;
    }

    [[nodiscard]] bool setRole(PermissibleRole role) const noexcept override
    {
        if (role == getRole()) {
            return true;
        }

        getServer().getLogger().error("Cannot change role of server console");
        return false;
    }

    [[nodiscard]] bool isPermissionSet(const std::string &name) const noexcept override
    {
        return perm_.isPermissionSet(name);
    }

    [[nodiscard]] bool hasPermission(const std::string &name) const noexcept override
    {
        return perm_.hasPermission(name);
    }

    PermissionAttachment *addAttachment(Plugin &plugin, const std::string &name, bool value) noexcept override
    {
        return perm_.addAttachment(plugin, name, value);
    }

    PermissionAttachment *addAttachment(Plugin &plugin) noexcept override
    {
        return perm_.addAttachment(plugin);
    }

    bool removeAttachment(const PermissionAttachment &attachment) noexcept override
    {
        return perm_.removeAttachment(attachment);
    }

    void recalculatePermissions() noexcept override
    {
        perm_.recalculatePermissions();
    }

private:
    PermissibleBase perm_ = PermissibleBase(*this);
};
