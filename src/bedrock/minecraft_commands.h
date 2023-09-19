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

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "bedrock/command_context.h"
#include "bedrock/internal.h"
#include "endstone/command/bedrock/bedrock_command.h"

struct MCRESULT {
    bool is_success;
    uint8_t category;
    uint16_t error_code;

    [[nodiscard]] constexpr int getFullCode() const
    {
        return error_code | ((category | (is_success ? 0 : 0x8000)) << 16);
    }

    friend bool operator==(const MCRESULT &lhs, const MCRESULT &rhs)
    {
        return lhs.is_success == rhs.is_success && lhs.category == rhs.category && lhs.error_code == rhs.error_code;
    }

    friend bool operator!=(const MCRESULT &lhs, const MCRESULT &rhs)
    {
        return !(lhs == rhs);
    }

    static const MCRESULT SUCCESS;
    static const MCRESULT FAILED;
};
inline const MCRESULT MCRESULT::SUCCESS = {true};
inline const MCRESULT MCRESULT::FAILED = {false};

class MinecraftCommands {
public:
    BEDROCK_API MCRESULT executeCommand(CommandContext &command_ctx, bool flag) const;

private:
    inline static std::function<bool(const std::string &, std::unique_ptr<CommandOrigin>)> mVanillaDispatcher;

    // Allow the BedrockCommand::execute to use the vanilla executor
    friend bool BedrockCommand::execute(CommandSender &sender, const std::string &label,
                                        const std::vector<std::string> &args) const noexcept;
};
