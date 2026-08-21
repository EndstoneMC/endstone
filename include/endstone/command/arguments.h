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

#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "endstone/command/argument_type.h"

namespace endstone {

/**
 * Factories for the argument types Endstone provides.
 *
 * Each one maps onto a grammar the Bedrock client already knows, so parameters keep native
 * completion and hints as the player types.
 */
class Arguments {
public:
    /** A boolean, suggested as true or false. */
    static NotNull<ArgumentType> boolean() { return make(ArgumentKind::Boolean); }

    /** An integer, optionally bounded. */
    static NotNull<ArgumentType> integer(int minimum = std::numeric_limits<int>::min(),
                                         int maximum = std::numeric_limits<int>::max())
    {
        auto type = std::make_shared<BuiltinArgumentType>(ArgumentKind::Integer);
        type->setRange(minimum, maximum);
        return type;
    }

    /** A floating-point number, optionally bounded. */
    static NotNull<ArgumentType> floating(float minimum = -std::numeric_limits<float>::infinity(),
                                          float maximum = std::numeric_limits<float>::infinity())
    {
        auto type = std::make_shared<BuiltinArgumentType>(ArgumentKind::Float);
        type->setRange(minimum, maximum);
        return type;
    }

    /** A single word, or a quoted string if it contains spaces. */
    static NotNull<ArgumentType> string() { return make(ArgumentKind::String); }

    /**
     * Everything up to the end of the line.
     *
     * Must be the last argument on its branch.
     */
    static NotNull<ArgumentType> message() { return make(ArgumentKind::Message); }

    /**
     * Everything up to the end of the line, taken verbatim.
     *
     * Unlike message(), selectors in the text are not expanded. Must be the last argument on
     * its branch.
     */
    static NotNull<ArgumentType> rawText() { return make(ArgumentKind::RawText); }

    /** A target selector resolving to exactly one player. */
    static NotNull<ArgumentType> player() { return make(ArgumentKind::Player); }

    /** A target selector resolving to any number of players. */
    static NotNull<ArgumentType> players() { return make(ArgumentKind::Players); }

    /** A target selector resolving to exactly one actor. */
    static NotNull<ArgumentType> entity() { return make(ArgumentKind::Entity); }

    /** A target selector resolving to any number of actors. */
    static NotNull<ArgumentType> entities() { return make(ArgumentKind::Entities); }

    /** A block position, accepting relative (`~`) and local (`^`) coordinates. */
    static NotNull<ArgumentType> blockPosition() { return make(ArgumentKind::BlockPosition); }

    /** A precise position, accepting relative (`~`) and local (`^`) coordinates. */
    static NotNull<ArgumentType> position() { return make(ArgumentKind::Position); }

    /** A block type, completed from the block registry. */
    static NotNull<ArgumentType> blockType() { return make(ArgumentKind::BlockType); }

    /** A list of block states, such as `["wood_type"="birch"]`. */
    static NotNull<ArgumentType> blockStates() { return make(ArgumentKind::BlockStates); }

    /** An actor type, completed from the entity registry. */
    static NotNull<ArgumentType> entityType() { return make(ArgumentKind::EntityType); }

    /** A JSON object. */
    static NotNull<ArgumentType> json() { return make(ArgumentKind::Json); }

    /** An integer range, such as `3`, `3..`, `..7` or `3..7`. */
    static NotNull<ArgumentType> integerRange() { return make(ArgumentKind::IntegerRange); }

    /**
     * One of a fixed set of values.
     *
     * The set is validated by the server and is fixed once the command is registered. Use
     * softEnum() for a set that changes while the server is running.
     *
     * @param name Name of the value set, shown to the player when the set is too large to inline
     * @param values Accepted values
     */
    static NotNull<ArgumentType> enumeration(std::string name, std::vector<std::string> values)
    {
        return std::make_shared<BuiltinArgumentType>(ArgumentKind::Enumeration, std::move(name), std::move(values));
    }

    /**
     * A string completed from a named set of suggestions that can change at runtime.
     *
     * Update the set with CommandMap::setSuggestions(). The values are only a hint to the client:
     * the server accepts any string, so validate the value in your handler.
     *
     * @param name Name of the suggestion set
     */
    static NotNull<ArgumentType> softEnum(std::string name)
    {
        return std::make_shared<BuiltinArgumentType>(ArgumentKind::SoftEnum, std::move(name));
    }

private:
    static NotNull<ArgumentType> make(ArgumentKind kind) { return std::make_shared<BuiltinArgumentType>(kind); }
};

}  // namespace endstone
