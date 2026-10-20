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
#include "bedrock/network/packet.h"
#include "bedrock/network/packet/serialize/serialized_packet.h"
#include "bedrock/safety/redactable_string.h"
#include "bedrock/world/containers/container_enum.h"
#include "bedrock/world/inventory/network/item_stack_net_id_variant.h"
#include "bedrock/world/inventory/network/item_stack_net_result.h"

struct ItemStackResponseSlotInfo {
    std::uint8_t requested_slot;
    std::uint8_t slot;
    std::uint8_t amount;
    ItemStackNetId item_stack_net_id;
    Bedrock::Safety::RedactableString custom_name;
    std::int16_t durability_correction;
};
BEDROCK_STATIC_ASSERT_SIZE(ItemStackResponseSlotInfo, 88, 72);

struct ItemStackResponseContainerInfo {
    FullContainerName full_container_name;
    std::vector<ItemStackResponseSlotInfo> slots;
};
BEDROCK_STATIC_ASSERT_SIZE(ItemStackResponseContainerInfo, 40, 40);

struct ItemStackResponseInfo {
    ItemStackNetResult result;
    TypedClientNetId<ItemStackRequestIdTag> client_request_id;
    std::vector<ItemStackResponseContainerInfo> containers;
};
BEDROCK_STATIC_ASSERT_SIZE(ItemStackResponseInfo, 48, 48);

struct ItemStackResponsePacketPayload {
    std::vector<ItemStackResponseInfo> responses;
};

class ItemStackResponsePacket : public Packet {
public:
    ItemStackResponsePacketPayload payload;
    SerializationMode serialization_mode;
};
BEDROCK_STATIC_ASSERT_SIZE(ItemStackResponsePacket, 80, 80);
