//
// Created by Vincent on 08/09/2023.
//

#include "endstone/command/bedrock/bedrock_command.h"

#include "bedrock/minecraft_commands.h"
#include "bedrock_command_sender.h"

bool BedrockCommand::execute(CommandSender &sender, const std::string &label,
                             const std::vector<std::string> &args) const
{
    auto &bedrock_sender = dynamic_cast<BedrockCommandSender &>(sender);
    auto command_line = toCommandLine(label, args);
    return MinecraftCommands::vanilla_dispatcher.value()(command_line, std::move(bedrock_sender.takeOrigin()));
}

std::string BedrockCommand::toCommandLine(const std::string &label, const std::vector<std::string> &args)
{
    // remove the fallback prefix in a command (e.g. minecraft:) before dispatching it using the vanilla dispatcher
    auto name = std::regex_replace(label, std::regex("^(\\w+):"), "");

    if (args.empty()) {
        return "/" + name;
    }
    else {
        return fmt::format("/{} {}", name, fmt::join(args.begin(), args.end(), " "));
    }
}
