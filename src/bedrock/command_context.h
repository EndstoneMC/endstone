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

#include <memory>
#include <string>
#include <utility>

#include "bedrock/command_origin.h"
#include "bedrock/internal.h"

class CommandContext {
public:
    // NOLINTNEXTLINE(modernize-pass-by-value)
    CommandContext(const std::string &command_line, std::unique_ptr<CommandOrigin> sender, int command_version)
        : command_line_(command_line), origin_(std::move(sender)), command_version_(command_version)
    {
    }

    [[nodiscard]] const std::string &getCommandLine() const
    {
        return command_line_;
    }

    void setCommandLine(const std::string &command_line)
    {
        command_line_ = command_line;
    }

    [[nodiscard]] const std::unique_ptr<CommandOrigin> &getOrigin() const
    {
        return origin_;
    }

    std::unique_ptr<CommandOrigin> takeOrigin()
    {
        return std::move(origin_);
    }

    [[nodiscard]] int getCommandVersion() const
    {
        return command_version_;
    }

private:
    std::string command_line_;               // +0
    std::unique_ptr<CommandOrigin> origin_;  // +32
    int command_version_;                    // +40
};

static_assert(sizeof(CommandContext) == 48);
