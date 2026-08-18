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

#include "bedrock/world/level/block/actor/mob_spawner_block_actor.h"
#include "endstone/block/creature_spawner.h"
#include "endstone/core/block/block_state.h"

namespace endstone::core {

class EndstoneCreatureSpawner : public EndstoneBlockStateBase<CreatureSpawner> {
public:
    EndstoneCreatureSpawner(const EndstoneBlock &block, ::MobSpawnerBlockActor &spawner, bool use_snapshot);

    [[nodiscard]] const ActorType &getSpawnedType() const override;
    void setSpawnedType(ActorTypeId type) override;
    [[nodiscard]] int getDelay() const override;
    void setDelay(int delay) override;
    [[nodiscard]] int getMinSpawnDelay() const override;
    void setMinSpawnDelay(int delay) override;
    [[nodiscard]] int getMaxSpawnDelay() const override;
    void setMaxSpawnDelay(int delay) override;
    [[nodiscard]] int getSpawnCount() const override;
    void setSpawnCount(int count) override;
    [[nodiscard]] int getMaxNearbyEntities() const override;
    void setMaxNearbyEntities(int count) override;
    [[nodiscard]] int getRequiredPlayerRange() const override;
    void setRequiredPlayerRange(int range) override;
    [[nodiscard]] int getSpawnRange() const override;
    void setSpawnRange(int range) override;

private:
    ::MobSpawnerBlockActor &spawner_;
};

}  // namespace endstone::core
