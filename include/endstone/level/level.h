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
#include <memory>
#include <string>
#include <vector>

#include "endstone/actor/actor.h"
#include "endstone/game_rule.h"
#include "endstone/inventory/recipe.h"
#include "endstone/level/dimension.h"
#include "endstone/level/dimension_creator.h"

namespace endstone {

/**
 * Represents a level, which may contain actors, chunks and blocks.
 */
class Level {
public:
    virtual ~Level() = default;

    /**
     * Gets the unique name of this level.
     *
     * @return Name of this level
     */
    [[nodiscard]] virtual std::string getName() const = 0;

    /**
     * Get a list of all actors in this level.
     *
     * @return A List of all actors currently residing in this level
     */
    [[nodiscard]] virtual std::vector<NotNull<Actor>> getActors() const = 0;

    /**
     * Gets the relative in-game time of this level.
     *
     * @return The current relative time
     */
    [[nodiscard]] virtual int getTime() const = 0;

    /**
     * Sets the relative in-game time on the server.
     *
     * @param time The new relative time to set the in-game time to
     */
    virtual void setTime(int time) = 0;

    /**
     * Gets a list of all dimensions within this level.
     *
     * @return a list of dimensions
     */
    [[nodiscard]] virtual std::vector<NotNull<Dimension>> getDimensions() const = 0;

    /**
     * Gets the dimension with the given id.
     *
     * @param id the id of the dimension to retrieve.
     *
     * @return The Dimension with the given id, or null if none exists
     */
    [[nodiscard]] virtual Nullable<Dimension> getDimension(DimensionId id) const = 0;

    /**
     * Creates a new custom dimension within this level.
     * <p>
     * The dimension is created from the options described by the given DimensionCreator. Custom dimensions are
     * empty (void) dimensions identified by a namespaced id, e.g. `myplugin:void_realm`; populate them with blocks,
     * structures or actors afterward.
     * <p>
     * The level remembers which id belongs to which name, so a dimension's terrain and actors survive a restart. The
     * registration itself does not: call this again on every startup to get the same dimension back.
     * <p>
     * If a dimension with the requested name already exists, that existing dimension is returned instead.
     *
     * @param creator the options to use when creating the dimension
     * @return the newly created (or existing) Dimension, or null if it could not be created
     */
    [[nodiscard]] virtual Nullable<Dimension> createDimension(const DimensionCreator &creator) = 0;

    /**
     * Gets the Seed for this level.
     *
     * @return This level's Seed
     */
    [[nodiscard]] virtual std::int64_t getSeed() const = 0;

    /**
     * Checks if a game rule exists.
     *
     * @param rule The Minecraft game rule to check
     * @return True if the game rule exists
     */
    [[nodiscard]] virtual bool _hasGameRule(Identifier<GameRule> rule) const = 0;

    /**
     * Checks if a game rule exists.
     *
     * @tparam T The type of the game rule's value.
     * @param rule The Minecraft game rule to check
     * @return True if the game rule exists
     */
    template <typename T>
    [[nodiscard]] bool hasGameRule(GameRuleId<T> rule) const
    {
        return _hasGameRule(rule);
    }

    /**
     * Gets the value of a game rule.
     *
     * @param rule The Minecraft game rule to get
     * @return The current game rule value
     */
    [[nodiscard]] virtual GameRuleValue _getGameRule(Identifier<GameRule> rule) const = 0;

    /**
     * Gets the value of a game rule.
     *
     * @tparam T The type of the game rule's value.
     * @param rule The Minecraft game rule to get
     * @return The current game rule value
     */
    template <typename T>
    [[nodiscard]] T getGameRule(GameRuleId<T> rule) const
    {
        return std::get<T>(_getGameRule(rule));
    }

    /**
     * Sets the value of a game rule.
     *
     * @param rule The Minecraft game rule to set
     * @param value The new value
     * @return True if the value was accepted
     */
    virtual bool _setGameRule(Identifier<GameRule> rule, GameRuleValue value) = 0;

    /**
     * Sets the value of a game rule.
     *
     * @tparam T The type of the game rule's value.
     * @param rule The Minecraft game rule to set
     * @param value The new value
     * @return True if the value was accepted
     */
    template <typename T>
    bool setGameRule(GameRuleId<T> rule, T value)
    {
        return _setGameRule(rule, value);
    }

    /**
     * Get the list of crafting recipes.
     *
     * @return a list of recipes
     */
    [[nodiscard]] virtual std::vector<NotNull<Recipe>> getRecipes() const = 0;
};

}  // namespace endstone
