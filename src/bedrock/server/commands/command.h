
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

#include <cstdint>

#include "bedrock/bedrock.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/server/commands/command_output.h"
#include "bedrock/server/commands/command_registry.h"

class CommandPosition {
public:
    explicit CommandPosition(const Vec3 &);

private:
    Vec3 offset_;      // +0
    bool relative_x_;  // +12
    bool relative_y_;  // +13
    bool relative_z_;  // +14
    bool local_;       // +15
};

class CommandPositionFloat : public CommandPosition {};

class RelativeFloat {
public:
    RelativeFloat() = default;
    RelativeFloat(float offset, bool relative);
    [[nodiscard]] float getValue(float base) const;
    [[nodiscard]] bool isRelative() const;

private:
    float offset_;
    bool relative_;
};

class Command {
public:
    Command() = default;
    virtual ~Command() = default;
    virtual bool collectOptionalArguments();
    virtual void execute(CommandOrigin const &, CommandOutput &) const = 0;

    [[nodiscard]] std::string getCommandName() const;
    void run(CommandOrigin const &origin, CommandOutput &output) const;

private:
    int version_ = 0;
    CommandRegistry *registry_{nullptr};                                          // +16
    CommandRegistry::Symbol command_symbol_;                                      // +24
    CommandPermissionLevel permission_level_ = CommandPermissionLevel::Internal;  // +28
    CommandFlag flags_;                                                           // +30
};
BEDROCK_STATIC_ASSERT_SIZE(Command, 32, 32);
