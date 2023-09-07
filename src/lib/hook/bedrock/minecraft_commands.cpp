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

    if (!original_executor.has_value()) {
        original_executor = [this, &command_ctx, flag](auto &command_line, auto command_origin) -> bool {
            MCRESULT result{};
            // replace the fallback prefix in a command (e.g. /minecraft:) with a forward slash (i.e. /)
            auto ctx = CommandContext{std::regex_replace(command_ctx.getCommandLine(), std::regex("^/(\\w+):"), "/"),
                                      std::move(command_origin), command_ctx.getCommandVersion()};
            result = *ENDSTONE_HOOK_CALL_ORIGINAL_RVO(&MinecraftCommands::executeCommand, this, &result, ctx, flag);
            return result.is_success;

        };
    }

    constexpr MCRESULT success{true, 0, 0};
    constexpr MCRESULT not_found{false, 0, 1};

    auto command_name = CommandMap::getCommandName(command_ctx.getCommandLine());

    auto &server = dynamic_cast<EndstoneServer &>(Endstone::getServer());
    auto command = server.getCommandMap().getCommand(command_name);

    MCRESULT result{};

    auto sender = BedrockCommandSender::fromCommandOrigin(command_ctx.getCommonOrigin());
    if (server.dispatchCommand(*sender, command_ctx.getCommandLine())) {
        result = success;
    }
    else {
        result = not_found;
    }

    return result;
}
