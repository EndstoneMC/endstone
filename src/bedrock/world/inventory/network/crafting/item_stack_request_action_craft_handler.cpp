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

#include "bedrock/world/inventory/network/crafting/item_stack_request_action_craft_handler.h"

#include "bedrock/world/item/item.h"

ItemStackNetResult ItemStackRequestActionCraftHandler::_initCraftResults(const std::vector<ItemInstance> &results,
                                                                         std::uint8_t num_crafts)
{
    if (!craft_results_.empty()) {
        return ItemStackNetResult::RequestAlreadyInProgress;
    }
    if (results.empty()) {
        return ItemStackNetResult::EmptyCraftResults;
    }
    for (const auto &result : results) {
        ItemInstance item;
        item = result;
        if (!item) {
            return ItemStackNetResult::InvalidCraftResultItem;
        }
        if (item.getAuxValue() != 0) {
            item.getItem()->fixupCommon(item);
        }
        auto &craft_result = craft_results_.emplace_back();
        craft_result.first = item;
        craft_result.second = (num_crafts - 1) * item.getCount();
    }
    return ItemStackNetResult::Success;
}
