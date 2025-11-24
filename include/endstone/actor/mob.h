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

#include "endstone/actor/actor.h"

namespace endstone {
/**
 * @brief Represents a mobile entity (i.e. living entity), such as a monster or player.
 */
class Mob : public Actor {
public:
    /**
     * @brief Checks to see if an actor is gliding, such as using an Elytra.
     * @return True if this actor is gliding.
     */
    [[nodiscard]] virtual bool isGliding() const = 0;

    /**
     * @brief Gets the entity's health from 0 to its max possible value, where 0 is dead.
     *
     * @return Health represented from 0 to max
     */
    [[nodiscard]] virtual int getHealth() const = 0;

    /**
     * Sets the entity's health from 0 to its possible value, where 0 is dead.
     *
     * @param health New health represented from 0 to max
     */
    [[nodiscard]] virtual Result<void> setHealth(int health) const = 0;

    /**
     * @brief Gets the maximum health this entity has.
     *
     * @return Maximum health
     */
    [[nodiscard]] virtual int getMaxHealth() const = 0;

    /**
     * @brief Sets the maximum health this entity can have.
     *
     * If the health of the entity is above the value provided, it will be set to that value.
     *
     * @note An entity with a health bar e.g., Player, EnderDragon, Wither, etc. will have their bar scaled accordingly.
     *
     * @param health amount of health to set the maximum to
     */
    [[nodiscard]] virtual Result<void> setMaxHealth(int health) const = 0;
};
}  // namespace endstone
