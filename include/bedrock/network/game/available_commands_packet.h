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

class AvailableCommandsPacket : public Packet {
public:
    class CommandData;
    class ChainedSubcommandData;
    class EnumData;
    class SoftEnumData;
    class ConstrainedValueData;

    ~AvailableCommandsPacket() override = default;

    int unknown1;                               // +8
    int unknown2;                               // +12
    int unknown3;                               // +16
    int64_t unknown4;                           // +24
    int64_t unknown5;                           // +32
    int unknown6;                               // +40
    std::vector<std::string> enum_names;        // +48
    std::vector<std::string> subcommand_names;  // +72
    char enums[24];                             // +96  std::vector<EnumData>
    std::vector<std::string> postfixes;         // +120
    char chained_subcommands[24];               // +144 std::vector<ChainedSubcommandData>
    char commands[24];                          // +168 std::vector<CommandData>
    char soft_enums[24];                        // +192 std::vector<SoftEnumData>
    char constrained_values[24];                // +216 std::vector<ConstrainedValueData>
};
