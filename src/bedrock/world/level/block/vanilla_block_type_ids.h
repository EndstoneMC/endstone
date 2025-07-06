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
extern const HashedString BeeNest;
extern const HashedString Beehive;
extern const HashedString CampFire;
extern const HashedString SoulCampfire;
extern const HashedString Tnt;
extern const HashedString UnderwaterTnt;
}  // namespace VanillaBlockTypeIds

namespace VanillaBlockTypeGroups {
extern const std::vector<std::reference_wrapper<const HashedString>> TntIds;
}  // namespace VanillaBlockTypeGroups
