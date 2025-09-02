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

#include "bedrock/world/actor/synched_actor_data.h"

#include "bedrock/entity/components/synched_actor_data_component.h"
#include "bedrock/world/level/block_pos.h"

SynchedActorData::TypeInt8 SynchedActorData::getInt8(ID id) const
{
    if (!hasData(id)) {
        return 0;
    }
    auto *item = _find(id);
    if (item->getType() != DataItemType::Byte) {
        return 0;
    }
    return static_cast<DataItem2<TypeInt8> *>(item)->data;
}

SynchedActorData::TypeInt64 SynchedActorData::getInt64(ID id) const
{
    if (!hasData(id)) {
        return 0;
    }
    auto *item = _find(id);
    if (item->getType() != DataItemType::Int64) {
        return 0;
    }
    return static_cast<DataItem2<TypeInt64> *>(item)->data;
}

const std::string &SynchedActorData::getString(ID id) const
{
    static std::string empty_string;
    if (!hasData(id)) {
        return empty_string;
    }
    auto *item = _find(id);
    if (item->getType() != DataItemType::String) {
        return empty_string;
    }
    return static_cast<DataItem2<std::string> *>(item)->data;
}

bool SynchedActorData::hasData(ID id) const
{
    return id < items_array_.size() && items_array_[id];
}

DataItem &SynchedActorData::_get(ID id)
{
    return *items_array_[id];
}

DataItem *SynchedActorData::_find(ID id) const
{
    return items_array_[id].get();
}

SynchedActorData::TypeInt8 SynchedActorDataEntityWrapper::getInt8(SynchedActorData::ID id) const
{
    return data_->data.getInt8(id);
}

SynchedActorData::TypeInt64 SynchedActorDataEntityWrapper::getInt64(SynchedActorData::ID id) const
{
    return data_->data.getInt64(id);
}

const std::string &SynchedActorDataEntityWrapper::getString(SynchedActorData::ID id) const
{
    return data_->data.getString(id);
}

bool SynchedActorDataEntityWrapper::hasData(SynchedActorData::ID id) const
{
    return data_->data.hasData(id);
}

gsl::not_null<SynchedActorData *> SynchedActorDataEntityWrapper::_get() const
{
    return &data_->data;
}

template <>
void SynchedActorDataEntityWrapper::set<SynchedActorData::TypeInt8>(SynchedActorData::ID id,
                                                                    const SynchedActorData::TypeInt8 &value)
{
    auto data = _get();
    auto &item = data->_get(id);
    if (item.getType() != DataItemType::Byte) {
        return;
    }
    auto &item2 = static_cast<DataItem2<SynchedActorData::TypeInt8> &>(item);
    if (item2.data != value) {
        item2.data = value;
        data->dirty_flags_.set(item.getId(), true);
    }
}

template <>
void SynchedActorDataEntityWrapper::set<SynchedActorData::TypeInt64>(SynchedActorData::ID id,
                                                                    const SynchedActorData::TypeInt64 &value)
{
    auto data = _get();
    auto &item = data->_get(id);
    if (item.getType() != DataItemType::Int64) {
        return;
    }
    auto &item2 = static_cast<DataItem2<SynchedActorData::TypeInt64> &>(item);
    if (item2.data != value) {
        item2.data = value;
        data->dirty_flags_.set(item.getId(), true);
    }
}

template <>
void SynchedActorDataEntityWrapper::set<std::string>(SynchedActorData::ID id, const std::string &value)
{
    auto data = _get();
    auto &item = data->_get(id);
    if (item.getType() != DataItemType::String) {
        return;
    }
    auto &item2 = static_cast<DataItem2<std::string> &>(item);
    if (item2.data != value) {
        item2.data = value;
        data->dirty_flags_.set(item.getId(), true);
    }
}

template <>
void SynchedActorDataEntityWrapper::set<BlockPos>(SynchedActorData::ID id, const BlockPos &value)
{
    auto data = _get();
    auto &item = data->_get(id);
    if (item.getType() != DataItemType::Pos) {
        return;
    }
    auto &item2 = static_cast<DataItem2<BlockPos> &>(item);
    if (item2.data != value) {
        item2.data = value;
        data->dirty_flags_.set(item.getId(), true);
    }
}
