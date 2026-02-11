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

#include "bedrock/network/packet/inventory_slot_packet.h"

#include "bedrock/network/packet/serialize/world/item/item_stack_serializer.h"

InventorySlotPacket::InventorySlotPacket(ContainerID id, std::uint32_t slot, const ItemStack &item)
    : InventorySlotPacket(id, slot, item, {}, {})
{
}

InventorySlotPacket::InventorySlotPacket(ContainerID id, std::uint32_t slot, const ItemStack &item,
                                         const FullContainerName &full_container_name, const ItemStack &storage_item)
    : inventory_id(id), full_container_name(full_container_name), storage_item(storage_item), slot(slot), item(item)
{
}

MinecraftPacketIds InventorySlotPacket::getId() const
{
    return MinecraftPacketIds::InventorySlot;
}

std::string_view InventorySlotPacket::getName() const
{
    return "InventorySlotPacket";
}

void InventorySlotPacket::write(BinaryStream &stream) const
{
    stream.writeUnsignedVarInt(inventory_id, "Container ID", nullptr);
    stream.writeUnsignedVarInt(slot, "Slot", nullptr);
    stream.writeByte(static_cast<std::uint8_t>(full_container_name.name), "Container Name", nullptr);
    stream.writeBool(full_container_name.dynamic_id.has_value(), "Has Value",
                     "If true, follow with appropriate data type, otherwise nothing");
    if (full_container_name.dynamic_id.has_value()) {
        stream.writeUnsignedInt(full_container_name.dynamic_id.value(), "Unsigned Int", nullptr);
    }
    ItemStackSerializerHelpers::write<NetworkItemStackDescriptor>(storage_item, stream);
    if (item) {
        item.setIncludeNetIds(true);
    }
    ItemStackSerializerHelpers::write<NetworkItemStackDescriptor>(item, stream);
}

Bedrock::Result<void> InventorySlotPacket::_read(ReadOnlyBinaryStream &stream)
{
    throw std::runtime_error("InventorySlotPacket::_read()");
}
