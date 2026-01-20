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
#include <stdexcept>
#include <string>
#include <vector>

#include "endstone/command/command_sender.h"
#include "endstone/core/actor/actor.h"
#include "endstone/level/location.h"

namespace endstone {
/**
 * @brief Represents a base actor in the level.
 */
class Actor : public CommandSender {
public:
    /**
     * @brief Get the type of the actor.
     *
     * This method returns the type of the actor as a string, for example, minecraft:pig.
     *
     * @return The type of the actor.
     */
    [[nodiscard]] ActorType getType() const { return impl().getType(); }

    /**
     * Returns the runtime id for this actor
     *
     * @return Runtime id for this actor
     */
    [[nodiscard]] std::uint64_t getRuntimeId() const { return impl().getRuntimeId(); }

    /**
     * Gets the actor's current position
     *
     * @return a new copy of Location containing the position of this actor
     */
    [[nodiscard]] Location getLocation() const { return impl().getLocation(); }

    /**
     * Gets this actor's current velocity
     *
     * @return Current traveling velocity of this actor
     */
    [[nodiscard]] Vector getVelocity() const { return impl().getVelocity(); }

    /**
     * Returns true if the actor is supported by a block.
     *
     * @return True if actor is on ground.
     */
    [[nodiscard]] bool isOnGround() const { return impl().isOnGround(); }

    /**
     * Returns true if the actor is in water.
     *
     * @return True if the actor is in water.
     */
    [[nodiscard]] bool isInWater() const { return impl().isInWater(); }

    /**
     * Returns true if the actor is in lava.
     *
     * @return True if the actor is in lava.
     */
    [[nodiscard]] bool isInLava() const { return impl().isInLava(); }

    /**
     * Gets the current Level this actor resides in
     *
     * @return The current Level this actor resides in
     */
    [[nodiscard]] Level getLevel() const { return impl().getLevel(); }

    /**
     * Gets the current Dimension this actor resides in
     *
     * @return The current Dimension this actor resides in
     */
    [[nodiscard]] Dimension getDimension() const { return impl().getDimension(); }

    /**
     * @return Sets the actor's rotation.
     * <p>
     * Note that if the actor is affected by AI, it may override this rotation.
     *
     * @param yaw Rotation around the up axis (Y axis)
     * @param pitch Rotation around the right axis (X axis)
     */
    void setRotation(float yaw, float pitch) { impl().setRotation(yaw, pitch); }

    /**
     * @return Teleports this actor to the given location.
     *
     * @param location New location to teleport this actor to
     */
    bool teleport(Location location) { return impl().teleport(location); }

    /**
     * Teleports this actor to the target Actor.
     *
     * @param target Actor to teleport this actor to
     */
    bool teleport(const Actor &target) { return impl().teleport(target); }

    /**
     * @brief Returns a unique id for this actor
     *
     * @return Actor id
     */
    [[nodiscard]] std::int64_t getId() const { return impl().getId(); }

    /**
     * Remove this actor from the level.
     *
     * If you are trying to remove a Player, use Player::kick instead.
     */
    void remove() { impl().remove(); }

    /**
     * @brief Returns true if this actor has been marked for removal.
     *
     * @return True if it is dead.
     */
    [[nodiscard]] bool isDead() const { return impl().isDead(); }

    /**
     * Returns false if the entity has died, been despawned for some other reason, or has not been added to the level.
     *
     * @return True if valid.
     */
    [[nodiscard]] bool isValid() const { return !impl_.expired() && impl().isValid(); }

    /**
     * @brief Returns a list of scoreboard tags for this actor.
     *
     * @return a list of scoreboard tags for this actor
     */
    [[nodiscard]] std::vector<std::string> getScoreboardTags() const { return impl().getScoreboardTags(); }

    /**
     * @brief Adds a tag to this actor.
     *
     * @param tag the tag to add
     * @return true if the tag was successfully added, false if the tag already exists.
     */
    [[nodiscard]] bool addScoreboardTag(std::string tag) const { return impl().addScoreboardTag(std::move(tag)); }

    /**
     * @brief Removes a given tag from this actor.
     *
     * @param tag the tag to remove
     * @return true if the tag was successfully removed, false if the tag does not exist.
     */
    [[nodiscard]] bool removeScoreboardTag(std::string tag) const { return impl().removeScoreboardTag(std::move(tag)); }

    /**
     * @brief Checks if the actor's name tag is currently visible.
     *
     * @return True if the name tag is visible, false otherwise.
     */
    [[nodiscard]] bool isNameTagVisible() const { return impl().isNameTagVisible(); }

    /**
     * @brief Sets if the actor's name tag is visible or not.
     *
     * @param visible True to make the name tag visible, false to hide it.
     */
    void setNameTagVisible(bool visible) { impl().setNameTagVisible(visible); }

    /**
     * @brief Checks if the actor's name tag is always visible.
     *
     * @return True if the name tag is always visible, false otherwise.
     */
    [[nodiscard]] bool isNameTagAlwaysVisible() const { return impl().isNameTagAlwaysVisible(); }

    /**
     * @brief Sets whether the actor's name tag should always be visible.
     *
     * @param visible True to make the name tag always visible, false to disable always visibility.
     */
    void setNameTagAlwaysVisible(bool visible) { impl().setNameTagAlwaysVisible(visible); }

    /**
     * @brief Gets the current name tag of the actor.
     *
     * @return The name tag.
     */
    [[nodiscard]] std::string getNameTag() const { return impl().getNameTag(); }

    /**
     * @brief Sets the name tag for the actor.
     *
     * @param name The new name tag to set.
     */
    void setNameTag(std::string name) { impl().setNameTag(std::move(name)); }

    /**
     * @brief Gets the current score tag of the actor.
     *
     * @return The score tag.
     */
    [[nodiscard]] std::string getScoreTag() const { return impl().getScoreTag(); }

    /**
     * @brief Sets the score tag for the actor.
     *
     * @param score The new score tag to set.
     */
    void setScoreTag(std::string score) { impl().setScoreTag(std::move(score)); }

protected:
    template <typename T = core::EndstoneActor>
    [[nodiscard]] const T &impl() const
    {
        if (impl_.expired()) {
            throw std::runtime_error("Actor has been removed from the level.");
        }
        return *static_cast<const T *>(impl_.lock().get());
    }

    template <typename T = core::EndstoneActor>
    T &impl()
    {
        if (impl_.expired()) {
            throw std::runtime_error("Actor has been removed from the level.");
        }
        return *static_cast<T *>(impl_.lock().get());
    }

private:
    std::weak_ptr<core::EndstoneActor> impl_;
};
}  // namespace endstone

template <>
struct fmt::formatter<endstone::Actor> : formatter<string_view> {
    using Type = endstone::Actor;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "{}", val.getName());
    }
};
