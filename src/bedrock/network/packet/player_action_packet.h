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

#include "bedrock/network/network_block_position.h"
#include "bedrock/network/packet.h"
#include "bedrock/network/packet/serialize/serialized_packet.h"
#include "bedrock/world/actor/actor_runtime_id.h"

enum class PlayerActionType : int {
    Unknown = -1,
    StartDestroyBlock = 0,
    AbortDestroyBlock = 1,
    StopDestroyBlock = 2,
    GetUpdatedBlock = 3,
    DropItem = 4,
    StartSleeping = 5,
    StopSleeping = 6,
    Respawn = 7,
    StartJump = 8,
    StartSprinting = 9,
    StopSprinting = 10,
    StartSneaking = 11,
    StopSneaking = 12,
    CreativeDestroyBlock = 13,
    ChangeDimensionAck = 14,
    StartGliding = 15,
    StopGliding = 16,
    DenyDestroyBlock = 17,
    CrackBlock = 18,
    ChangeSkin = 19,
    DEPRECATED_UpdatedEnchantingSeed = 20,
    StartSwimming = 21,
    StopSwimming = 22,
    StartSpinAttack = 23,
    StopSpinAttack = 24,
    InteractWithBlock = 25,
    PredictDestroyBlock = 26,
    ContinueDestroyBlock = 27,
    StartItemUseOn = 28,
    StopItemUseOn = 29,
    HandledTeleport = 30,
    MissedSwing = 31,
    StartCrawling = 32,
    StopCrawling = 33,
    StartFlying = 34,
    StopFlying = 35,
    DEPRECATED_ClientAckServerData = 36,
    StartUsingItem = 37,
    Count = 38,
};

struct PlayerActionPacketPayload {
    PlayerActionPacketPayload();
    PlayerActionPacketPayload(PlayerActionType, const BlockPos &, FacingID, ActorRuntimeID);
    PlayerActionPacketPayload(PlayerActionType, ActorRuntimeID);
    PlayerActionPacketPayload(PlayerActionType, const BlockPos &, ActorRuntimeID);
    PlayerActionPacketPayload(PlayerActionType, int, ActorRuntimeID);
    PlayerActionPacketPayload(PlayerActionType, const BlockPos &, int, ActorRuntimeID);
    PlayerActionPacketPayload(PlayerActionType, const BlockPos &, const BlockPos &, int, ActorRuntimeID);
    [[nodiscard]] bool getIsFromServerPlayerMovementSystem() const;
    void setFromServerPlayerMovementSystem(bool);

    NetworkBlockPosition pos;
    NetworkBlockPosition result_pos;
    int face;
    PlayerActionType action;
    ActorRuntimeID runtime_id;

private:
    bool is_from_server_player_movement_system_;  // +40
};

struct PlayerActionPacketInfo {
    static constexpr auto PACKET_NAME = "PlayerActionPacket";
    static constexpr auto PACKET_ID = MinecraftPacketIds::PlayerAction;
    static constexpr auto DEFAULT_PACKET_SERIALIZATION_MODE = SerializationMode::SideBySide_LogOnMismatch;
    static constexpr auto COMPRESSIBILITY = Compressibility::Compressible;
};

class PlayerActionPacket : public SerializedPayloadPacket<PlayerActionPacketInfo, PlayerActionPacketPayload> {
public:
    static constexpr bool SHARE_WITH_HANDLER = false;
};
