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

#include "bedrock/network/packet.h"
#include "bedrock/world/container_id.h"
#include "bedrock/world/containers/container_enum.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/item/network_item_stack_descriptor.h"

class InventorySlotPacket : public Packet {
public:
    InventorySlotPacket();
    InventorySlotPacket(ContainerID id, std::uint32_t slot, const ItemStack &item);
    InventorySlotPacket(ContainerID id, std::uint32_t slot, const ItemStack &item,
                        const FullContainerName &full_container_name, const ItemStack &storage_item);
    [[nodiscard]] MinecraftPacketIds getId() const override;
    [[nodiscard]] std::string_view getName() const override;
    void write(BinaryStream &stream) const override;

    ContainerID inventory_id;
    FullContainerName full_container_name;
    NetworkItemStackDescriptor storage_item;
    uint32_t slot;
    NetworkItemStackDescriptor item;

private:
    Bedrock::Result<void> _read(ReadOnlyBinaryStream &) override;
};
