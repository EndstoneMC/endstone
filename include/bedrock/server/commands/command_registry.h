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
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/core/utility/type_id.h"
#include "bedrock/network/packet/available_commands_packet.h"
#include "bedrock/server/commands/command_flag.h"
#include "bedrock/server/commands/command_origin.h"
#include "bedrock/server/commands/command_permission_level.h"
#include "bedrock/server/commands/command_version.h"

enum SemanticConstraint : std::uint8_t;

class Command;
class CommandParameterData;

class CommandRegistry {
public:
    static constexpr int NonTerminalBit = 0x100000;
    static constexpr int FirstNonTerminal = 0x100001;
    enum class HardNonTerminal : std::uint32_t {
        Epsilon = NonTerminalBit,
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
        CodeBuilderArg = 0x100058,
        CodeBuilderArgs = 0x100059,
        CodeBuilderSelectParam = 0x10005a,
        CodeBuilderSelector = 0x10005b,
    };

    class Symbol {
        static constexpr int EnumBit = 0x200000;
        static constexpr int OptionalBit = 0x400000;
        static constexpr int FactorizationBit = 0x800000;
        static constexpr int PostfixBit = 0x1000000;
        static constexpr int EnumValueBit = 0x2000000;
        static constexpr int SoftEnumBit = 0x4000000;
        static constexpr int ChainedSubcommandBit = 0x8000000;
        static constexpr int ChainedSubcommandValueBit = 0x10000000;

    public:
        explicit Symbol(std::uint32_t value = 0) : value_(value){};
        explicit Symbol(HardNonTerminal value) : value_(static_cast<std::uint32_t>(value)){};

        [[nodiscard]] std::uint32_t value() const
        {
            return value_;
        }

        [[nodiscard]] std::uint64_t toIndex() const
        {
            return value_ & 0xE00FFFFF;
        }

    private:
        std::uint32_t value_;
    };

    struct Overload {
        using AllocFunction = std::unique_ptr<Command> (*)();

        Overload(const CommandVersion &version, AllocFunction alloc) : version(version), alloc(alloc) {}

        CommandVersion version;                    // +0
        AllocFunction alloc;                       // +8
        std::vector<CommandParameterData> params;  // +16
        std::int32_t version_offset{-1};           // +40
        bool chaining{false};                      // +44
        std::vector<Symbol> params_symbol;         // +48
    };

    struct Signature {
        std::string name;                                              // +0
        std::string description;                                       // +32
        std::vector<Overload> overloads;                               // +64
        std::vector<std::uint32_t> chained_subcommand_indexes_values;  // +88
        CommandPermissionLevel permission_level;                       // +112
        Symbol command_symbol;                                         // +116
        Symbol command_alias_symbol;                                   // +120
        CommandFlag flags;                                             // +124
        int first_rule;                                                // +128
        int first_factorization;                                       // +132
        int first_optional;                                            // +136
        bool runnable;                                                 // +140
        bool has_chained_subcommands;                                  // +141
        bool finalized_chained_subcommands_subcommands;                // +142
        std::int64_t rule_counter_;                                    // +144
    };

    struct ParseToken {
        std::unique_ptr<ParseToken> child;  // +0
        std::unique_ptr<ParseToken> next;   // +8
        ParseToken *parent;                 // +16
        char const *text;                   // +24
        std::uint32_t length;               // +32
        Symbol type;                        // +36

        friend std::ostream &operator<<(std::ostream &os, const ParseToken &token);
    };
    BEDROCK_STATIC_ASSERT_SIZE(CommandRegistry::ParseToken, 40, 40);

    using ParseFunction = bool (CommandRegistry::*)(void *, const ParseToken &, const CommandOrigin &, int,
                                                    std::string &, std::vector<std::string> &) const;
    using CommandOverrideFunctor = std::function<void(CommandFlag &, std::string const &)>;

    template <typename T>
    bool parse(void *value, const ParseToken &parse_token, const CommandOrigin &, int, std::string &,
               std::vector<std::string> &) const;

    class ParseTable;
    struct Enum {
        std::string name;                                             // +0
        Bedrock::typeid_t<CommandRegistry> type;                      // +32
        ParseFunction parse;                                          // +40
        std::vector<std::pair<std::uint64_t, std::uint64_t>> values;  // +48
    };
    class SoftEnum;
    class OptionalParameterChain;
    class ConstrainedValue;
    class RegistryState;
    struct ChainedSubcommand;
    struct Factorization;

    ENDSTONE_HOOK void registerCommand(const std::string &name, char const *description, CommandPermissionLevel level,
                                       CommandFlag flag1, CommandFlag flag2);
    ENDSTONE_HOOK void registerAlias(std::string name, std::string alias);
    ENDSTONE_HOOK int addEnumValues(const std::string &name, const std::vector<std::string> &values);
    [[nodiscard]] ENDSTONE_HOOK AvailableCommandsPacket serializeAvailableCommands() const;

    template <typename CommandType>
    static std::unique_ptr<Command> allocateCommand()
    {
        return std::move(std::make_unique<CommandType>());
    }

    template <typename CommandType>
    const Overload *registerOverload(const char *name, CommandVersion version, std::vector<CommandParameterData> params)
    {
        auto *signature = const_cast<Signature *>(findCommand(name));
        if (!signature) {
            return nullptr;
        }

        auto overload = Overload(version, allocateCommand<CommandType>);
        overload.params = std::move(params);

        signature->overloads.push_back(overload);
        registerOverloadInternal(*signature, overload);
        return &signature->overloads.back();
    }

    std::string describe(const Signature &signature, const Overload &overload)
    {
        return describe(signature, signature.name, overload, 0, nullptr, nullptr);
    }

