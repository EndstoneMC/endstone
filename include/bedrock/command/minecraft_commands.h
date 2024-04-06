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

#pragma once

#include "bedrock/command/command_context.h"
#include "bedrock/command/command_output.h"
#include "bedrock/command/command_registry.h"

struct MCRESULT {
    bool is_success;
    std::uint8_t category;
    std::uint16_t error_code;
};
inline MCRESULT const MCRESULT_Success{true};                  // NOLINT
inline MCRESULT const MCRESULT_CommandsDisabled{false, 0, 7};  // NOLINT

class MinecraftCommands {
public:
    virtual ~MinecraftCommands() = default;

    [[nodiscard]] CommandOutputSender &getOutputSender() const
    {
        return *output_sender_;
    }

    [[nodiscard]] CommandRegistry &getRegistry() const
    {
        return *registry_;
    }

    BEDROCK_API MCRESULT executeCommand(CommandContext &ctx, bool flag) const;

private:
    std::unique_ptr<CommandOutputSender> output_sender_;  // +8
    std::unique_ptr<CommandRegistry> registry_;           // +16
};

static_assert(sizeof(MinecraftCommands) == 24);
