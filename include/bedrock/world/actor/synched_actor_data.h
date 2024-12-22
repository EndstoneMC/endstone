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

#include <gsl/gsl>

#include "bedrock/entity/components/actor_data_dirty_flags_component.h"
#include "bedrock/entity/components/actor_data_flag_component.h"
#include "bedrock/entity/gamerefs_entity/gamerefs_entity.h"

enum class DataItemType : std::uint8_t {
    Byte = 0,
    Short = 1,
    Int = 2,
    Float = 3,
    String = 4,
    CompoundTag = 5,
    Pos = 6,
    Int64 = 7,
    Vec3 = 8,
    Unknown = 9,
};

class DataItem {
public:
    using ID = std::uint16_t;
};

class SynchedActorData {
public:
    using ID = DataItem::ID;
};

struct SynchedActorDataComponent;

class SynchedActorDataEntityWrapper {
public:
    [[nodiscard]] const std::string &getString(SynchedActorData::ID) const;
    template <typename T>
    void set(SynchedActorData::ID, const T &);

private:
    gsl::not_null<SynchedActorDataComponent *> data_;            // +0
    gsl::not_null<ActorDataFlagComponent *> flag_data_;          // +8
    gsl::not_null<ActorDataDirtyFlagsComponent *> dirty_flags_;  // +16
    WeakRef<EntityContext> entity_context_;                      // +24
};
BEDROCK_STATIC_ASSERT_SIZE(SynchedActorDataEntityWrapper, 48, 48);
