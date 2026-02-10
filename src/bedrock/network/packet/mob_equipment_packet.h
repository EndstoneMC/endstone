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

#include "bedrock/network/packet.h"
#include "bedrock/network/packet/inventory_transaction_packet.h"

class MobEquipmentPacket : public InventoryPacket {
public:
    static constexpr bool SHARE_WITH_HANDLER = true;

    MobEquipmentPacket();
    MobEquipmentPacket(ActorRuntimeID, const ItemStack &, int, int, ContainerID);
    MobEquipmentPacket(ActorRuntimeID, const NetworkItemStackDescriptor &, int, int, ContainerID);

    [[nodiscard]] MinecraftPacketIds getId() const override;
    [[nodiscard]] std::string_view getName() const override;
    void handle(ServerPlayer &, BlockPalette &, const MoveInputComponent &, ActorRotationComponent &,
                bool) const override;
    [[nodiscard]] const ComplexInventoryTransaction *getComplexInventoryTransaction() const override;
    void write(BinaryStream &) const override;

    ActorRuntimeID runtime_id;
    NetworkItemStackDescriptor item;
    int slot;
    int selected_slot;
    ContainerID container_id;
    char slot_byte;
    char selected_slot_byte;
    char container_id_byte;
};
