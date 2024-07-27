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
    Actor() = default;
    Actor(const Actor &) = delete;
    Actor &operator=(const Actor &) = delete;
    Actor(Actor &&) = delete;
    Actor &operator=(Actor &&) = delete;

    ~Actor() override = default;

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
     * @brief Returns a unique id for this actor
     *
     * @return Actor id
     */
    [[nodiscard]] virtual std::int64_t getId() const = 0;

    /**
     * @brief Returns true if this actor has been marked for removal.
     *
     * @return True if it is dead.
     */
    [[nodiscard]] virtual bool isDead() const = 0;
};

}  // namespace endstone
