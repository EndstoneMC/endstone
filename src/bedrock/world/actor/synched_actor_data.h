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

#include "bedrock/core/math/vec3.h"
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

    virtual ~DataItem();
    [[nodiscard]] virtual ID getId() const = 0;
    [[nodiscard]] virtual DataItemType getType() const = 0;
    [[nodiscard]] virtual bool isDataEqual(const DataItem &) const = 0;
    [[nodiscard]] virtual std::unique_ptr<DataItem> clone() const = 0;
};

template <typename T>
class DataItem2 : public DataItem {
public:
    DataItem2(DataItemType type, ID id, const T &data) : type_(type), id_(id), data(data) {}

private:
    const DataItemType type_;
    const ID id_;

public:
    T data;
};

class SynchedActorData {
public:
    using TypeInt8 = std::int8_t;
    using TypeShort = std::int16_t;
    using TypeInt = std::int32_t;
    using TypeInt64 = std::int64_t;
    using TypeFloat = float;
    using TypeVec3 = Vec3;
    using DataList = std::vector<std::unique_ptr<DataItem>>;
    using ID = DataItem::ID;

    [[nodiscard]] TypeInt8 getInt8(ID) const;
    [[nodiscard]] const std::string &getString(ID) const;
    [[nodiscard]] bool hasData(ID) const;

private:
    friend class SynchedActorDataEntityWrapper;

    DataItem &_get(ID);
    DataItem *_find(ID) const;

    DataList items_array_;                                                   // +0
    std::bitset<static_cast<int>(ActorDataIDs::Count)> dirty_flags_;         // +24
    std::bitset<static_cast<int>(ActorDataIDs::Count)> has_component_data_;  // +48
};

struct SynchedActorDataComponent;

class SynchedActorDataEntityWrapper {
public:
    [[nodiscard]] SynchedActorData::TypeInt8 getInt8(SynchedActorData::ID) const;
    [[nodiscard]] const std::string &getString(SynchedActorData::ID) const;
    template <typename T>
    void set(SynchedActorData::ID, const T &);

private:
    gsl::not_null<SynchedActorData *> _get() const;

    gsl::not_null<SynchedActorDataComponent *> data_;            // +0
    gsl::not_null<ActorDataFlagComponent *> flag_data_;          // +8
    gsl::not_null<ActorDataDirtyFlagsComponent *> dirty_flags_;  // +16
    WeakRef<EntityContext> entity_context_;                      // +24
};
BEDROCK_STATIC_ASSERT_SIZE(SynchedActorDataEntityWrapper, 48, 48);
