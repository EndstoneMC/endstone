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

#include "endstone/actor/actor_type.h"
#include "endstone/block/tile_state.h"

namespace endstone {

/**
 * Represents a captured state of a creature spawner.
 */
class CreatureSpawner : public TileState {
public:
    /**
     * Gets the type of actor this spawner will spawn.
     *
     * @return the type of actor
     */
    [[nodiscard]] virtual const ActorType &getSpawnedType() const = 0;

    /**
     * Sets the type of actor this spawner will spawn.
     *
     * @param type the type of actor (e.g. `minecraft:zombie`)
     */
    virtual void setSpawnedType(ActorTypeId type) = 0;

    /**
     * Gets the delay until the spawner spawns the next batch of actors.
     *
     * @return the delay, in ticks
     */
    [[nodiscard]] virtual int getDelay() const = 0;

    /**
     * Sets the delay until the spawner spawns the next batch of actors.
     *
     * @param delay the delay, in ticks
     */
    virtual void setDelay(int delay) = 0;

    /**
     * Gets the minimum delay the spawner will wait between spawns.
     *
     * @return the minimum delay, in ticks
     */
    [[nodiscard]] virtual int getMinSpawnDelay() const = 0;

    /**
     * Sets the minimum delay the spawner will wait between spawns.
     *
     * @param delay the minimum delay, in ticks
     */
    virtual void setMinSpawnDelay(int delay) = 0;

    /**
     * Gets the maximum delay the spawner will wait between spawns.
     *
     * @return the maximum delay, in ticks
     */
    [[nodiscard]] virtual int getMaxSpawnDelay() const = 0;

    /**
     * Sets the maximum delay the spawner will wait between spawns.
     *
     * @param delay the maximum delay, in ticks
     */
    virtual void setMaxSpawnDelay(int delay) = 0;

    /**
     * Gets how many actors the spawner attempts to spawn at a time.
     *
     * @return the number of actors
     */
    [[nodiscard]] virtual int getSpawnCount() const = 0;

    /**
     * Sets how many actors the spawner attempts to spawn at a time.
     *
     * @param count the number of actors
     */
    virtual void setSpawnCount(int count) = 0;

    /**
     * Gets the maximum number of similar actors allowed nearby before the spawner stops spawning.
     *
     * @return the maximum number of similar actors
     */
    [[nodiscard]] virtual int getMaxNearbyEntities() const = 0;

    /**
     * Sets the maximum number of similar actors allowed nearby before the spawner stops spawning.
     *
     * @param count the maximum number of similar actors
     */
    virtual void setMaxNearbyEntities(int count) = 0;

    /**
     * Gets how far away a player must be for the spawner to be active.
     *
     * @return the required player range, in blocks
     */
    [[nodiscard]] virtual int getRequiredPlayerRange() const = 0;

    /**
     * Sets how far away a player must be for the spawner to be active.
     *
     * @param range the required player range, in blocks
     */
    virtual void setRequiredPlayerRange(int range) = 0;

    /**
     * Gets the radius around the spawner in which actors are spawned.
     *
     * @return the spawn range, in blocks
     */
    [[nodiscard]] virtual int getSpawnRange() const = 0;

    /**
     * Sets the radius around the spawner in which actors are spawned.
     *
     * @param range the spawn range, in blocks
     */
    virtual void setSpawnRange(int range) = 0;
};

}  // namespace endstone
