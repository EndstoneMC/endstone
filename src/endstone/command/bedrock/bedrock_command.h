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

#pragma once

#include <string>
#include <utility>
#include <vector>

#include "bedrock/command_context.h"
#include "endstone/command/endstone_command.h"

class BedrockCommand : public EndstoneCommand {
public:
    explicit BedrockCommand(const std::string &name) noexcept : BedrockCommand(name, "", {"/" + name}, {}) {}
    explicit BedrockCommand(const std::string &name, std::string description, const std::vector<std::string> &usages,
                            const std::vector<std::string> &aliases) noexcept
        : EndstoneCommand(name, std::move(description), usages, aliases)
    {
        setPermission("minecraft.command." + name);
    }

    bool execute(CommandSender &sender, const std::string &label,
                 const std::vector<std::string> &args) const noexcept override;

private:
    static std::string toCommandLine(const std::string &label, const std::vector<std::string> &args) noexcept;
};
