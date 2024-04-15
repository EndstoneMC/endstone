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

#include "bedrock/command/command_flag.h"
#include "bedrock/network/protocol/packet.h"

class AvailableCommandsPacket : public Packet {
public:
    struct OverloadData;
    struct ChainedSubcommandData;
    struct EnumData;
    struct SoftEnumData;
    struct ConstrainedValueData;
    struct CommandData {
        std::string name;                         // +0
        std::string description;                  // +32
        CommandFlag command_flag;                 // +64
        CommandPermissionLevel permission_level;  // +66
        std::vector<void *> overloads;            // +72 std::vector<OverloadData>
        std::vector<int> subcommand_values;       // +96
        int enum_symbol_index = -1;               // +120
    };
    // static_assert(sizeof(CommandData) == 128);

    ~AvailableCommandsPacket() override = default;

    std::vector<std::string> enum_names;        // +48
    std::vector<std::string> subcommand_names;  // +72
    std::vector<void *> enums;                  // +96  std::vector<EnumData>
    std::vector<std::string> postfixes;         // +120
    std::vector<void *> chained_subcommands;    // +144 std::vector<ChainedSubcommandData>
    std::vector<CommandData> commands;          // +168 std::vector<CommandData>
    std::vector<void *> soft_enums;             // +192 std::vector<SoftEnumData>
    std::vector<void *> constrained_values;     // +216 std::vector<ConstrainedValueData>
};
