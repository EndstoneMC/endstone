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
#include <string>
#include <utility>

#include "endstone/command/brigadier/argument/actor_argument_type.h"
#include "endstone/command/brigadier/argument/block_position_argument_type.h"
#include "endstone/command/brigadier/argument/dimension_argument_type.h"
#include "endstone/command/brigadier/argument/fine_position_argument_type.h"
#include "endstone/command/brigadier/argument/player_argument_type.h"
#include "endstone/command/brigadier/argument/resource_argument_type.h"
#include "endstone/command/brigadier/argument/soft_enum_argument_type.h"

namespace endstone {

class ActorType;
class BlockType;
class ItemType;

/**
 * The argument types Endstone adds to Brigadier's own.
 */
class ArgumentTypes {
public:
    ArgumentTypes() = delete;

    /**
     * A selector or player name for exactly one player, resolved through a PlayerSelectorArgumentResolver.
     *
     * @return the argument type
     */
    static std::shared_ptr<const PlayerArgumentType> player()
    {
        return std::make_shared<const PlayerArgumentType>(true);
    }

    /**
     * A selector or player name for any number of players, resolved through a PlayerSelectorArgumentResolver.
     *
     * @return the argument type
     */
    static std::shared_ptr<const PlayerArgumentType> players()
    {
        return std::make_shared<const PlayerArgumentType>(false);
    }

    /**
     * A selector or player name for exactly one actor, resolved through an ActorSelectorArgumentResolver.
     *
     * @return the argument type
     */
    static std::shared_ptr<const ActorArgumentType> actor() { return std::make_shared<const ActorArgumentType>(true); }

    /**
     * A selector or player name for any number of actors, resolved through an ActorSelectorArgumentResolver.
     *
     * @return the argument type
     */
    static std::shared_ptr<const ActorArgumentType> actors()
    {
        return std::make_shared<const ActorArgumentType>(false);
    }

    /**
     * A block position, resolved through a BlockPositionResolver.
     *
     * @return the argument type
     */
    static std::shared_ptr<const BlockPositionArgumentType> blockPosition()
    {
        return std::make_shared<const BlockPositionArgumentType>();
    }

    /**
     * A fine position, resolved through a FinePositionResolver.
     *
     * @return the argument type
     */
    static std::shared_ptr<const FinePositionArgumentType> finePosition()
    {
        return std::make_shared<const FinePositionArgumentType>();
    }

    /**
     * A dimension, whose value is a NotNull<Dimension>.
     *
     * @return the argument type
     */
    static std::shared_ptr<const DimensionArgumentType> dimension()
    {
        return std::make_shared<const DimensionArgumentType>();
    }

    /**
     * An entry of a registry, whose value is a `const T *` that is never null.
     *
     * @tparam T the registry type, which is BlockType, ItemType or ActorType
     * @return the argument type
     */
    template <typename T>
        requires std::same_as<T, BlockType> || std::same_as<T, ItemType> || std::same_as<T, ActorType>
    static std::shared_ptr<const ResourceArgumentType> resource()
    {
        if constexpr (std::same_as<T, BlockType>) {
            return std::make_shared<const ResourceArgumentType>("BlockType");
        }
        else if constexpr (std::same_as<T, ItemType>) {
            return std::make_shared<const ResourceArgumentType>("ItemType");
        }
        else {
            return std::make_shared<const ResourceArgumentType>("ActorType");
        }
    }

    /**
     * An argument completed from a soft enum, whose value is the word typed as a string.
     *
     * The server accepts any word, so the command checks the value itself. The name must be a registered soft enum.
     * Soft enums are named for their content in CamelCase, as the server names its own.
     *
     * @param name the name of a soft enum a plugin, Endstone or the server owns
     * @return the argument type
     */
    static std::shared_ptr<const SoftEnumArgumentType> softEnum(std::string name)
    {
        return std::make_shared<const SoftEnumArgumentType>(std::move(name));
    }
};

}  // namespace endstone
