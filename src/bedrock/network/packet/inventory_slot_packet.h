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
#include <optional>

#include "bedrock/network/packet.h"
#include "bedrock/network/packet/cerealize/core/packet_serialization_helper.h"
#include "bedrock/network/packet/serialize/serialized_packet.h"
#include "bedrock/world/container_id.h"
#include "bedrock/world/containers/container_enum.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/item/network_item_stack_descriptor.h"

struct InventorySlotPacketPayload {
    InventorySlotPacketPayload();
    InventorySlotPacketPayload(ContainerID id, std::uint32_t slot, const ItemStack &item);
    InventorySlotPacketPayload(ContainerID id, std::uint32_t slot, const ItemStack &item,
                               const FullContainerName &full_container_name, const ItemStack &storage_item);
    ContainerID inventory_id;
    std::uint32_t slot;
    std::optional<FullContainerName> full_container_name;
    std::optional<NetworkItemStackDescriptor> storage_item;
    NetworkItemStackDescriptor item;
};

class InventorySlotPacket : public Packet {
public:
    InventorySlotPacketPayload payload;
    SerializationMode serialization_mode = SerializationMode::CerealOnly;
    InventorySlotPacket();
    InventorySlotPacket(InventorySlotPacketPayload payload);
};
