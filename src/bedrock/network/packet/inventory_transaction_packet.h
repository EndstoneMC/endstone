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

#include "bedrock/forward.h"
#include "bedrock/network/packet.h"
#include "bedrock/world/containers/container_enum.h"
#include "bedrock/world/inventory/network/item_stack_net_id_variant.h"

class InventoryTransactionPacket : public Packet {
public:
    [[nodiscard]] MinecraftPacketIds getId() const override;
    [[nodiscard]] std::string_view getName() const override;
    void write(BinaryStream &stream) const override;
    void handle(ServerPlayer &player, BlockPalette &block_palette, const MoveInputComponent &move_input,
                ActorRotationComponent &actor_rotation, bool is_aim_assist) const;

    ItemStackLegacyRequestId legacy_request_id;
    std::vector<std::pair<ContainerEnumName, std::vector<unsigned char>>> legacy_set_item_slots;
    std::unique_ptr<ComplexInventoryTransaction> transaction;
    bool is_client_side;

private:
    Bedrock::Result<void> _read(ReadOnlyBinaryStream &stream) override;
};
