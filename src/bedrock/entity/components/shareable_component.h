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

#include <string>
#include <vector>

#include <entt/core/type_info.hpp>

#include "bedrock/bedrock.h"
#include "bedrock/world/item/item_descriptor.h"

class Actor;
class ItemStack;

struct Shareable {
    Shareable();
    ItemDescriptor item_descriptor;
    int want_amount;
    int surplus_amount;
    int max_amount;
    int pickup_limit;
    int craft_into_item;
    int craft_into_item_aux;
    int item_priority;
    bool admire;
    bool barter;
    bool stored_in_inventory;
    bool consume_item;
    bool pickup_only;
    bool singular_pickup;
    void setItem(const std::string &);
    void setCraftInto(const std::string &name);
};
BEDROCK_STATIC_ASSERT_SIZE(Shareable, 56, 56);

class ShareableComponent {
public:
    [[nodiscard]] int getCountToPickUp(const Actor &actor, const ItemStack &item) const;

private:
    std::vector<Shareable> items_;
    bool share_all_items_;
    int all_item_want_amount_;
    int all_item_surplus_amount_;
    int all_item_max_amount_;
    bool singular_pickup_;
};
BEDROCK_STATIC_ASSERT_SIZE(ShareableComponent, 48, 48);
#if defined __clang__ || defined __GNUC__
static_assert(entt::type_hash<ShareableComponent>::value() == 0xDDFFCB9F);
#endif
