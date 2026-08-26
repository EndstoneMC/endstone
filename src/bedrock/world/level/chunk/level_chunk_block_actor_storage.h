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
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <gsl/gsl>

#include "bedrock/bedrock.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/chunk/chunk_block_pos.h"

class BlockActor;
class IVanillaRenderBlockActorComponent;
class IVanillaTickBlockActorComponent;

class LevelChunkBlockActorStorage {
public:
    struct VanillaTickBlockActorComponent {
        ChunkBlockPos chunk_block_pos;
        IVanillaTickBlockActorComponent *component;
    };
    using Map = std::unordered_map<ChunkBlockPos, std::shared_ptr<BlockActor>>;

    [[nodiscard]] Map::const_iterator begin() const { return map_.begin(); }
    [[nodiscard]] Map::const_iterator end() const { return map_.end(); }

private:
    bool track_permanently_rendered_;
    Map map_;
    std::vector<gsl::not_null<IVanillaRenderBlockActorComponent *>> vanilla_render_block_actor_components_;
    std::unordered_set<BlockPos> permanent_rendering_locations_;
    std::vector<VanillaTickBlockActorComponent> vanilla_tick_block_actor_components_;
};
BEDROCK_STATIC_ASSERT_SIZE(LevelChunkBlockActorStorage, 184, 136);
