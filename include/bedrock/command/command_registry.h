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
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/command/command_flag.h"
#include "bedrock/command/command_origin.h"
#include "bedrock/command/command_permission_level.h"
#include "bedrock/command/command_version.h"
#include "bedrock/network/protocol/game/available_commands_packet.h"
#include "bedrock/type_id.h"

enum SemanticConstraint : std::uint8_t;

class Command;
class CommandParameterData;

class CommandRegistry {
public:
    enum class HardNonTerminal : std::uint32_t {
        // Epsilon = NonTerminalBit,
        Int = 0x100001,
        Float = 0x100002,
        Val = 0x100003,
        RVal = 0x100004,
        WildcardInt = 0x100005,
        Operator = 0x100006,
        CompareOperator = 0x100007,
        Selection = 0x100008,
        StandaloneSelection = 0x100009,
        WildcardSelection = 0x10000a,
        NonIdSelector = 0x10000b,
        ScoresArg = 0x10000c,
        ScoresArgs = 0x10000d,
        ScoreSelectParam = 0x10000e,
        ScoreSelector = 0x10000f,
        TagSelector = 0x100010,
        FilePath = 0x100011,
        FilePathVal = 0x100012,
        FilePathCont = 0x100013,
        IntegerRangeVal = 0x100014,
        IntegerRangePostVal = 0x100015,
        IntegerRange = 0x100016,
        FullIntegerRange = 0x100017,
        RationalRangeVal = 0x100018,
        RationalRangePostVal = 0x100019,
        RationalRange = 0x10001a,
        FullRationalRange = 0x10001b,
        SelArgs = 0x10001c,
        Args = 0x10001d,
        Arg = 0x10001e,
        MArg = 0x10001f,
        MValue = 0x100020,
        NameArg = 0x100021,
        TypeArg = 0x100022,
        FamilyArg = 0x100023,
        HasPermissionArg = 0x100024,
        HasPermissionArgs = 0x100025,
        HasPermissionSelector = 0x100026,
        HasPermissionElement = 0x100027,
        HasPermissionElements = 0x100028,
        TagArg = 0x100029,
        HasItemElement = 0x10002a,
        HasItemElements = 0x10002b,
        HasItemArg = 0x10002c,
        HasItemArgs = 0x10002d,
        HasItemSelector = 0x10002e,
        EquipmentSlotEnum = 0x10002f,
        PropertyValue = 0x100030,
        HasPropertyParamValue = 0x100031,
        HasPropertyParamEnumValue = 0x100032,
        HasPropertyArg = 0x100033,
        HasPropertyArgs = 0x100034,
        HasPropertyElement = 0x100035,
        HasPropertyElements = 0x100036,
        HasPropertySelector = 0x100037,
        Id = 0x100038,
        IdCont = 0x100039,
        CoordXInt = 0x10003a,
        CoordYInt = 0x10003b,
        CoordZInt = 0x10003c,
        CoordXFloat = 0x10003d,
        CoordYFloat = 0x10003e,
        CoordZFloat = 0x10003f,
        Position = 0x100040,
        PositionFloat = 0x100041,
        MessageExp = 0x100042,
        Message = 0x100043,
        MessageRoot = 0x100044,
        PostSelector = 0x100045,
        RawText = 0x100046,
        RawTextCont = 0x100047,
        JsonValue = 0x100048,
        JsonField = 0x100049,
        JsonObject = 0x10004a,
        JsonObjectFields = 0x10004b,
        JsonObjectCont = 0x10004c,
        JsonArray = 0x10004d,
        JsonArrayValues = 0x10004e,
        JsonArrayCont = 0x10004f,
        BlockState = 0x100050,
        BlockStateKey = 0x100051,
        BlockStateValue = 0x100052,
        BlockStateValues = 0x100053,
        BlockStateArray = 0x100054,
        BlockStateArrayCont = 0x100055,
        Command = 0x100056,
        SlashCommand = 0x100057,
    };

    struct Overload {
        Overload(const CommandVersion &version, std::unique_ptr<Command> (*factory)())
            : version(version), factory(factory)
        {
        }

