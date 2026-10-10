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

#include "bedrock/world/level/levelgen/v1/overworld_generator_multinoise.h"

#include <cstdint>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>

namespace {
std::shared_mutex mutex;
std::unordered_map<const OverworldGeneratorMultinoise *, std::unordered_map<std::int64_t, bool>> answers;
}  // namespace

bool OverworldGeneratorMultinoise::chunkPosNeedsBlending(const ChunkPos &cp)
{
    // #blameMojang - every view move asks this for each empty slot and up to 165 neighbours, and BDS wipes its own
    // caches whole when full, so most calls rebuild a blender and read LevelDB on the server thread.
    // Fix: keep each answer, since the blending inputs don't change while the server runs.
    {
        std::shared_lock lock(mutex);
        if (const auto it = answers.find(this); it != answers.end()) {
            if (const auto jt = it->second.find(cp.packed); jt != it->second.end()) {
                return jt->second;
            }
        }
    }
    const auto result = ENDSTONE_HOOK_CALL_ORIGINAL(&OverworldGeneratorMultinoise::chunkPosNeedsBlending, this, cp);
    std::unique_lock lock(mutex);
    answers[this].emplace(cp.packed, result);
    return result;
}

void OverworldGeneratorMultinoise::_clearBlendingCache()
{
    {
        std::unique_lock lock(mutex);
        answers.erase(this);
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&OverworldGeneratorMultinoise::_clearBlendingCache, this);
}
