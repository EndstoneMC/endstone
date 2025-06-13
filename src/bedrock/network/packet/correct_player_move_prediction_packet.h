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

#include "bedrock/entity/enums/rewind_type.h"
#include "bedrock/network/packet/player_input_tick.h"

class CorrectPlayerMovePredictionPacket : public Packet {
public:
    Vec3 pos;
    Vec3 pos_delta;
    Vec2 rotation;
    std::optional<float> vehicle_angular_velocity;
    PlayerInputTick tick;
    bool on_ground;
    RewindType prediction_type;

    [[nodiscard]] MinecraftPacketIds getId() const override
    {
        return MinecraftPacketIds::CorrectPlayerMovePredictionPacket;
    }

    [[nodiscard]] std::string getName() const override
    {
        return "CorrectPlayerMovePredictionPacket";
    }

    void write(BinaryStream &stream) const override
    {
        stream.writeByte(static_cast<std::underlying_type_t<RewindType>>(prediction_type), "PredictionType",
                         "Vehicle or Player Prediction");
        stream.writeFloat(pos.x, "X", nullptr);
        stream.writeFloat(pos.y, "Y", nullptr);
        stream.writeFloat(pos.z, "Z", nullptr);
        stream.writeFloat(pos_delta.x, "X", nullptr);
        stream.writeFloat(pos_delta.y, "Y", nullptr);
        stream.writeFloat(pos_delta.z, "Z", nullptr);
        if (prediction_type == RewindType::Vehicle) {
            (stream.writeFloat)(rotation.x, "X", nullptr);
            (stream.writeFloat)(rotation.y, "Y", nullptr);
            stream.writeBool(vehicle_angular_velocity.has_value(), "Has Value",
                             "If true, follow with appropriate data type, otherwise nothing");
            if (vehicle_angular_velocity) {
                stream.writeFloat(vehicle_angular_velocity.value(), "Float", nullptr);
            }
        }
        stream.writeBool(on_ground, "On Ground", "Is on ground");
        stream.writeUnsignedVarInt64(
            tick, "Input tick",
            "\n"
            "In server authoritative movement mode the client supplies this in PlayerAuthInputPacket.\n"
            "For any client-bound packets containing a tick, the server should supply the tick value corresponding to "
            "the most recently processed PlayerAuthInputPacket.\n"
            "This allows the client to adjust any client predictions made while the packet was in flight.\n"
            "If the packet is being sent to the client but not relating to data of the player or a client-predicted "
            "vehicle, it can be zero.\n"
            "It is also acceptable to specify zero for players, although this may lead to minor visual flickers and "
            "less smooth application of CorrectPlayerMovePredictionPacket.\n");
    }

private:
    Bedrock::Result<void> _read(ReadOnlyBinaryStream &stream) override
    {
        throw std::runtime_error("Not implemented");
    }
};
static_assert(sizeof(CorrectPlayerMovePredictionPacket) == 104);
