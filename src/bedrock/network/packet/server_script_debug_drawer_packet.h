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

#include "bedrock/core/math/color.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/network/packet.h"

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

struct PacketShapeData {
    std::optional<ScriptDebugShapeType> shape_type;
    std::optional<Vec3> location;
    std::optional<Vec3> rotation;
    std::optional<float> scale;
    std::optional<mce::Color> color;
    std::optional<float> time_left_total_sec;
    std::optional<std::string> text;
    std::optional<Vec3> box_bound;
    std::optional<Vec3> end_location;
    std::optional<float> arrow_head_length;
    std::optional<float> arrow_head_radius;
    std::optional<std::uint8_t> num_segments;
};

class ServerScriptDebugDrawerPacket : public Packet {
public:
    std::vector<PacketShapeData> shapes;
};
}  // namespace ScriptModuleDebugUtilities
