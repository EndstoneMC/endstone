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
#include <unordered_set>
#include <variant>
#include <vector>

#include "bedrock/server/commands/command.h"
#include "bedrock/server/commands/command_registry.h"
#include "endstone/command/command_sender.h"
#include "endstone/core/command/compiler/command_compiler.h"
#include "endstone/core/server.h"

template <typename T>
class CommandSelector;

namespace endstone::core {

class CommandAdapter : public ::Command {
public:
    CommandAdapter();
    CommandAdapter(const CommandAdapter &) = delete;
    CommandAdapter &operator=(const CommandAdapter &) = delete;
    ~CommandAdapter() override;
    void execute(const CommandOrigin &origin, CommandOutput &output) const override;
    [[nodiscard]] virtual bool run(const CommandOrigin &origin, const NotNull<CommandSender> &sender,
                                   const std::string &input) const = 0;
    [[nodiscard]] static const CommandAdapter *find(const ::Command *command);

private:
    static std::unordered_set<const ::Command *> instances_;
};

class MinecraftCommandAdapter : public CommandAdapter {
public:
    MinecraftCommandAdapter(std::string label, RegisteredOverload registered);
    [[nodiscard]] bool run(const CommandOrigin &origin, const NotNull<CommandSender> &sender,
                           const std::string &input) const override;

private:
    friend class ::CommandRegistry;
    std::vector<CommandSlot> args_;
    std::string label_;
    RegisteredOverload registered_;
};

class BrigadierCommandAdapter : public CommandAdapter {
public:
    BrigadierCommandAdapter(std::string label, RegisteredOverload registered);
    [[nodiscard]] bool run(const CommandOrigin &origin, const NotNull<CommandSender> &sender,
                           const std::string &input) const override;
    static void *getSlot(::Command *command, int index);
    static bool *isSlotSet(::Command *command, int index);

private:
    std::string label_;
    RegisteredOverload registered_;
    std::vector<CommandSlot> slots_;
    std::vector<std::variant<std::monostate, std::shared_ptr<CommandSelector<::Actor>>,
                             std::shared_ptr<CommandSelector<::Player>>, CommandPosition, CommandPositionFloat>>
        native_slots_;
};

}  // namespace endstone::core

template <>
bool CommandRegistry::parse<endstone::core::MinecraftCommandAdapter>(void *storage, const ParseToken &token,
                                                                     const CommandOrigin &origin, int version,
                                                                     std::string &error,
                                                                     std::vector<std::string> &error_params) const;
template <>
const CommandRegistry::ParamParseRule CommandRegistry::ParseRuleFor<endstone::core::MinecraftCommandAdapter>::instance;
template <>
bool CommandRegistry::parse<endstone::core::BrigadierCommandAdapter>(void *storage, const ParseToken &token,
                                                                     const CommandOrigin &origin, int version,
                                                                     std::string &error,
                                                                     std::vector<std::string> &error_params) const;
