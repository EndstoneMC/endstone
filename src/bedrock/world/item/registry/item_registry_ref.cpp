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

#include "bedrock/world/item/registry/item_registry_ref.h"

ItemRegistryRef::LockGuard::LockGuard(std::shared_ptr<std::mutex> mutex) : mutex_(mutex)
{
    if (mutex_) {
        mutex_->lock();
    }
}

ItemRegistryRef::LockGuard::~LockGuard()
{
    if (mutex_) {
        mutex_->unlock();
    }
}

bool ItemRegistryRef::isValid() const
{
    return !weak_registry_.expired();
}

WeakPtr<Item> ItemRegistryRef::lookupByName(int &out_item_aux, std::string_view in_string) const
{
    return _lockRegistry()->lookupByName(out_item_aux, in_string);
}

const std::unordered_map<HashedString, WeakPtr<Item>> &ItemRegistryRef::getNameToItemMap() const
{
    return _lockRegistry()->name_to_item_map_;
}

WeakPtr<Item> ItemRegistryRef::getItem(std::int16_t id) const
{
    return _lockRegistry()->getItem(id);
}

WeakPtr<Item> ItemRegistryRef::getItem(const HashedString &id) const
{
    return _lockRegistry()->getItem(id);
}

BaseGameVersion ItemRegistryRef::getWorldBaseGameVersion() const
{
    return _lockRegistry()->world_base_game_version_;
}

bool ItemRegistryRef::shouldCheckForItemWorldCompatibility() const
{
    return _lockRegistry()->check_for_item_world_compatibility_;
}

ItemRegistryRef::LockGuard ItemRegistryRef::lockItemWorldCompatibilityMutex() const
{
    return _lockRegistry()->compatibility_check_mutex_;
}

Bedrock::NonOwnerPointer<CreativeItemRegistry> ItemRegistryRef::getCreativeItemRegistry()
{
    return _lockRegistry()->getCreativeItemRegistry();
}

std::shared_ptr<ItemRegistry> ItemRegistryRef::_lockRegistry() const
{
    return weak_registry_.lock();
}
