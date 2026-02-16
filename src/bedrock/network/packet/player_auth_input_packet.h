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

#include "bedrock/core/math/vec2.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/entity/components/player_block_actions.h"
#include "bedrock/input/input_mode.h"
#include "bedrock/network/packet.h"
#include "bedrock/network/packet/player_input_tick.h"
#include "bedrock/world/actor/actor_unique_id.h"

class PlayerAuthInputPacket : public Packet {
public:
    enum InputData : unsigned int {
        Ascend = 0,
        Descend = 1,
        NorthJump_DEPRECATED = 2,
        JumpDown = 3,
        SprintDown = 4,
        ChangeHeight = 5,
        Jumping = 6,
        AutoJumpingInWater = 7,
        Sneaking = 8,
        SneakDown = 9,
        Up = 10,
        Down = 11,
        Left = 12,
        Right = 13,
        UpLeft = 14,
        UpRight = 15,
        WantUp = 16,
        WantDown = 17,
        WantDownSlow = 18,
        WantUpSlow = 19,
        Sprinting = 20,
        AscendBlock = 21,
        DescendBlock = 22,
        SneakToggleDown = 23,
        PersistSneak = 24,
        StartSprinting = 25,
        StopSprinting = 26,
        StartSneaking = 27,
        StopSneaking = 28,
        StartSwimming = 29,
        StopSwimming = 30,
        StartJumping = 31,
        StartGliding = 32,
        StopGliding = 33,
        PerformItemInteraction = 34,
        PerformBlockActions = 35,
        PerformItemStackRequest = 36,
        HandledTeleport = 37,
        Emoting = 38,
        MissedSwing = 39,
        StartCrawling = 40,
        StopCrawling = 41,
        StartFlying = 42,
        StopFlying = 43,
        ClientAckServerData = 44,
        IsInClientPredictedVehicle = 45,
        PaddlingLeft = 46,
        PaddlingRight = 47,
        BlockBreakingDelayEnabled = 48,
        HorizontalCollision = 49,
        VerticalCollision = 50,
        DownLeft = 51,
        DownRight = 52,
        StartUsingItem = 53,
        IsCameraRelativeMovementEnabled = 54,
        IsRotControlledByMoveDirection = 55,
        StartSpinAttack = 56,
        StopSpinAttack = 57,
        IsHotbarOnlyTouch = 58,
        JumpReleasedRaw = 59,
        JumpPressedRaw = 60,
        JumpCurrentRaw = 61,
        SneakReleasedRaw = 62,
        SneakPressedRaw = 63,
        SneakCurrentRaw = 64,
        INPUT_NUM = 65,
    };

    PlayerAuthInputPacket() = default;
    [[nodiscard]] MinecraftPacketIds getId() const override { return MinecraftPacketIds::PlayerAuthInputPacket; }
    [[nodiscard]] std::string_view getName() const override { return "PlayerAuthInputPacket"; }
    void write(BinaryStream &stream) const override {}                                 // todo
    Bedrock::Result<void> _read(ReadOnlyBinaryStream &stream) override { return {}; }  // todo

    [[nodiscard]] bool getInput(InputData input) const { return input_data.test(input); }

    Vec2 rot;
    Vec3 pos;
    float y_head_rot;
    Vec3 pos_delta;
    Vec2 vehicle_rot;
    Vec2 analog_move_vector;
    Vec2 move;
    Vec2 interact_rotation;
    Vec3 camera_orientation;
    Vec2 raw_move_vector;
    std::bitset<INPUT_NUM> input_data;
    InputMode input_mode;
    ClientPlayMode player_mode;
    NewInteractionModel new_interaction_model;
    PlayerInputTick client_tick;
    std::unique_ptr<PackedItemUseLegacyInventoryTransaction> item_use_transaction;
    std::unique_ptr<ItemStackRequestData> item_stack_request;
    PlayerBlockActions player_block_actions;
    ActorUniqueID client_predicted_vehicle;
};
