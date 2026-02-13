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

#include "endstone/core/command/minecraft_command_wrapper.h"

#include <boost/algorithm/string/join.hpp>

#include "bedrock/locale/i18n.h"
#include "bedrock/server/commands/command_origin_loader.h"
#include "bedrock/server/commands/minecraft_commands.h"
#include "endstone/core/permissions/default_permissions.h"
#include "endstone/core/server.h"

namespace endstone::core {

MinecraftCommandWrapper::MinecraftCommandWrapper(MinecraftCommands &minecraft_commands,
                                                 const CommandRegistry::Signature &signature)
    : EndstoneCommand(signature.name), minecraft_commands_(minecraft_commands)
{
    // Description
    auto description = getI18n().get(signature.description, {}, nullptr);
    setDescription(std::move(description));

    // Usages
    // TODO(fixme): re-enable this
    // std::vector<std::string> usages;
    // usages.reserve(signature.overloads.size());
    // for (const auto &overload : signature.overloads) {
    //     usages.push_back(minecraft_commands.getRegistry().describe(signature, overload));
    // }
    // setUsages(std::move(usages));

    // Permissions
    setPermissions(getPermission(signature));
}

bool MinecraftCommandWrapper::execute(CommandSender &sender, const std::vector<std::string> &args) const
{
    if (!testPermission(sender)) {
        return true;
    }

    const auto command_origin = getCommandOrigin(sender);
    if (!command_origin) {
        sender.sendErrorMessage("Unsupported sender type!");
        return false;
    }

    // compile command
    std::vector command_parts = {"/" + getName()};
    command_parts.insert(command_parts.end(), args.begin(), args.end());
    const auto full_command = boost::algorithm::join(command_parts, " ");
    auto *command = minecraft_commands_.compileCommand(  //
        full_command, *command_origin, CurrentCmdVersion::Latest,
        [&sender](auto const &err) { sender.sendErrorMessage(err); });

    if (!command) {
        return false;
    }

    // we've already done a stricter permission check above, we can safely bypass the vanilla check
    const auto permission_level = command->permission_level_;
    command->permission_level_ = CommandPermissionLevel::Any;  // bypass
    CommandOutput output{MinecraftCommands::getOutputType(*command_origin)};
    command->run(*command_origin, output);
    command->permission_level_ = permission_level;  // clean up

    // redirect outputs to sender
    for (const auto &message : output.getMessages()) {
        switch (message.getType()) {
        case CommandOutputMessageType::Success:
            sender.sendMessage(Translatable(message.getMessageId(), message.getParams()));
            break;
        case CommandOutputMessageType::Error:
            sender.sendErrorMessage(Translatable(message.getMessageId(), message.getParams()));
            break;
        }
    }

    return output.getSuccessCount() > 0;
}

std::unique_ptr<CommandOrigin> MinecraftCommandWrapper::getCommandOrigin(CommandSender &sender)
{
    const auto *level = EndstoneServer::getInstance().getEndstoneLevel();
    if (const auto *console = sender.asConsole(); console) {
        ::CompoundTag tag;
        {
            tag.putByte("OriginType", static_cast<std::uint8_t>(CommandOriginType::DedicatedServer));
            tag.putString("RequestId", mce::UUID::EMPTY.asString());
            tag.putByte("CommandPermissionLevel", static_cast<std::uint8_t>(CommandPermissionLevel::Owner));
            tag.putString("DimensionId", "overworld");
        }
        return CommandOriginLoader::load(tag, static_cast<ServerLevel &>(level->getHandle()));
    }

    if (const auto *player = sender.asPlayer(); player) {
        ::CompoundTag tag;
        {
            tag.putByte("OriginType", static_cast<std::uint8_t>(CommandOriginType::Player));
            tag.putInt64("PlayerId", player->getId());
        }
        return CommandOriginLoader::load(tag, static_cast<ServerLevel &>(level->getHandle()));
    }

    if (const auto *actor = sender.asActor(); actor) {
        ::CompoundTag tag;
        {
            tag.putByte("OriginType", static_cast<std::uint8_t>(CommandOriginType::Entity));
            tag.putInt64("EntityId", actor->getId());
        }
        return CommandOriginLoader::load(tag, static_cast<ServerLevel &>(level->getHandle()));
    }

    return nullptr;
}

std::string MinecraftCommandWrapper::getPermission(const CommandRegistry::Signature &minecraft_command)
{
    return "minecraft.command." + minecraft_command.name;
}

}  // namespace endstone::core
