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

#include "bedrock/world/level/block/fire_block.h"

#include "bedrock/world/level/block/campfire_block.h"
#include "bedrock/world/level/block/states/vanilla_block_states.h"
#include "bedrock/world/level/block/vanilla_block_type_ids.h"
#include "bedrock/world/level/dimension/dimension.h"
#include "bedrock/world/level/level.h"
#include "bedrock/world/level/weather.h"

// void FireBlock::checkBurn(BlockSource &region, const BlockPos &pos, int chance, Randomize &randomize, int age) const
// {
//     _tryEvictBeehive(region, pos);
//     const auto &block = region.getBlock(pos);
//     if (randomize.nextIntInclusive(0, chance - 1) >= block.getBurnOdds()) {
//         return;
//     }
//
//     if (block.getName() == VanillaBlockTypeIds::CampFire || block.getName() == VanillaBlockTypeIds::SoulCampfire) {
//         CampfireBlock::tryLightFire(region, pos, nullptr);
//         return;
//     }
//
//     if (!block.getBlockType().anyOf(VanillaBlockTypeGroups::TntIds)) {
//         // the block is NOT a TNT
//         // TODO(event): call BlockBurnEvent, return if cancelled
//         const auto &weather = region.getDimension().getWeather();
//         if (randomize.nextIntInclusive(0, age + 9) < 5 && !weather.isRainingAt(region, pos)) {
//             const auto new_age = std::min(age + (randomize.nextIntInclusive(0, 4) / 4), 15);
//             const auto block_state_with_age = getDefaultState().setState<int>(VanillaStateIds::Age, new_age);
//             region.removeBlock(pos);
//             if (isValidFireLocation(region, pos)) {
//                 region.setBlock(pos, *block_state_with_age, UPDATE_ALL, nullptr, nullptr);
//             }
//         }
//         else {
//             region.removeBlock(pos);
//         }
//     }
//     else {
//         // the block is a TNT
//         if (region.getLevel().getGameRules().getBool(GameRuleId(GameRules::DO_TNT_EXPLODE), false)) {
//             // TODO(event): call TNTPrimeEvent, return if cancelled
//             const auto exploded_block = block.setState<bool>(VanillaStateIds::ExplodeBit, true);
//             exploded_block->destroy(region, pos, nullptr);
//             region.removeBlock(pos);
//         }
//     }
// }
