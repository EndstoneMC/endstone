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

#include "endstone/command/command_sender.h"

namespace endstone {

/**
 * @brief Represents a wrapper that forwards commands to the wrapped CommandSender and captures its output
 */
class CommandSenderWrapper final : public CommandSender {
public:
    using Callback = std::function<void(const Message &)>;
    explicit CommandSenderWrapper(CommandSender &sender, Callback on_message = {}, Callback on_error = {})
        : sender_(sender), on_message_(std::move(on_message)), on_error_(std::move(on_error)) {};

    void sendMessage(const Message &message) const override
    {
        if (on_message_) {
            on_message_(message);
        }
    }
    void sendErrorMessage(const Message &message) const override
    {
        if (on_error_) {
            on_error_(message);
        }
    }

    [[nodiscard]] PermissionLevel getPermissionLevel() const override
    {
        return sender_.getPermissionLevel();
    }

    [[nodiscard]] bool isPermissionSet(std::string name) const override
    {
        return sender_.isPermissionSet(name);
    }

    [[nodiscard]] bool isPermissionSet(const Permission &perm) const override
    {
        return sender_.isPermissionSet(perm);
    }

    [[nodiscard]] bool hasPermission(std::string name) const override
    {
        return sender_.hasPermission(name);
    }

    [[nodiscard]] bool hasPermission(const Permission &perm) const override
    {
        return sender_.hasPermission(perm);
    }

    PermissionAttachment *addAttachment(Plugin &plugin, const std::string &name, bool value) override
    {
        return sender_.addAttachment(plugin, name, value);
    }

    PermissionAttachment *addAttachment(Plugin &plugin) override
    {
        return sender_.addAttachment(plugin);
    }

    Result<void> removeAttachment(PermissionAttachment &attachment) override
    {
        return sender_.removeAttachment(attachment);
    }

    void recalculatePermissions() override
    {
        sender_.recalculatePermissions();
    }

    [[nodiscard]] std::unordered_set<PermissionAttachmentInfo *> getEffectivePermissions() const override
    {
        return sender_.getEffectivePermissions();
    }

    [[nodiscard]] ConsoleCommandSender *asConsole() const override
    {
        return sender_.asConsole();
    }

    [[nodiscard]] Actor *asActor() const override
    {
        return sender_.asActor();
    }

    [[nodiscard]] Player *asPlayer() const override
    {
        return sender_.asPlayer();
    }

    [[nodiscard]] Server &getServer() const override
    {
        return sender_.getServer();
    }

    [[nodiscard]] std::string getName() const override
    {
        return sender_.getName();
    }

private:
    CommandSender &sender_;
    Callback on_message_;
    Callback on_error_;
};

}  // namespace endstone
