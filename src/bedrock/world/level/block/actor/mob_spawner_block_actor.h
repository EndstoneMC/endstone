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

#include "bedrock/bedrock.h"
#include "bedrock/world/level/base_mob_spawner.h"
#include "bedrock/world/level/block/actor/vanilla_block_actor.h"

class MobSpawnerBlockActor : public VanillaBlockActor {
public:
    static constexpr BlockActorType TypeId = BlockActorType::MobSpawner;

    BaseMobSpawner &getSpawner() { return *spawner_; }
    [[nodiscard]] const BaseMobSpawner &getSpawner() const { return *spawner_; }

private:
    std::unique_ptr<BaseMobSpawner> spawner_;
};
BEDROCK_STATIC_ASSERT_SIZE(MobSpawnerBlockActor, 248, 224);
