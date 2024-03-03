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

#include "bedrock/command/command_output.h"
#include "bedrock/command/command_registry.h"

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

private:
    std::unique_ptr<CommandOutputSender> output_sender_;  // +8
    std::unique_ptr<CommandRegistry> registry_;           // +16
};
static_assert(sizeof(MinecraftCommands) == 24);
