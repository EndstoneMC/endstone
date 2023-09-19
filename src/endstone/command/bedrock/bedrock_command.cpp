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

#include "endstone/command/bedrock/bedrock_command.h"

#include <regex>
#include <utility>

#include "bedrock/minecraft_commands.h"
#include "endstone/command/bedrock/bedrock_command_sender.h"

bool BedrockCommand::execute(CommandSender &sender, const std::string &label,
                             const std::vector<std::string> &args) const noexcept
{
    auto &bedrock_sender = dynamic_cast<BedrockCommandSender &>(sender);
    auto command_line = toCommandLine(label, args);
    return MinecraftCommands::mVanillaDispatcher(command_line, std::move(bedrock_sender.takeOrigin()));
}

std::string BedrockCommand::toCommandLine(const std::string &label, const std::vector<std::string> &args) noexcept
{
    // remove the fallback prefix in a command (e.g. minecraft:) before dispatching it using the vanilla dispatcher
    auto name = std::regex_replace(label, std::regex("^(\\w+):"), "");

    if (args.empty()) {
        return "/" + name;
    }
    return fmt::format("/{} {}", name, fmt::join(args.begin(), args.end(), " "));
}
