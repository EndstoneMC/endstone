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
#include <optional>
#include <vector>

#include "endstone/actor/actor.h"
#include "endstone/attribute/attribute_instance.h"
#include "endstone/potion/effect.h"

namespace endstone {
/**
 * Represents a mobile entity (i.e. living entity), such as a monster or player.
 */
class Mob : public Actor {
public:
    /**
     * Checks to see if an actor is gliding, such as using an Elytra.
     * @return True if this actor is gliding.
     */
    [[nodiscard]] virtual bool isGliding() const = 0;

    /**
     * Gets the entity's health from 0 to its max possible value, where 0 is dead.
     *
     * @return Health represented from 0 to max
     */
    [[nodiscard]] virtual int getHealth() const = 0;

    /**
     * Sets the entity's health from 0 to its possible value, where 0 is dead.
     *
     * @param health New health represented from 0 to max
     */
    virtual void setHealth(int health) const = 0;

    /**
     * Gets the maximum health this entity has.
     *
     * @return Maximum health
     */
    [[nodiscard]] virtual int getMaxHealth() const = 0;

    /**
     * Sets the maximum health this entity can have.
     *
     * If the health of the entity is above the value provided, it will be set to that value.
     *
     * @note An entity with a health bar e.g., Player, EnderDragon, Wither, etc. will have their bar scaled accordingly.
     *
     * @param health amount of health to set the maximum to
     */
    virtual void setMaxHealth(int health) const = 0;

    /**
     * Checks whether the given attribute is present on the object.
     *
     * @param id The attribute to check
     */
    [[nodiscard]] virtual bool hasAttribute(AttributeId id) const = 0;

    /**
     * Gets the specified attribute instance from the object. This instance will be backed directly to the object and
     * any changes will be visible at once.
     *
     * @param id The attribute to get
     */
    [[nodiscard]] virtual std::unique_ptr<AttributeInstance> getAttribute(AttributeId id) = 0;

    /**
     * Gets all attribute instances from the object. This instance will be backed directly to the object and any
     * changes will be visible at once.
     */
    [[nodiscard]] virtual std::vector<std::unique_ptr<AttributeInstance>> getAttributes() = 0;

    /**
     * Adds the given Effect to this entity.
     *
     * Only one effect of any given type may be active at any one time. If this entity already has an effect of the
     * same type, it will be overwritten with the new one.
     *
     * @param effect Effect to be added
     */
    virtual void addEffect(const Effect &effect) = 0;

    /**
     * Removes any effects of the given type that are present on this entity.
     *
     * @param type the effect type to remove
     */
    virtual void removeEffect(EffectId type) = 0;

    /**
     * Returns whether the entity already has an existing effect of the given type applied to it.
     *
     * @param type the effect type to check
     * @return whether the entity has the given potion effect active
     */
    [[nodiscard]] virtual bool hasEffect(EffectId type) const = 0;

    /**
     * Returns the active Effect of the specified type.
     *
     * If the effect is not present on the entity then std::nullopt will be returned.
     *
     * @param type the effect type to check
     * @return the active effect, or std::nullopt if not present
     */
    [[nodiscard]] virtual std::optional<Effect> getEffect(EffectId type) const = 0;

    /**
     * Returns all currently active Effects on this entity.
     *
     * @return a vector of Effects
     */
    [[nodiscard]] virtual std::vector<Effect> getActiveEffects() const = 0;
};
}  // namespace endstone
