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

#include "bedrock/command/stop_command.h"

#include "bedrock/command/command.h"
#include "bedrock/command/command_registry.h"
#include "bedrock/type_id.h"
#include "endstone_runtime/hook.h"

class TestCommand : public Command {
public:
    static void setup(CommandRegistry &registry)
    {
        registry.registerCommand("test", "Test command description", CommandPermissionLevel::Any, {128}, {0});
        auto version = CommandVersion{1, INT_MAX};
        registry.registerOverload<TestCommand>("test", version);
        registry.registerOverload<TestCommand>("test", version, CommandParameterData::create<std::string>("text"));
        registry.registerOverload<TestCommand>("test", version, CommandParameterData::create<float>("value", true));
        registry.registerOverload<TestCommand>("test", version, CommandParameterData::create<int>("value", false));
        // registry.registerOverload<TestCommand>("test", version, CommandParameterData::create<bool>("boolean"));
    }

    void execute(const struct CommandOrigin &origin, struct CommandOutput &output) const override
    {
        printf("Hello world from Endstone!!\n");
    }
};

void StopCommand::setup(CommandRegistry &registry, DedicatedServer &server)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&StopCommand::setup, registry, server);

    TestCommand::setup(registry);
}
