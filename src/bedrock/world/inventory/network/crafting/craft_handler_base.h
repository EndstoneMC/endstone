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

#include <cstdint>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/world/containers/container_enum.h"
#include "bedrock/world/inventory/network/item_stack_net_result.h"
#include "bedrock/world/inventory/network/item_stack_request_action.h"
#include "bedrock/world/inventory/simulation/container_screen_validation_utils.h"

class ItemStack;
class ItemStackRequestActionCraftBase;
class ItemStackRequestActionCraftHandler;
class Recipes;

class CraftHandlerBase {
public:
    virtual ~CraftHandlerBase() = 0;
    virtual ItemStackNetResult handleConsumedItem(const FullContainerName &, std::uint8_t, const ItemStack &) = 0;
    virtual ItemStackNetResult preHandleAction(ItemStackRequestActionType) = 0;
    virtual void endRequestBatch() = 0;

protected:
    CraftHandlerBase();

    virtual ItemStackNetResult _handleCraftAction(const ItemStackRequestActionCraftBase &) = 0;
    virtual void _postCraftRequest(bool) = 0;
    [[nodiscard]] virtual const Recipes *_getLevelRecipes() const = 0;

    ItemStackRequestActionCraftHandler &craft_request_handler_;
    ItemStackRequestActionType craft_action_type_;
    bool non_implemented_trust_client_results_;
    std::vector<ExpectedSlotConsume> expected_slot_consumes_;
    std::vector<ExpectedAnywhereConsume> expected_anywhere_consumes_;
    std::vector<AllowedAnywhereConsume> allowed_anywhere_consumes_;

private:
    bool is_craft_request_;
};
BEDROCK_STATIC_ASSERT_SIZE(CraftHandlerBase, 104, 104);
