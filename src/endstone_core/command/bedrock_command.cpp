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

#include "endstone/detail/command/bedrock_command.h"

#include <utility>

namespace endstone::detail {

BedrockCommand::BedrockCommand(std::string name, std::string description, std::vector<std::string> usages,
                               std::vector<std::string> aliases, std::vector<std::string> permissions)
    : Command(std::move(name), std::move(description), std::move(usages), std::move(aliases), std::move(permissions))
{
    setPermissions("minecraft.commands." + getName());
}

bool BedrockCommand::execute(CommandSender &sender, const std::vector<std::string> &args) const
{
    sender.sendErrorMessage("Command '/{}' is provided by Bedrock Dedicated Server and is not executable.", getName());
    return false;
}

}  // namespace endstone::detail