        CommandVersion version;                    // +0
        std::unique_ptr<Command> (*factory)();     // +8
        std::vector<CommandParameterData> params;  // +16
        std::int32_t unknown1{-1};                 // +40
        std::int8_t unknown2{0};                   // +44
        std::uint64_t unknown3{0};                 // +48
        std::uint64_t unknown4{0};                 // +56
        std::uint64_t unknown5{0};                 // +64
    };

    class Symbol {
    public:
        explicit Symbol(std::uint32_t value = 0) : value_(value){};
        explicit Symbol(CommandRegistry::HardNonTerminal value) : value_(static_cast<std::uint32_t>(value)){};

        [[nodiscard]] std::uint32_t value() const
        {
            return value_;
        }

    private:
        std::uint32_t value_;
    };

    struct Signature {
        std::string name;                                  // +0
        std::string description;                           // +32
        std::vector<CommandRegistry::Overload> overloads;  // +64
        std::vector<int> subcommand_values;                // +88
        CommandPermissionLevel permission_level;           // +112
        CommandRegistry::Symbol symbol;                    // +116
        CommandRegistry::Symbol enum_symbol;               // +120
        CommandFlag command_flag;                          // +124
        int unknown3;                                      // +128
        int symbol_index;                                  // +132
        int optional_index;                                // +136
        char unknown6;                                     // +140
        std::int64_t unknown7;                             // +144
    };

    struct ParseToken {
        std::unique_ptr<ParseToken> child;  // +0
        std::unique_ptr<ParseToken> next;   // +8
        ParseToken *parent;                 // +16
        char const *data;                   // +24
        std::uint32_t size;                 // +32
        Symbol symbol;                      // +36

        friend std::ostream &operator<<(std::ostream &os, const ParseToken &token);
    };
    static_assert(sizeof(CommandRegistry::ParseToken) == 40);

    using ParseRule = bool (CommandRegistry::*)(void *, const CommandRegistry::ParseToken &, const CommandOrigin &, int,
                                                std::string &, std::vector<std::string> &) const;

    template <typename T>
    bool parse(void *value, const CommandRegistry::ParseToken &parse_token, const CommandOrigin &, int, std::string &,
               std::vector<std::string> &) const;

    class ParseTable;
    class Enum {
    public:
        std::string name;                                             // +0
        Bedrock::typeid_t<CommandRegistry> type_id;                   // +32
        ParseRule parse_rule;                                         // +40
        std::vector<std::pair<std::uint64_t, std::uint64_t>> values;  // +48
    };
    class SoftEnum;

    BEDROCK_API void registerCommand(const std::string &name, char const *description, CommandPermissionLevel level,
                                     CommandFlag flag1, CommandFlag flag2);
    BEDROCK_API void registerAlias(std::string name, std::string alias);
    BEDROCK_API int addEnumValues(const std::string &name, const std::vector<std::string> &values);
    [[nodiscard]] BEDROCK_API AvailableCommandsPacket serializeAvailableCommands() const;

    template <typename CommandType>
    static std::unique_ptr<Command> allocateCommand()
    {
        return std::move(std::make_unique<CommandType>());
    }

    template <typename CommandType>
    const CommandRegistry::Overload *registerOverload(const char *name, CommandVersion version,
                                                      std::vector<CommandParameterData> params)
    {
        auto *signature = const_cast<CommandRegistry::Signature *>(findCommand(name));
        if (!signature) {
            return nullptr;
        }

        auto overload = CommandRegistry::Overload(version, CommandRegistry::allocateCommand<CommandType>);
        overload.params = std::move(params);

        signature->overloads.push_back(overload);
        registerOverloadInternal(*signature, overload);
        return &signature->overloads.back();
    }

    std::string describe(const CommandRegistry::Signature &signature, const CommandRegistry::Overload &overload)
    {
        return describe(signature, signature.name, overload, 0, nullptr, nullptr);
    }

