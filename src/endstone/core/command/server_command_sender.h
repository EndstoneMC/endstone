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

#include <utility>

#include "endstone/core/permissions/permissible_base.h"
#include "endstone/core/server.h"

namespace endstone::core {
template <typename Interface>
class ServerCommandSender : public Interface {
protected:
    explicit ServerCommandSender(std::shared_ptr<PermissibleBase> perm = nullptr)
    {
        if (perm) {
            perm_ = std::move(perm);
        }
        else {
            perm_ = std::make_shared<PermissibleBase>(this);
        }
    }

public:
    [[nodiscard]] Server &getServer() const override
    {
        return EndstoneServer::getInstance();
    }

    [[nodiscard]] bool isPermissionSet(std::string name) const override
    {
        return perm_->isPermissionSet(name);
    }

    [[nodiscard]] bool isPermissionSet(const Permission &perm) const override
    {
        return perm_->isPermissionSet(perm);
    }

    [[nodiscard]] bool hasPermission(std::string name) const override
    {
        return perm_->hasPermission(name);
    }

    [[nodiscard]] bool hasPermission(const Permission &perm) const override
    {
        return perm_->hasPermission(perm);
    }

    PermissionAttachment *addAttachment(Plugin &plugin, const std::string &name, bool value) override
    {
        return perm_->addAttachment(plugin, name, value);
    }

    PermissionAttachment *addAttachment(Plugin &plugin) override
    {
        return perm_->addAttachment(plugin);
    }

    bool removeAttachment(PermissionAttachment &attachment) override
    {
        return perm_->removeAttachment(attachment);
    }

    void recalculatePermissions() override
    {
        perm_->recalculatePermissions();
    }

    [[nodiscard]] std::unordered_set<PermissionAttachmentInfo *> getEffectivePermissions() const override
    {
        return perm_->getEffectivePermissions();
    }

    [[nodiscard]] ConsoleCommandSender *asConsole() const override
    {
        return nullptr;
    }

    [[nodiscard]] BlockCommandSender *asBlock() const override
    {
        return nullptr;
    }

    [[nodiscard]] Actor *asActor() const override
    {
        return nullptr;
    }

    [[nodiscard]] Player *asPlayer() const override
    {
        return nullptr;
    }

private:
    mutable std::shared_ptr<PermissibleBase> perm_;
};

}  // namespace endstone::core
