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

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include "bedrock/forward.h"
#include "bedrock/util/new_type.h"
#include "bedrock/util/weighted_random_list.h"

using ActorList = std::vector<class Actor *>;
using BlockActorList = std::vector<class BlockActor *>;
using RuntimeActorList = std::vector<Actor *>;
using MobList = std::vector<std::shared_ptr<class MobSpawnerData>>;
using MobCategoryMap = std::array<WeightedRandomList<br::worldgen::SpawnerData>, 8UL>;

enum class SubClientId : std::uint8_t {
    Server = 0,
    PrimaryClient = 0,
    Client2 = 1,
    Client3 = 2,
    Client4 = 3,
    ExtraIdSlotStart = 100,
    EditorUI = 101,
};

using BlockRuntimeId = std::uint32_t;
struct BiomeIdType : NewType<std::uint16_t> {};

struct NewBlockID : NewType<std::uint16_t> {};
static_assert(sizeof(NewBlockID) == 2);

struct Brightness : NewType<std::uint8_t> {};
static_assert(sizeof(Brightness) == 1);

struct BrightnessPair {
    Brightness sky;
    Brightness block;
};
static_assert(sizeof(BrightnessPair) == 2);

using DataID = std::uint16_t;
using FacingID = std::uint8_t;
