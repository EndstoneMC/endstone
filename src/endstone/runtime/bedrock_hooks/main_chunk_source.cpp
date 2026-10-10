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

#include "bedrock/world/level/chunk/main_chunk_source.h"

#include <ranges>
#include <unordered_map>
#include <vector>

#include "bedrock/util/random.h"

namespace {
struct ChunkSnapshot {
    std::vector<ChunkPos> positions;
    int picks_left = 0;
};
std::unordered_map<const MainChunkSource *, ChunkSnapshot> snapshots;
}  // namespace

std::shared_ptr<LevelChunk> MainChunkSource::getRandomChunk(Random &random)
{
    // #blameMojang - autosave picks up to 20 random chunks per tick, and each pick walks the chunk map's
    // node list to the drawn index, tens of thousands of nodes with spread players.
    // Fix: index a snapshot of the keys, rebuilt every 400 picks.
    const auto &map = *getChunkMap();
    if (map.empty()) {
        return nullptr;
    }
    const auto index = static_cast<std::size_t>(random.nextInt(static_cast<int>(map.size())));
    auto &snapshot = snapshots[this];
    if (snapshot.picks_left-- <= 0 || snapshot.positions.empty()) {
        snapshot.positions.clear();
        for (const auto &pos : map | std::views::keys) {
            snapshot.positions.push_back(pos);
        }
        snapshot.picks_left = 400;
    }
    auto chunk = getExistingChunk(snapshot.positions[index * snapshot.positions.size() / map.size()]);
    if (!chunk || chunk->getState() < ChunkState::Loaded) {
        return nullptr;
    }
    return chunk;
}
