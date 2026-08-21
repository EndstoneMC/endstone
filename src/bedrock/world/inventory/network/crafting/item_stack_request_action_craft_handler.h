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
#include <utility>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/world/inventory/network/crafting/item_stack_request_action_craft.h"
#include "bedrock/world/inventory/network/item_stack_net_result.h"
#include "bedrock/world/item/item_instance.h"

class CraftHandlerBase;
class ItemStackRequestActionHandler;
class Player;
class SimpleSparseContainer;

class ItemStackRequestActionCraftHandler {
public:
    virtual ~ItemStackRequestActionCraftHandler() = 0;
    ItemStackRequestActionCraftHandler(ItemStackRequestActionHandler &, Player &);
    ENDSTONE_HOOK ItemStackNetResult handleCraftAction(const ItemStackRequestActionCraftBase &request_action);

private:
    ItemStackRequestActionHandler &request_action_handler_;
    Player &player_;
    std::unique_ptr<CraftHandlerBase> craft_handler_;
    bool is_craft_request_;
    std::shared_ptr<SimpleSparseContainer> created_output_sparse_container_;
    std::vector<std::pair<ItemInstance, unsigned int>> craft_results_;
};
BEDROCK_STATIC_ASSERT_SIZE(ItemStackRequestActionCraftHandler, 80, 80);
