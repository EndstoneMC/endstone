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
#include "bedrock/server/commands/command_flag.h"
#include "bedrock/server/commands/command_permission_level.h"

class AvailableCommandsPacketPayload {
public:
    struct EnumData {
        std::string name;
        std::vector<std::uint32_t> values;
    };

    struct ChainedSubcommandData {
        std::string name;
        std::vector<std::pair<std::uint32_t, std::uint32_t>> values;
    };

    struct SoftEnumData {
        std::string name;
        std::vector<std::string> values;
    };

    struct ConstrainedValueData {
        std::uint32_t enum_value_symbol;
        std::uint32_t enum_symbol;
        std::vector<std::uint8_t> constraints;
    };

    struct ParamData {
        std::string name;
        std::uint32_t parse_symbols;
        bool optional;
        std::uint8_t param_options;
    };

    struct OverloadData {
        std::vector<ParamData> params;
        bool is_chaining;
    };

    struct CommandData {
        std::string name;                                        // +0
        std::string description;                                 // +32
        CommandFlagSize flags;                                   // +64
        CommandPermissionLevel permission_level;                 // +66
        std::vector<OverloadData> overloads;                     // +72
        std::vector<std::uint32_t> chained_subcommands_indexes;  // +96
        int alias_enum = -1;                                     // +120
    };
    std::vector<std::string> enum_values;
    std::vector<std::string> postfixes;
    std::vector<EnumData> enums;
    std::vector<std::string> chained_subcommand_values;
    std::vector<ChainedSubcommandData> chained_subcommands;
    std::vector<CommandData> commands;
    std::vector<SoftEnumData> soft_enums;
    std::vector<ConstrainedValueData> constraints;
};

class AvailableCommandsPacket : public Packet {
    [[nodiscard]] MinecraftPacketIds getId() const override
    {
        return MinecraftPacketIds::AvailableCommands;
    }
    [[nodiscard]] std::string_view getName() const override
    {
        return "AvailableCommandsPacket";
    }
    void write(BinaryStream & /*unused*/) const override
    {
        throw std::logic_error("Not implemented");
    }

protected:
    Bedrock::Result<void> _read(ReadOnlyBinaryStream & /*unused*/) override
    {
        throw std::logic_error("Not implemented");
    }

public:
    AvailableCommandsPacketPayload payload;
    SerializationMode serialization_mode;
};
