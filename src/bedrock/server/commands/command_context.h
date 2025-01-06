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

#include <memory>
#include <string>
#include <utility>

#include "bedrock/server/commands/command_origin.h"

class CommandContext {
public:
    CommandContext(std::string command, std::unique_ptr<CommandOrigin> origin, int version)
        : command_(std::move(command)), origin_(std::move(origin)), version_(version)
    {
    }

    [[nodiscard]] std::string getCommand() const
    {
        return command_;
    }

    [[nodiscard]] CommandOrigin &getOrigin() const
    {
        return *origin_;
    }

private:
    std::string command_;                    // +0
    std::unique_ptr<CommandOrigin> origin_;  // +32
    int version_;                            // +40
};
// static_assert(sizeof(CommandContext) == 48);
