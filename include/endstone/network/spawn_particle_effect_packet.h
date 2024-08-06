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
#include <string>

#include "endstone/network/packet.h"
#include "endstone/network/packet_type.h"
#include "endstone/util/vector.h"

namespace endstone {

/**
 * @brief Represents a packet for spawning a particle effect.
 */
class SpawnParticleEffectPacket final : public Packet {
public:
    [[nodiscard]] PacketType getType() const override
    {
        return PacketType::SpawnParticleEffect;
    }

    int dimension_id;
    std::int64_t actor_id{-1};
    Vector<float> position;
    std::string effect_name;
    std::optional<std::string> molang_variables_json;
};

}  // namespace endstone
