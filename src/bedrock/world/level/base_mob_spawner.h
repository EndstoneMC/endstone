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
#include <utility>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/entity/weak_entity_ref.h"
#include "bedrock/world/actor/actor_definition_identifier.h"

class BlockSource;
class SpawnData;

class BaseMobSpawner {
public:
    virtual ~BaseMobSpawner() = default;

    void setEntityId(ActorDefinitionIdentifier actor_def_id)
    {
        display_entity_ = nullptr;
        actor_def_id_ = std::move(actor_def_id);
        if (!actor_def_id_.getNamespace().empty() || !actor_def_id_.getIdentifier().empty()) {
            active_ = true;
        }
    }

    void setSpawnDelay(int spawn_delay) { spawn_delay_ = spawn_delay; }

    // Endstone
    [[nodiscard]] const ActorDefinitionIdentifier &getEntityId() const { return actor_def_id_; }
    [[nodiscard]] int getSpawnDelay() const { return spawn_delay_; }
    [[nodiscard]] int getMinSpawnDelay() const { return min_spawn_delay_; }
    void setMinSpawnDelay(int value) { min_spawn_delay_ = value; }
    [[nodiscard]] int getMaxSpawnDelay() const { return max_spawn_delay_; }
    void setMaxSpawnDelay(int value) { max_spawn_delay_ = value; }
    [[nodiscard]] int getSpawnCount() const { return spawn_count_; }
    void setSpawnCount(int value) { spawn_count_ = value; }
    [[nodiscard]] int getMaxNearbyEntities() const { return max_nearby_entities_; }
    void setMaxNearbyEntities(int value) { max_nearby_entities_ = value; }
    [[nodiscard]] int getRequiredPlayerRange() const { return required_player_range_; }
    void setRequiredPlayerRange(int value) { required_player_range_ = value; }
    [[nodiscard]] int getSpawnRange() const { return spawn_range_; }
    void setSpawnRange(int value) { spawn_range_ = value; }
    [[nodiscard]] bool isActive() const { return active_; }
    // Endstone
    void delay(BlockSource &source) { _delay(source); }

protected:
    void _delay(BlockSource &source);

private:
    int spawn_delay_;
    float spin_;
    float o_spin_;
    ActorDefinitionIdentifier actor_def_id_;
    std::vector<SpawnData> spawn_potentials_;
    std::unique_ptr<SpawnData> next_spawn_data_;
    int min_spawn_delay_;
    int max_spawn_delay_;
    int spawn_count_;
    WeakEntityRef display_entity_;
    int max_nearby_entities_;
    int required_player_range_;
    int spawn_range_;
    bool active_;
    float display_entity_width_;
    float display_entity_height_;
    float display_entity_scale_;
};
BEDROCK_STATIC_ASSERT_SIZE(BaseMobSpawner, 304, 264);
