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
const HashedString AcaciaLog = "minecraft:acacia_log";
const HashedString AcaciaWood = "minecraft:acacia_wood";
const HashedString Bed = "minecraft:bed";
const HashedString BeeNest = "minecraft:bee_nest";
const HashedString Beehive = "minecraft:beehive";
const HashedString BirchLog = "minecraft:birch_log";
const HashedString BirchWood = "minecraft:birch_wood";
const HashedString CampFire = "minecraft:campfire";
const HashedString CherryLog = "minecraft:cherry_log";
const HashedString CherryWood = "minecraft:cherry_wood";
const HashedString DarkOakLog = "minecraft:dark_oak_log";
const HashedString DarkOakWood = "minecraft:dark_oak_wood";
const HashedString EndPortal = "minecraft:end_portal";
const HashedString EndPortalFrame = "minecraft:end_portal_frame";
const HashedString JungleLog = "minecraft:jungle_log";
const HashedString JungleWood = "minecraft:jungle_wood";
const HashedString MangroveLog = "minecraft:mangrove_log";
const HashedString MangroveWood = "minecraft:mangrove_wood";
const HashedString OakLog = "minecraft:oak_log";
const HashedString OakWood = "minecraft:oak_wood";
const HashedString PaleOakLog = "minecraft:pale_oak_log";
const HashedString PaleOakWood = "minecraft:pale_oak_wood";
const HashedString SoulCampfire = "minecraft:soul_campfire";
const HashedString SpruceLog = "minecraft:spruce_log";
const HashedString SpruceWood = "minecraft:spruce_wood";
const HashedString StrippedAcaciaLog = "minecraft:stripped_acacia_log";
const HashedString StrippedAcaciaWood = "minecraft:stripped_acacia_wood";
const HashedString StrippedBirchLog = "minecraft:stripped_birch_log";
const HashedString StrippedBirchWood = "minecraft:stripped_birch_wood";
const HashedString StrippedCherryLog = "minecraft:stripped_cherry_log";
const HashedString StrippedCherryWood = "minecraft:stripped_cherry_wood";
const HashedString StrippedDarkOakLog = "minecraft:stripped_dark_oak_log";
const HashedString StrippedDarkOakWood = "minecraft:stripped_dark_oak_wood";
const HashedString StrippedJungleLog = "minecraft:stripped_jungle_log";
const HashedString StrippedJungleWood = "minecraft:stripped_jungle_wood";
const HashedString StrippedMangroveLog = "minecraft:stripped_mangrove_log";
const HashedString StrippedMangroveWood = "minecraft:stripped_mangrove_wood";
const HashedString StrippedOakLog = "minecraft:stripped_oak_log";
const HashedString StrippedOakWood = "minecraft:stripped_oak_wood";
const HashedString StrippedPaleOakLog = "minecraft:stripped_pale_oak_log";
const HashedString StrippedPaleOakWood = "minecraft:stripped_pale_oak_wood";
const HashedString StrippedSpruceLog = "minecraft:stripped_spruce_log";
const HashedString StrippedSpruceWood = "minecraft:stripped_spruce_wood";
const HashedString Tnt = "minecraft:tnt";
const HashedString UnderwaterTnt = "minecraft:underwater_tnt";
const HashedString WitherRose = "minecraft:wither_rose";
}  // namespace VanillaBlockTypeIds

namespace VanillaBlockTypeGroups {
const std::vector<std::reference_wrapper<const HashedString>> LogAndStrippedLogBlockIds = {
    VanillaBlockTypeIds::OakLog,      VanillaBlockTypeIds::StrippedOakLog,
    VanillaBlockTypeIds::SpruceLog,   VanillaBlockTypeIds::StrippedSpruceLog,
    VanillaBlockTypeIds::BirchLog,    VanillaBlockTypeIds::StrippedBirchLog,
    VanillaBlockTypeIds::JungleLog,   VanillaBlockTypeIds::StrippedJungleLog,
    VanillaBlockTypeIds::AcaciaLog,   VanillaBlockTypeIds::StrippedAcaciaLog,
    VanillaBlockTypeIds::DarkOakLog,  VanillaBlockTypeIds::StrippedDarkOakLog,
    VanillaBlockTypeIds::MangroveLog, VanillaBlockTypeIds::StrippedMangroveLog,
    VanillaBlockTypeIds::CherryLog,   VanillaBlockTypeIds::StrippedCherryLog,
    VanillaBlockTypeIds::PaleOakLog,  VanillaBlockTypeIds::StrippedPaleOakLog,
};
const std::vector<std::reference_wrapper<const HashedString>> TntIds = {VanillaBlockTypeIds::Tnt,
                                                                        VanillaBlockTypeIds::UnderwaterTnt};
const std::vector<std::reference_wrapper<const HashedString>> WoodAndStrippedWoodBlockIds = {
    VanillaBlockTypeIds::OakWood,      VanillaBlockTypeIds::StrippedOakWood,
    VanillaBlockTypeIds::SpruceWood,   VanillaBlockTypeIds::StrippedSpruceWood,
    VanillaBlockTypeIds::BirchWood,    VanillaBlockTypeIds::StrippedBirchWood,
    VanillaBlockTypeIds::JungleWood,   VanillaBlockTypeIds::StrippedJungleWood,
    VanillaBlockTypeIds::AcaciaWood,   VanillaBlockTypeIds::StrippedAcaciaWood,
    VanillaBlockTypeIds::DarkOakWood,  VanillaBlockTypeIds::StrippedDarkOakWood,
    VanillaBlockTypeIds::MangroveWood, VanillaBlockTypeIds::StrippedMangroveWood,
    VanillaBlockTypeIds::CherryWood,   VanillaBlockTypeIds::StrippedCherryWood,
    VanillaBlockTypeIds::PaleOakWood,  VanillaBlockTypeIds::StrippedPaleOakWood,
};
}  // namespace VanillaBlockTypeGroups
