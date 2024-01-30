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

#include <optional>

#include <spdlog/spdlog.h>

#include "bedrock/command/command.h"
#include "bedrock/command/command_registry.h"
#include "bedrock/type_id.h"
#include "endstone_runtime/hook.h"

class TestCommand : public Command {
public:
    static void setup(CommandRegistry &registry)
    {
        auto version = CommandVersion{1, INT_MAX};

        registry.registerCommand("test", "Test command description", CommandPermissionLevel::Any, {128}, {0});
        registry.registerOverload<TestCommand>("test", version);
        registry.registerOverload<TestCommand>("test", version,
                                               CommandParameterData::create("text", &TestCommand::text_));

        registry.registerOverload<TestCommand>("test", version,
                                               CommandParameterData::create("value", &TestCommand::i_));

        registry.registerCommand("testo", "Test optional command description", CommandPermissionLevel::Any, {128}, {0});
        registry.registerOverload<TestCommand>("testo", version,
                                               CommandParameterData::create("optional value", &TestCommand::f_));
    }

    void execute(const struct CommandOrigin &origin, struct CommandOutput &output) const override
    {
        spdlog::info("String: {}", text_);
        spdlog::info("Int: {}", i_);

        if (f_.has_value()) {
            spdlog::info("Float: {}", f_.value());
        }
        else {
            spdlog::info("Float: no value");
        }
    }

private:
    std::string text_;
    int i_;
    std::optional<float> f_;
};

void StopCommand::setup(CommandRegistry &registry, DedicatedServer &server)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&StopCommand::setup, registry, server);

    TestCommand::setup(registry);
}
