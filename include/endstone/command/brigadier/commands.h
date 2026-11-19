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

#include <concepts>
#include <memory>
#include <set>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "endstone/brigadier/builder.h"
#include "endstone/command/brigadier/basic_command.h"
#include "endstone/command/brigadier/command_source_stack.h"
#include "endstone/command/soft_enum.h"
#include "endstone/util/pointers.h"

namespace endstone {

/**
 * The registrar for a plugin's commands and soft enums.
 */
class Commands {
public:
    virtual ~Commands() = default;

    /**
     * Creates a literal node builder, the keyword a command or a subcommand starts with.
     *
     * @param name the keyword
     * @return the builder
     */
    static brigadier::LiteralArgumentBuilder<CommandSource> literal(const std::string &name)
    {
        return brigadier::LiteralArgumentBuilder<CommandSource>::literal(name);
    }

    /**
     * Creates an argument node builder.
     *
     * @param name the name the command reads the argument back by
     * @param type the type of the argument
     * @return the builder
     */
    template <typename T>
        requires std::derived_from<std::remove_const_t<T>, brigadier::ArgumentType>
    static brigadier::RequiredArgumentBuilder<CommandSource> argument(const std::string &name, std::shared_ptr<T> type)
    {
        return brigadier::RequiredArgumentBuilder<CommandSource>::argument(name, std::move(type));
    }

    /**
     * Creates a soft enum owned by this plugin, removed with it when the plugin is disabled.
     *
     * Registering a name this plugin already owns replaces its values and returns the same soft enum. The values can
     * be changed later from any thread through the returned SoftEnum.
     *
     * @param name the name the client sees
     * @param values the initial values
     * @return the soft enum
     * @throws std::invalid_argument if the name is empty or holds whitespace, a value is empty or holds whitespace, or
     * the name belongs to the server or another plugin
     * @throws std::runtime_error if called off the server thread
     */
    virtual NotNull<SoftEnum> registerSoftEnum(std::string name, std::vector<std::string> values = {}) = 0;

    /**
     * Registers a command tree, removed when this plugin is disabled.
     *
     * The label is the root node's literal. A label or alias another command holds is skipped, and the command is
     * still reachable as `plugin:label`.
     *
     * @param node the root node of the command
     * @param description a short description of the command
     * @param aliases the aliases of the command
     * @return the labels that were registered, including aliases and `plugin:label` forms
     * @throws std::invalid_argument if the tree cannot be written as a Bedrock command, naming the clash
     * @throws std::runtime_error if called off the server thread
     */
    virtual std::set<std::string> registerCommand(const NotNull<brigadier::LiteralCommandNode<CommandSource>> &node,
                                                  std::string description = "",
                                                  std::vector<std::string> aliases = {}) = 0;

    /**
     * Registers a command that takes its arguments as words, which the client shows as `/label <args: message>`.
     *
     * @param label the label of the command
     * @param description a short description of the command
     * @param aliases the aliases of the command
     * @param command the command
     * @return the labels that were registered, including aliases and `plugin:label` forms
     * @throws std::invalid_argument if the label is empty or holds whitespace
     * @throws std::runtime_error if called off the server thread
     */
    virtual std::set<std::string> registerCommand(std::string label, std::string description,
                                                  std::vector<std::string> aliases, NotNull<BasicCommand> command) = 0;
};

}  // namespace endstone
