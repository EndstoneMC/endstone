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

enum class ItemVersion : int {
    Legacy = 0,
    DataDriven = 1,
    None = 2,
};

class ItemRegistry : public std::enable_shared_from_this<ItemRegistry> {
public:
    static const std::int16_t START_ITEM_ID = 256;

    [[nodiscard]] WeakPtr<Item> getItem(std::int16_t id) const;
    [[nodiscard]] WeakPtr<Item> getItem(const HashedString &name) const;
    Bedrock::NonOwnerPointer<CreativeItemRegistry> getCreativeItemRegistry() const;

private:
    WeakPtr<Item> lookupByName(int &, std::string_view) const;

    friend ItemRegistryRef;

    using ItemRegistryMap = std::vector<SharedPtr<Item>>;
    struct ItemAlias {
        HashedString current_name;
        BaseGameVersion since_version;
    };
    struct ItemHashAlias {
        std::size_t alias_name;
        BaseGameVersion since_version;
    };

    gsl::not_null<std::unique_ptr<cereal::ReflectionCtx>> cereal_context_;
    Bedrock::NonOwnerPointer<LinkedAssetValidator> validator_;
    ItemRegistryMap item_registry_;  // +48
    std::unordered_map<int, WeakPtr<Item>> id_to_item_map_;
    std::unordered_map<HashedString, WeakPtr<Item>> name_to_item_map_;
    std::unordered_map<HashedString, WeakPtr<Item>> tile_namespace_to_item_map_;
    std::unordered_map<HashedString, WeakPtr<Item>> tile_item_name_to_item_map_;
    std::unordered_map<HashedString, ItemAlias> item_alias_lookup_map_;
    std::unordered_map<std::size_t, ItemHashAlias> reverse_alias_lookup_map_;
    std::unordered_map<std::size_t, ItemHashAlias> reverse_full_name_alias_lookup_map_;
    std::unordered_map<HashedString, ItemRegistryComplexAlias> complex_alias_lookup_map_;
    std::unordered_map<std::int16_t, HashedString> legacy_id_to_name_map_;
    std::int16_t max_item_id_;
    std::vector<HashedString> attachable_definitions_;
    std::unordered_map<ItemTag, std::unordered_set<const Item *>> tag_to_items_map_;
    std::unordered_set<const Item *> const empty_item_set_;
    bool server_initializing_creative_items_;
    bool is_initialized_;
    std::function<void(ItemRegistryRef)> extra_item_init_callback_;
    std::unique_ptr<Bedrock::PubSub::PublisherPtr<void(), Bedrock::PubSub::ThreadModel::MultiThreaded>>
        finished_init_server_publisher_;
    std::shared_ptr<std::atomic<int>> can_update_tags_;
    ItemRegistryMap dead_item_registry_;
    BaseGameVersion world_base_game_version_;
    bool check_for_item_world_compatibility_;
    std::shared_ptr<std::mutex> compatibility_check_mutex_;
    std::unique_ptr<CreativeItemRegistry> creative_item_registry_;
};
