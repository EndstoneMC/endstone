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

#include <string>

#include "bedrock/server/commands/command_selector.h"
#include "endstone/command/argument_type.h"
#include "endstone/command/command_sender.h"

namespace endstone::core {

/**
 * Returns the built-in type an argument ends up being parsed as.
 *
 * A custom argument type borrows a built-in type's grammar, so follow the chain to whichever
 * built-in actually reaches the command registry.
 */
inline ArgumentKind effectiveArgumentKind(const ArgumentType &type)
{
    if (type.getKind() != ArgumentKind::Custom) {
        return type.getKind();
    }
    const auto native = type.getNativeType();
    return native ? effectiveArgumentKind(*native) : ArgumentKind::Custom;
}

/**
 * Returns whether an argument of this kind is parsed into a target selector.
 */
inline bool isSelectorArgument(const ArgumentKind kind)
{
    switch (kind) {
    case ArgumentKind::Player:
    case ArgumentKind::Players:
    case ArgumentKind::Entity:
    case ArgumentKind::Entities:
        return true;
    default:
        return false;
    }
}

/**
 * Converts the text the command registry parsed into the argument's value.
 *
 * Throws CommandError if the text does not satisfy the argument's constraints.
 */
ArgumentValue convertArgument(const ArgumentType &type, const std::string &text, const NotNull<CommandSender> &sender);

/**
 * Converts the actors a target selector matched into the argument's value.
 *
 * Throws CommandError if the selector matched nothing, or matched more than one where the argument
 * accepts a single target.
 */
ArgumentValue convertSelector(const ArgumentType &type, const CommandResultVector &results,
                              const NotNull<CommandSender> &sender);

}  // namespace endstone::core