    std::function<void(Packet const &)> network_update_callback;                                // +0
    std::function<int(bool &, std::string const &, Actor const &)> get_score_for_objective;     // +56
    bool edu_mode;                                                                              // +128
    std::vector<ParseFunction> rules;                                                           // +136
    std::map<std::uint32_t, ParseTable> parse_tables;                                           // +160
    std::vector<OptionalParameterChain> optionals;                                              // +176
    std::vector<std::string> enum_values;                                                       // +200
    std::vector<Enum> enums;                                                                    // +224
    std::vector<std::string> chained_subcommand_values;                                         // +248
    std::vector<ChainedSubcommand> chained_subcommands;                                         // +272
    std::vector<Factorization> factorizations;                                                  // +296
    std::vector<std::string> postfixes;                                                         // +320
    std::map<std::string, std::uint32_t> enum_lookup;                                           // +344
    std::map<std::string, std::uint64_t> enum_value_lookup;                                     // +360
    std::map<std::string, std::uint32_t> chained_subcommand_lookup;                             // +376
    std::map<std::string, std::uint64_t> chained_subcommand_value_lookup;                       // +392
    std::vector<Symbol> command_symbols;                                                        // +408
    std::map<std::string, Signature> signatures;                                                // +432
    std::map<Bedrock::typeid_t<CommandRegistry>, std::int32_t> type_lookup;                     // +448
    std::map<std::string, std::string> aliases;                                                 // +464
    std::vector<SemanticConstraint> semantic_constraints;                                       // +480
    std::map<SemanticConstraint, std::uint8_t> semantic_constraint_lookup;                      // +504
    std::vector<ConstrainedValue> constrained_values;                                           // +520
    std::map<std::pair<std::uint64_t, std::uint32_t>, std::uint32_t> constrained_value_lookup;  // +544
    std::vector<SoftEnum> soft_enums;                                                           // +560
    std::map<std::string, std::uint32_t> soft_enum_lookup;                                      // +584
    std::vector<RegistryState> state_stack;                                                     // +600
    char param_symbols[100];                                                                    // +624
    std::unordered_set<int> skip_on_eps_autocomplete_symbols;                                   // +728
    std::unordered_set<int> allow_empty_symbols;                                                // +792
    CommandOverrideFunctor command_override_functor;

private:
    [[nodiscard]] ENDSTONE_HOOK const Signature *findCommand(const std::string &name) const;
    [[nodiscard]] ENDSTONE_HOOK std::unique_ptr<Command> createCommand(const ParseToken &parse_token,
                                                                       const CommandOrigin &origin, int version,
                                                                       std::string &error_message,
                                                                       std::vector<std::string> &error_params) const;
    [[nodiscard]] ENDSTONE_HOOK std::string describe(CommandParameterData const &) const;
    [[nodiscard]] ENDSTONE_HOOK std::string describe(const Signature &signature, const std::string &name,
                                                     const Overload &overload, unsigned int a4, unsigned int *a5,
                                                     unsigned int *a6) const;

    ENDSTONE_HOOK void registerOverloadInternal(Signature &signature, Overload &overload);
};

enum class CommandParameterDataType : int {
    Basic = 0,
    Enum = 1,
    SoftEnum = 2,
    Postfix = 3
};

enum class CommandParameterOption : std::uint8_t {
    None = 0,
    EnumAutocompleteExpansion = 1,
    HasSemanticConstraint = 2,
    EnumAsChainedCommand = 4
};

class CommandParameterData {
public:
    CommandParameterData(Bedrock::typeid_t<CommandRegistry> type, CommandRegistry::ParseFunction parse,
                         char const *name, CommandParameterDataType param_type, char const *enum_name,
                         char const *fallback_typename, int offset, bool is_optional, int set_offset)
        : type(type), parse(parse), name(name), enum_name(enum_name), fallback_typename(fallback_typename),
          param_type(param_type), offset(offset), set_offset(set_offset), is_optional(is_optional)
    {
    }

    Bedrock::typeid_t<CommandRegistry> type;                             // +0
    CommandRegistry::ParseFunction parse;                                // +8
    std::string name;                                                    // +16
    const char *enum_name;                                               // +48
    CommandRegistry::Symbol enum_symbol{static_cast<uint32_t>(-1)};      // +56
    const char *fallback_typename;                                       // +64
    CommandRegistry::Symbol fallback_symbol{static_cast<uint32_t>(-1)};  // +72
    CommandParameterDataType param_type;                                 // +76
    int offset;                                                          // +80
    int set_offset;                                                      // +84
    bool is_optional;                                                    // +88
    CommandParameterOption options{CommandParameterOption::None};        // +89
};

namespace fmt {

template <>
struct formatter<CommandRegistry::ParseToken> : formatter<string_view> {
    using Type = CommandRegistry::ParseToken;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) -> format_context::iterator
    {
        std::stack<std::pair<const CommandRegistry::ParseToken *, int>> to_visit;
        auto out = ctx.out();

        to_visit.emplace(&val, 0);
        while (!to_visit.empty()) {
            auto node_level = to_visit.top();
            to_visit.pop();

            const auto *node = node_level.first;
            auto level = node_level.second;

            for (int i = 0; i < level * 4; ++i) {
                *out++ = ' ';
            }

            if (node) {
                out = format_to(out, "Symbol: 0x{:x}", node->type.value());
                if (node->length > 0) {
                    out = format_to(out, ", Data: {}", std::string(node->text, node->length));
                }
                out = format_to(out, "\n");

                if (node->next) {
                    to_visit.emplace(node->next.get(), level);
                }
                if (node->child) {
                    to_visit.emplace(node->child.get(), level + 1);
                }
            }
        }

        return out;
    }
};
}  // namespace fmt