    std::function<void(class Packet const &)> network_update_callback;                      // +0
    std::function<int(bool &, std::string const &, class Actor const &)> score_callback;    // +56
    std::vector<void *> unknown1;                                                           // +128
    std::map<std::uint32_t, CommandRegistry::ParseTable> parse_tables;                      // +152
    std::vector<void *> optionals;                                                          // +168
    std::vector<std::string> literals;                                                      // +192
    std::vector<CommandRegistry::Enum> enums;                                               // +216
    std::vector<std::string> subcommands;                                                   // +240
    std::vector<CommandRegistry::Enum> chained_subcommands;                                 // +264
    std::vector<CommandRegistry::Symbol> symbols;                                           // +288
    std::vector<std::string> postfixes;                                                     // +312
    std::map<std::string, int> enum_symbol_index;                                           // +336
    std::map<std::string, CommandRegistry::Symbol> enum_symbols;                            // +352
    std::map<std::string, int> subcommand_symbol_index;                                     // +368
    std::map<std::string, CommandRegistry::Symbol> subcommand_symbols;                      // +384
    std::vector<CommandRegistry::Symbol> command_symbols;                                   // +400
    std::map<std::string, CommandRegistry::Signature> commands;                             // +424
    std::map<Bedrock::typeid_t<CommandRegistry>, int> type_ids;                             // +440
    std::map<std::string, std::string> aliases;                                             // +456
    std::vector<SemanticConstraint> semantic_constraints;                                   // +472
    std::map<SemanticConstraint, unsigned char> constrained_values;                         // +496
    std::vector<void *> constrained_value_data;                                             // +512
    std::map<std::pair<std::uint64_t, std::uint32_t>, std::uint32_t> unknown8;              // +536
    std::vector<CommandRegistry::SoftEnum> soft_enums;                                      // +552
    std::map<std::string, int> soft_enum_symbol_index;                                      // +576
    std::vector<void *> unknown10;                                                          // +592
    char param_symbols[96];                                                                 // +616
    std::unordered_map<unsigned char, unsigned char> unknown11;                             // +712
    std::unordered_map<unsigned char, unsigned char> unknown12;                             // +776
    std::function<void(CommandFlag &, std::string const &)> command_registration_override;  // +840

private:
    [[nodiscard]] BEDROCK_API const CommandRegistry::Signature *findCommand(const std::string &name) const;
    [[nodiscard]] BEDROCK_API std::unique_ptr<Command> createCommand(const CommandRegistry::ParseToken &parse_token,
                                                                     const CommandOrigin &origin, int version,
                                                                     std::string &error_message,
                                                                     std::vector<std::string> &error_params) const;
    [[nodiscard]] BEDROCK_API std::string describe(CommandParameterData const &) const;
    [[nodiscard]] BEDROCK_API std::string describe(const CommandRegistry::Signature &signature, const std::string &name,
                                                   const CommandRegistry::Overload &overload, unsigned int a4,
                                                   unsigned int *a5, unsigned int *a6) const;

    BEDROCK_API void registerOverloadInternal(CommandRegistry::Signature &signature,
                                              CommandRegistry::Overload &overload);
};

enum CommandParameterDataType : int {
    Default = 0,
    Enum = 1,
};

enum class CommandParameterOption : char {
    None = 0,
    EnumAutocompleteExpansion = 0x01,
    HasSemanticConstraint = 0x02,
    EnumAsChainedCommand = 0x04
};

struct CommandParameterData {
    CommandParameterData(Bedrock::typeid_t<CommandRegistry> type_id, CommandRegistry::ParseRule parse_rule,
                         char const *name, CommandParameterDataType type, char const *type_name,
                         char const *fallback_typename, int offset_value, bool optional, int offset_has_value)
        : type_id(type_id), parse_rule(parse_rule), name(name), enum_name(type_name),
          fallback_typename(fallback_typename), type(type), offset_value(offset_value),
          offset_has_value(offset_has_value), optional(optional)
    {
    }

    Bedrock::typeid_t<CommandRegistry> type_id;                          // +0
    CommandRegistry::ParseRule parse_rule;                               // +8
    std::string name;                                                    // +16
    const char *enum_name;                                               // +48
    CommandRegistry::Symbol enum_symbol{static_cast<uint32_t>(-1)};      // +56
    const char *fallback_typename;                                       // +64
    CommandRegistry::Symbol fallback_symbol{static_cast<uint32_t>(-1)};  // +72
    CommandParameterDataType type;                                       // +76
    int offset_value;                                                    // +80
    int offset_has_value;                                                // +84
    bool optional;                                                       // +88
    CommandParameterOption option{0};                                    // +89
};
