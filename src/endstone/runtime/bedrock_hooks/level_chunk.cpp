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

#include "bedrock/world/level/chunk/level_chunk.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <functional>
#include <variant>

#include "bedrock/symbol.h"
#include "bedrock/world/level/block/bedrock_block_names.h"
#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block/registry/block_type_registry.h"
#include "bedrock/world/level/level.h"
#include "endstone/runtime/hook.h"

namespace {
constexpr std::uint32_t MaxWarmedSamples = 256;

const Block *getAir(const Level &level)
{
    static const Block *air = [&level] {
        const Block *result = nullptr;
        level.getBlockTypeRegistry()->forEachBlockType([&result](const BlockType &block_type) {
            if (block_type.getName() == BedrockBlockNames::Air) {
                result = &block_type.getDefaultState();
            }
            return result == nullptr;
        });
        return result;
    }();
    return air;
}

void warmRandomTicks(const LevelChunk &chunk)
{
    static const auto *seed_global = BEDROCK_VAR(const std::uint32_t *, "LevelChunk::tickImpl::seed");

    auto &level = chunk.getLevel();
    const auto *rule = level.getGameRules().getRule(GameRuleId(GameRules::RANDOM_TICK_SPEED));
    const auto *speed = rule ? std::get_if<int>(&rule->getValue()) : nullptr;
    const auto *air = getAir(level);
    if (!speed || *speed <= 0 || !air) {
        return;
    }

    const auto sub_chunks = chunk.getAllSubChunks();
    int top = -1;
    for (auto i = static_cast<int>(sub_chunks.size()) - 1; i >= 0; --i) {
        const auto *standard = sub_chunks[i].blocks_read_ptr_[0];
        if (!standard) {
            return;
        }
        const auto *extra = sub_chunks[i].blocks_read_ptr_[1];
        if (!standard->isPaletteUniform(*air) || (extra && !extra->isPaletteUniform(*air))) {
            top = i;
            break;
        }
    }
    if (top < 0) {
        return;
    }

    const auto count = static_cast<std::uint32_t>(static_cast<int>(static_cast<float>(top + 1) * 2.5F));
    const auto total = std::min(count * static_cast<std::uint32_t>(*speed), MaxWarmedSamples);
    const auto height = (top + 1) * 16;

    std::array<const SubChunk *, MaxWarmedSamples> records;
    std::array<std::uint16_t, MaxWarmedSamples> indices;
    std::uint32_t n = 0;
    auto seed = *seed_global;
    for (std::uint32_t i = 0; i < total; ++i) {
        seed = seed * 3 + 0x3C6EF35F;
#ifdef _WIN32
        const auto shifted = static_cast<std::int32_t>(seed) >> 2;
        const auto value = static_cast<std::uint32_t>(shifted < 0 ? -shifted : shifted);
        const auto x = value & 15;
        const auto z = (value >> 8) & 15;
        const auto y = static_cast<std::uint32_t>(static_cast<int>((value >> 16) & 0xFFFF) % height);
#else
        const auto x = (seed >> 2) & 15;
        const auto z = (seed >> 10) & 15;
        const auto y = static_cast<std::uint32_t>(static_cast<int>((seed >> 18) & 0xFFFF) % height);
#endif
        if ((y >> 4) >= sub_chunks.size()) {
            continue;
        }
        records[n] = &sub_chunks[y >> 4];
        indices[n] = static_cast<std::uint16_t>((x << 8) | (z << 4) | (y & 15));
        __builtin_prefetch(&records[n]->blocks_read_ptr_[0]);
        ++n;
    }

    std::array<const SubChunkBlockStorage *, MaxWarmedSamples> storages;
    for (std::uint32_t k = 0; k < n; ++k) {
        storages[k] = records[k]->blocks_read_ptr_[0];
        if (storages[k]) {
            __builtin_prefetch(storages[k]);
        }
    }

    std::array<const Block *, MaxWarmedSamples> blocks;
    for (std::uint32_t k = 0; k < n; ++k) {
        blocks[k] = storages[k] ? &storages[k]->getElement(indices[k]) : nullptr;
    }

    for (std::uint32_t k = 0; k < n; ++k) {
        if (blocks[k]) {
            __builtin_prefetch(&blocks[k]->getBlockType().getEventManager());
        }
    }
}
}  // namespace

void LevelChunk::tickImpl(BlockSource &tick_region, const Tick &tick, std::function<void()> spawner_callback)
{
    // Vanilla runs the random-tick loop shortly after the spawner, so warm the lines it is about to read.
    ENDSTONE_HOOK_CALL_ORIGINAL(&LevelChunk::tickImpl, this, tick_region, tick,
                                std::function<void()>([this, &spawner_callback] {
                                    spawner_callback();
                                    warmRandomTicks(*this);
                                }));
}
