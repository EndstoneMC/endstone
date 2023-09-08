//
// Created by Vincent on 06/09/2023.
//

#include "bedrock/minecraft_commands.h"

#include "bedrock/command_context.h"
#include "endstone/command/bedrock/bedrock_command_sender.h"
#include "endstone/command/command_map.h"
#include "endstone/endstone.h"
#include "endstone/endstone_server.h"
#include "hook/hook.h"

MCRESULT MinecraftCommands::executeCommand(CommandContext &command_ctx, bool flag) const
{
    constexpr static auto func_name = __FUNCDNAME__;

    if (!vanilla_dispatcher.has_value()) {
        vanilla_dispatcher = [this, version = command_ctx.getCommandVersion(), flag](auto &command_line,
                                                                                     auto command_origin) -> bool {
            MCRESULT result = {};

            CommandContext ctx = {
                // replace the fallback prefix in a command (e.g. /minecraft:) with a forward slash (i.e. /)
                std::regex_replace(command_line, std::regex("^/(\\w+):"), "/"),
                // take the ownership of command origin
                std::move(command_origin), version};

            result = *endstone::hook::get_function_rvo(&MinecraftCommands::executeCommand, func_name)(this, &result,
                                                                                                      ctx, flag);
            return result.is_success;
        };
    }

    constexpr static MCRESULT success = {true};
    constexpr static MCRESULT failed = {false};

    auto command_name = CommandMap::getCommandName(command_ctx.getCommandLine());

    auto &server = dynamic_cast<EndstoneServer &>(Endstone::getServer());
    auto command = server.getCommandMap().getCommand(command_name);

    MCRESULT result;

    auto sender = BedrockCommandSender::fromCommandOrigin(command_ctx.takeOrigin());
    if (server.dispatchCommand(*sender, command_ctx.getCommandLine())) {
        result = success;
    }
    else {
        result = failed;
    }

    return result;
}
