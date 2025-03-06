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

#include <memory>

#include "bedrock/core/string/string_hash.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/registry/creative_item_registry.h"
#include "bedrock/world/item/registry/item_registry.h"

class ItemRegistryRef {
public:
    class LockGuard {
    public:
        LockGuard() = default;
        LockGuard(std::shared_ptr<std::mutex>);
        ~LockGuard();

    private:
        std::shared_ptr<Bedrock::Threading::Mutex> mutex_;  // +0
    };

  [[nodiscard]]    bool isValid() const;
    WeakPtr<Item> lookupByName(int &, std::string_view) const;
    [[nodiscard]] const std::unordered_map<HashedString, WeakPtr<Item>> &getNameToItemMap() const;
    [[nodiscard]] WeakPtr<Item> getItem(std::int16_t id) const;
    [[nodiscard]] WeakPtr<Item> getItem(const HashedString &id) const;
    [[nodiscard]] BaseGameVersion getWorldBaseGameVersion() const;
    [[nodiscard]] bool shouldCheckForItemWorldCompatibility() const;
    [[nodiscard]] LockGuard lockItemWorldCompatibilityMutex() const;
    Bedrock::NonOwnerPointer<CreativeItemRegistry> getCreativeItemRegistry();

private:
    [[nodiscard]] std::shared_ptr<ItemRegistry> _lockRegistry() const;  // NOLINT

    std::weak_ptr<ItemRegistry> weak_registry_;
};
