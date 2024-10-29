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

#include "bedrock/core/string/string_hash.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/core/utility/pub_sub/publisher.h"
#include "bedrock/resources/base_game_version.h"
#include "bedrock/shared_ptr.h"
#include "bedrock/world/item/registry/creative_item_registry.h"
#include "bedrock/world/item/registry/item_registry_complex_alias.h"

class Item;
class ItemRegistryRef;

class ItemRegistry : public std::enable_shared_from_this<ItemRegistry> {
    struct ItemAlias {
        HashedString current_name;
        BaseGameVersion since_version;
    };

    struct ItemHashAlias {
        std::size_t alias_name;
        BaseGameVersion since_version;
    };

public:
    [[nodiscard]] WeakPtr<Item> getItem(std::int16_t id) const;
    [[nodiscard]] WeakPtr<Item> getItem(const HashedString &name) const;
    Bedrock::NonOwnerPointer<CreativeItemRegistry> getCreativeItemRegistry() const;

private:
    friend ItemRegistryRef;
    using ItemRegistryMap = std::vector<SharedPtr<Item>>;

    ItemRegistryMap item_registry_;                                                        // +16
    std::unordered_map<int, WeakPtr<Item>> id_to_item_map_;                                // +40
    std::unordered_map<HashedString, WeakPtr<Item>> name_to_item_map_;                     // +104 (+80)
    std::unordered_map<HashedString, WeakPtr<Item>> tile_namespace_to_item_map_;           // +168 (+120)
    std::unordered_map<HashedString, WeakPtr<Item>> tile_item_name_to_item_map_;           // +232 (+160)
    std::unordered_map<HashedString, ItemAlias> item_alias_lookup_map_;                    // +296
    std::unordered_map<std::size_t, ItemHashAlias> reverse_alias_lookup_map_;              // +360
    std::unordered_map<std::size_t, ItemHashAlias> reverse_full_name_alias_lookup_map_;    // +424
    std::unordered_map<HashedString, ItemRegistryComplexAlias> complex_alias_lookup_map_;  // +488
    std::unordered_map<std::int16_t, HashedString> legacy_id_to_name_map_;                 // +552
    std::int16_t max_item_id_;                                                             // +616
    std::vector<HashedString> attachable_definitions_;                                     // +624
    std::unordered_map<ItemTag, std::unordered_set<const Item *>> tag_to_items_map_;       // +648
    std::unordered_set<const Item *> const empty_item_set_;                                // +712
    bool server_initializing_creative_items_;                                              // +776
    bool is_initialized_;                                                                  // +777
    std::function<void(ItemRegistryRef)> extra_item_init_callback_;                        // +784
    Bedrock::PubSub::PublisherPtr<void(), Bedrock::PubSub::ThreadModel::MultiThreaded>
        finished_init_publisher_;                                   // +848
    std::shared_ptr<std::atomic<int>> can_update_tags_;             // +856
    ItemRegistryMap dead_item_registry_;                            // +872
    BaseGameVersion world_base_game_version_;                       // +896
    bool check_for_item_world_compatibility_;                       // +1016
    std::shared_ptr<std::mutex> compatibility_check_mutex_;         // +1024
    std::unique_ptr<CreativeItemRegistry> creative_item_registry_;  // +1040
};
