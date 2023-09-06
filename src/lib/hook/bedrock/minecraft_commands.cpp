//
// Created by Vincent on 06/09/2023.
//

#include "bedrock/minecraft_commands.h"

#include "endstone/command/bedrock_command.h"
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

    if (std::dynamic_pointer_cast<BedrockCommandPlaceHolder>(command)) {
        // replace the fallback prefix in a command (e.g. /minecraft:) with a forward slash (i.e. /)
        command_ctx.setCommandLine(std::regex_replace(command_ctx.getCommandLine(), std::regex("^/(\\w+):"), "/"));
        MCRESULT result;
        ENDSTONE_HOOK_CALL_ORIGINAL_RVO(&MinecraftCommands::executeCommand, this, &result, command_ctx, flag);
        return result;
    }
    else {
        // TODO: check origin type and pass the right command sender (e.g. PlayerCommandSender)
        if (server.dispatchCommand(server.getConsoleSender(), command_ctx.getCommandLine())) {
            return success;
        }
        else {
            return not_found;
        }
    }
}
