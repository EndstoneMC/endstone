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

#include <vector>

#include "bedrock/core/string/string_hash.h"

namespace VanillaBlockTypeIds {
extern const HashedString AcaciaLog;
extern const HashedString AcaciaWood;
extern const HashedString Bed;
extern const HashedString BeeNest;
extern const HashedString Beehive;
extern const HashedString BirchLog;
extern const HashedString BirchWood;
extern const HashedString CampFire;
extern const HashedString Cauldron;
extern const HashedString CherryLog;
extern const HashedString CherryWood;
extern const HashedString DarkOakLog;
extern const HashedString DarkOakWood;
extern const HashedString EndPortal;
extern const HashedString EndPortalFrame;
extern const HashedString JungleLog;
extern const HashedString JungleWood;
extern const HashedString MangroveLog;
extern const HashedString MangroveWood;
extern const HashedString OakLog;
extern const HashedString OakWood;
extern const HashedString PaleOakLog;
extern const HashedString PaleOakWood;
extern const HashedString SoulCampfire;
extern const HashedString SpruceLog;
extern const HashedString SpruceWood;
extern const HashedString StrippedAcaciaLog;
extern const HashedString StrippedAcaciaWood;
extern const HashedString StrippedBirchLog;
extern const HashedString StrippedBirchWood;
extern const HashedString StrippedCherryLog;
extern const HashedString StrippedCherryWood;
extern const HashedString StrippedDarkOakLog;
extern const HashedString StrippedDarkOakWood;
extern const HashedString StrippedJungleLog;
extern const HashedString StrippedJungleWood;
extern const HashedString StrippedMangroveLog;
extern const HashedString StrippedMangroveWood;
extern const HashedString StrippedOakLog;
extern const HashedString StrippedOakWood;
extern const HashedString StrippedPaleOakLog;
extern const HashedString StrippedPaleOakWood;
extern const HashedString StrippedSpruceLog;
extern const HashedString StrippedSpruceWood;
extern const HashedString Tnt;
extern const HashedString UnderwaterTnt;
extern const HashedString WitherRose;
}  // namespace VanillaBlockTypeIds

namespace VanillaBlockTypeGroups {
extern const std::vector<std::reference_wrapper<const HashedString>> LogAndStrippedLogBlockIds;
extern const std::vector<std::reference_wrapper<const HashedString>> TntIds;
extern const std::vector<std::reference_wrapper<const HashedString>> WoodAndStrippedWoodBlockIds;
}  // namespace VanillaBlockTypeGroups
