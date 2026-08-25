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

#include "bedrock/world/containers/container_enum.h"
#include "bedrock/world/item/item_stack.h"

struct ExpectedSlotConsume {
    ExpectedSlotConsume(const FullContainerName &open_container_net_id, std::uint8_t slot, const ItemStack &item,
                        std::uint8_t amount_override);

    FullContainerName open_container_net_id;
    std::uint8_t slot;
    ItemStack item;
};

struct ExpectedAnywhereConsume {
    ExpectedAnywhereConsume(const ItemStack &item, std::uint32_t quantity);

    ItemStack item;
    std::uint32_t quantity;
};

struct AllowedAnywhereConsume {
    AllowedAnywhereConsume(const FullContainerName &open_container_net_id, std::uint8_t min_slot,
                           std::uint8_t max_slot);

    FullContainerName open_container_net_id;
    std::uint8_t min_slot;
    std::uint8_t max_slot;
};
