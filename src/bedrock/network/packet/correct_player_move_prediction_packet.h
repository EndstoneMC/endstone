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

#include <optional>
#include <stdexcept>

#include "bedrock/core/math/vec2.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/entity/enums/rewind_type.h"
#include "bedrock/network/packet.h"
#include "bedrock/network/packet/player_input_tick.h"

struct CorrectPlayerMovePredictionPacketPayload {
    Vec3 pos;
    Vec3 pos_delta;
    Vec2 vehicle_rotation;
    std::optional<float> vehicle_angular_velocity;
    PlayerInputTick tick;
    bool on_ground;
    RewindType prediction_type;
};

class CorrectPlayerMovePredictionPacket : public Packet {
public:
    [[nodiscard]] MinecraftPacketIds getId() const override
    {
        return MinecraftPacketIds::CorrectPlayerMovePredictionPacket;
    }
    [[nodiscard]] std::string_view getName() const override
    {
        return "CorrectPlayerMovePredictionPacket";
    }
    void write(BinaryStream &stream) const override
    {
        throw std::logic_error("Not implemented");
    }
    Bedrock::Result<void> _read(ReadOnlyBinaryStream &stream) override
    {
        throw std::logic_error("Not implemented");
    }

    CorrectPlayerMovePredictionPacketPayload payload;
    SerializationMode serialization_mode;
};
