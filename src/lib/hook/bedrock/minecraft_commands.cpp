//
// Created by Vincent on 06/09/2023.
//

#include "bedrock/minecraft_commands.h"

#include "endstone/command/bedrock/bedrock_command.h"
#include "endstone/command/bedrock/bedrock_command_sender.h"
#include "endstone/command/command_map.h"
#include "endstone/endstone.h"
#include "endstone/endstone_server.h"
#include "hook/hook.h"
MCRESULT MinecraftCommands::executeCommand(CommandContext &command_ctx, bool flag) const
{
    constexpr MCRESULT success{true, 0, 0};
    constexpr MCRESULT not_found{false, 0, 1};

    auto command_name = CommandMap::getCommandName(command_ctx.getCommandLine());

    auto &server = dynamic_cast<EndstoneServer &>(Endstone::getServer());
    auto command = server.getCommandMap().getCommand(command_name);

    MCRESULT result;

    if (std::dynamic_pointer_cast<BedrockCommand>(command)) {
        // replace the fallback prefix in a command (e.g. /minecraft:) with a forward slash (i.e. /)
        command_ctx.setCommandLine(std::regex_replace(command_ctx.getCommandLine(), std::regex("^/(\\w+):"), "/"));
        result = *ENDSTONE_HOOK_CALL_ORIGINAL_RVO(&MinecraftCommands::executeCommand, this, &result, command_ctx, flag);
    }
    else {
        auto sender = BedrockCommandSender::fromCommandOrigin(command_ctx.getCommonOrigin());
        if (server.dispatchCommand(*sender, command_ctx.getCommandLine())) {
            result = success;
        }
        else {
            result = not_found;
        }
    }

    return result;
}
