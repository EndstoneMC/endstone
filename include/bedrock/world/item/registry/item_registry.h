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
#include <unordered_map>
#include <vector>

#include "bedrock/core/hashed_string.h"
#include "bedrock/core/memory.h"

class ItemRegistry : public std::enable_shared_from_this<ItemRegistry> {
public:
    [[nodiscard]] WeakPtr<Item> getItem(int id) const
    {
        return id_to_item_map_.at(id);
    }

    [[nodiscard]] int getItemCount() const
    {
        return items_.size();
    }

    [[nodiscard]] const std::unordered_map<HashedString, WeakPtr<Item>> &getNameToItemMap() const
    {
        return name_to_item_map_;
    }

private:
    std::vector<SharedPtr<Item>> items_;                                // +16
    std::unordered_map<int, WeakPtr<Item>> id_to_item_map_;             // +40
    std::unordered_map<HashedString, WeakPtr<Item>> name_to_item_map_;  // +104 (+80)
    std::unordered_map<HashedString, WeakPtr<Item>> unknown2_;          // +168 (+120)
    std::unordered_map<HashedString, WeakPtr<Item>> unknown3_;          // +232 (+160)
};
