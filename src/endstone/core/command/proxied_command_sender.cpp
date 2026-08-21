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

#include "endstone/core/command/proxied_command_sender.h"

#include "endstone/core/message.h"

namespace endstone::core {

EndstoneProxiedCommandSender::EndstoneProxiedCommandSender(const VirtualCommandOrigin &origin, CommandOutput &output)
    : origin_(origin), output_(output)
{
}

void EndstoneProxiedCommandSender::sendMessage(const Message &message) const
{
    const auto tr = EndstoneMessage::toTranslatable(message);
    std::vector<CommandOutputParameter> params;
    for (const auto &param : tr.getParameters()) {
        params.emplace_back(param);
    }
    output_.forceOutput(tr.getText(), params);
}

void EndstoneProxiedCommandSender::sendErrorMessage(const Message &message) const
{
    const auto tr = EndstoneMessage::toTranslatable(message);
    std::vector<CommandOutputParameter> params;
    for (const auto &param : tr.getParameters()) {
        params.emplace_back(param);
    }
    output_.error(tr.getText(), params);
}

std::string EndstoneProxiedCommandSender::getName() const
{
    return origin_.getName();
}

NotNull<CommandSender> EndstoneProxiedCommandSender::getCaller() const
{
    return origin_.getOutputReceiver().getEndstoneSender(output_);
}

NotNull<CommandSender> EndstoneProxiedCommandSender::getCallee() const
{
    return origin_.getOrigin()->getEndstoneSender(output_);
}

PermissionLevel EndstoneProxiedCommandSender::getPermissionLevel() const
{
    return getCaller()->getPermissionLevel();
}

bool EndstoneProxiedCommandSender::isPermissionSet(std::string name) const
{
    return getCaller()->isPermissionSet(std::move(name));
}

bool EndstoneProxiedCommandSender::isPermissionSet(const NotNull<Permission> &perm) const
{
    return getCaller()->isPermissionSet(perm);
}

bool EndstoneProxiedCommandSender::hasPermission(std::string name) const
{
    return getCaller()->hasPermission(std::move(name));
}

bool EndstoneProxiedCommandSender::hasPermission(const NotNull<Permission> &perm) const
{
    return getCaller()->hasPermission(perm);
}

NotNull<PermissionAttachment> EndstoneProxiedCommandSender::addAttachment(Plugin &plugin, const std::string &name,
                                                                         bool value)
{
    return getCaller()->addAttachment(plugin, name, value);
}

NotNull<PermissionAttachment> EndstoneProxiedCommandSender::addAttachment(Plugin &plugin)
{
    return getCaller()->addAttachment(plugin);
}

bool EndstoneProxiedCommandSender::removeAttachment(const NotNull<PermissionAttachment> &attachment)
{
    return getCaller()->removeAttachment(attachment);
}

void EndstoneProxiedCommandSender::recalculatePermissions()
{
    getCaller()->recalculatePermissions();
}

std::unordered_set<NotNull<PermissionAttachmentInfo>> EndstoneProxiedCommandSender::getEffectivePermissions() const
{
    return getCaller()->getEffectivePermissions();
}

}  // namespace endstone::core
