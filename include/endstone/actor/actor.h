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

#include "endstone/command/command_sender.h"
#include "endstone/level/location.h"
#include "endstone/level/position.h"

namespace endstone {

/**
 * @brief Represents a base actor in the level.
 */
class Actor : public CommandSender {
public:
    // CommandSender
    [[nodiscard]] Actor *asActor() const override
    {
        return const_cast<Actor *>(this);
    }

    /**
     * @brief Get the type of the actor.
     *
     * This method returns the type of the actor as a string, for example, minecraft:pig.
     *
     * @return The type of the actor.
     */
    [[nodiscard]] virtual std::string getType() const = 0;

    /**
     * Returns the runtime id for this actor
     *
     * @return Runtime id for this actor
     */
    [[nodiscard]] virtual std::uint64_t getRuntimeId() const = 0;

    /**
     * Gets the actor's current position
     *
     * @return a new copy of Location containing the position of this actor
     */
    [[nodiscard]] virtual Location getLocation() const = 0;

    /**
     * Gets this actor's current velocity
     *
     * @return Current traveling velocity of this actor
     */
    [[nodiscard]] virtual Vector<float> getVelocity() const = 0;

    /**
     * Returns true if the actor is supported by a block.
     *
     * @return True if actor is on ground.
     */
    [[nodiscard]] virtual bool isOnGround() const = 0;

    /**
     * Returns true if the actor is in water.
     *
     * @return True if the actor is in water.
     */
    [[nodiscard]] virtual bool isInWater() const = 0;

    /**
     * Returns true if the actor is in lava.
     *
     * @return True if the actor is in lava.
     */
    [[nodiscard]] virtual bool isInLava() const = 0;

    /**
     * Gets the current Level this actor resides in
     *
     * @return The current Level this actor resides in
     */
    [[nodiscard]] virtual Level &getLevel() const = 0;

    /**
     * Gets the current Dimension this actor resides in
     *
     * @return The current Dimension this actor resides in
     */
    [[nodiscard]] virtual Dimension &getDimension() const = 0;

    /**
     * @return Sets the actor's rotation.
     * <p>
     * Note that if the actor is affected by AI, it may override this rotation.
     *
     * @param yaw Rotation around the up axis (Y axis)
     * @param pitch Rotation around the right axis (X axis)
     */
    virtual void setRotation(float yaw, float pitch) = 0;

    /**
     * @return Teleports this actor to the given location.
     *
     * @param location New location to teleport this actor to
     */
    virtual void teleport(Location location) = 0;

    /**
     * Teleports this actor to the target Actor.
     *
     * @param target Actor to teleport this actor to
     */
    virtual void teleport(Actor &target) = 0;

    /**
     * @brief Returns a unique id for this actor
     *
     * @return Actor id
     */
    [[nodiscard]] virtual std::int64_t getId() const = 0;

    /**
     * Remove this actor from the level.
     *
     * If you are trying to remove a Player, use Player::kick instead.
     */
    virtual void remove() = 0;

    /**
     * @brief Returns true if this actor has been marked for removal.
     *
     * @return True if it is dead.
     */
    [[nodiscard]] virtual bool isDead() const = 0;

    /**
     * Returns false if the entity has died, been despawned for some other reason, or has not been added to the level.
     *
     * @return True if valid.
     */
    [[nodiscard]] virtual bool isValid() const = 0;

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
     * @brief Returns a list of scoreboard tags for this actor.
     *
     * @return a list of scoreboard tags for this actor
     */
    [[nodiscard]] virtual std::vector<std::string> getScoreboardTags() const = 0;

    /**
     * @brief Adds a tag to this actor.
     *
     * @param tag the tag to add
     * @return true if the tag was successfully added, false if the tag already exists.
     */
    [[nodiscard]] virtual bool addScoreboardTag(std::string tag) const = 0;

    /**
     * @brief Removes a given tag from this actor.
     *
     * @param tag the tag to remove
     * @return true if the tag was successfully removed, false if the tag does not exist.
     */
    [[nodiscard]] virtual bool removeScoreboardTag(std::string tag) const = 0;

    /**
     * @brief Checks if the actor's name tag is currently visible.
     *
     * @return True if the name tag is visible, false otherwise.
     */
    [[nodiscard]] virtual bool isNameTagVisible() const = 0;

    /**
     * @brief Sets if the actor's name tag is visible or not.
     *
     * @param visible True to make the name tag visible, false to hide it.
     */
    virtual void setNameTagVisible(bool visible) = 0;

    /**
     * @brief Checks if the actor's name tag is always visible.
     *
     * @return True if the name tag is always visible, false otherwise.
     */
    [[nodiscard]] virtual bool isNameTagAlwaysVisible() const = 0;

    /**
     * @brief Sets whether the actor's name tag should always be visible.
     *
     * @param visible True to make the name tag always visible, false to disable always visibility.
     */
    virtual void setNameTagAlwaysVisible(bool visible) = 0;

    /**
     * @brief Gets the current name tag of the actor.
     *
     * @return The name tag.
     */
    [[nodiscard]] virtual std::string getNameTag() const = 0;

    /**
     * @brief Sets the name tag for the actor.
     *
     * @param name The new name tag to set.
     */
    virtual void setNameTag(std::string name) = 0;

    /**
     * @brief Gets the current score tag of the actor.
     *
     * @return The score tag.
     */
    [[nodiscard]] virtual std::string getScoreTag() const = 0;

    /**
     * @brief Sets the score tag for the actor.
     *
     * @param score The new score tag to set.
     */
    virtual void setScoreTag(std::string score) = 0;
};

}  // namespace endstone

namespace fmt {
template <>
struct formatter<endstone::Actor> : formatter<string_view> {
    using Type = endstone::Actor;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "{}", val.getName());
    }
};
}  // namespace fmt
