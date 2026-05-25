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
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "bedrock/core/math/color.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/network/packet.h"
#include "bedrock/world/actor/actor_runtime_id.h"
#include "bedrock/world/level/dimension/dimension_type.h"

namespace ScriptModuleDebugUtilities {
enum class ScriptDebugShapeType : std::uint8_t {
    Line = 0,
    Box = 1,
    Sphere = 2,
    Circle = 3,
    Text = 4,
    Arrow = 5,
    NumShapeTypes = 6,
};
class ScriptDebugShape;
}  // namespace ScriptModuleDebugUtilities

namespace cereal {
struct NullType {};
}  // namespace cereal

struct ArrowDataPayload {
    std::optional<Vec3> end_location;
    std::optional<float> arrow_head_length;
    std::optional<float> arrow_head_radius;
    std::optional<std::uint8_t> num_segments;
};

struct TextDataPayload {
    std::string text;
};

struct BoxDataPayload {
    Vec3 box_bound;
};

struct LineDataPayload {
    Vec3 end_location;
};

struct SphereDataPayload {
    std::byte num_segments;
};

struct ShapeDataPayload {
    std::uint64_t network_id;
    std::optional<ScriptModuleDebugUtilities::ScriptDebugShapeType> shape_type;
    std::optional<Vec3> location;
    std::optional<Vec3> rotation;
    std::optional<float> scale;
    std::optional<mce::Color> color;
    std::optional<float> time_left_total_sec;
    std::optional<DimensionType> dimension_id;
    std::optional<ActorRuntimeID> attached_to_id;
    std::variant<cereal::NullType, ArrowDataPayload, TextDataPayload, BoxDataPayload, LineDataPayload,
                 SphereDataPayload>
        extra_data_payload;
};

struct DebugDrawerPacketPayload {
    std::vector<ShapeDataPayload> shapes;
};

class DebugDrawerPacket : public Packet {
public:
    static constexpr bool SHARE_WITH_HANDLER = false;
    DebugDrawerPacketPayload payload;
    SerializationMode serialization_mode;
};
