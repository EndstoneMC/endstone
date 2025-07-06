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

#include "bedrock/world/level/block/vanilla_block_type_ids.h"

namespace VanillaBlockTypeIds {
static const HashedString Bed = "minecraft:bed";
static const HashedString BeeNest = "minecraft:bee_nest";
static const HashedString Beehive = "minecraft:beehive";
static const HashedString CampFire = "minecraft:campfire";
static const HashedString EndPortal = "minecraft:end_portal";
static const HashedString EndPortalFrame = "minecraft:end_portal_frame";
static const HashedString SoulCampfire = "minecraft:soul_campfire";
static const HashedString Tnt = "minecraft:tnt";
static const HashedString UnderwaterTnt = "minecraft:underwater_tnt";
static const HashedString WitherRose = "minecraft:wither_rose";
}  // namespace VanillaBlockTypeIds

namespace VanillaBlockTypeGroups {
static const std::vector<std::reference_wrapper<const HashedString>> TntIds = {VanillaBlockTypeIds::Tnt,
                                                                               VanillaBlockTypeIds::UnderwaterTnt};
}  // namespace VanillaBlockTypeGroups
