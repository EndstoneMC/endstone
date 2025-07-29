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
#include <unordered_set>
#include <utility>
#include <vector>

#include <fmt/format.h>

#include "bedrock/bedrock.h"
#include "bedrock/core/utility/type_id.h"
#include "bedrock/network/packet/available_commands_packet.h"
#include "bedrock/platform/brstd/copyable_function.h"
#include "bedrock/server/commands/command_flag.h"
#include "bedrock/server/commands/command_permission_level.h"
#include "bedrock/server/commands/command_version.h"

class Command;
class CommandOrigin;
class CommandParameterData;
class CommandRunStats;

namespace endstone::core {
class EndstoneCommandMap;
class EndstonePlayer;
}  // namespace endstone::core

enum class CommandParameterDataType : int {
    Basic = 0,
    Enum = 1,
    SoftEnum = 2,
    Postfix = 3,
    ChainedSubcommand = 4,
};

enum class SemanticConstraint : std::uint8_t {
    None = 0,
    RequiresCheatsEnabled = 1,
    RequiresElevatedPermissions = 2,
    RequiresHostPermissions = 4,
    RequiresAllowAliases = 8,
    VALUE_MASK = 15,
};

class CommandRegistry {
public:
    friend class Command;
    friend class CommandParameterData;
    friend class endstone::core::EndstoneCommandMap;
    friend class endstone::core::EndstonePlayer;

    using CustomStorageGetFn = void *(*)(Command *, int);
    using CustomStorageIsSetFn = bool *(*)(Command *, int);

    int addEnumValues(const std::string &name, const std::vector<std::string> &values);
    int addSoftEnum(const std::string &name, std::vector<std::string> values);
    void registerCommand(const std::string &name, char const *description, CommandPermissionLevel level,
                         CommandFlag flag1, CommandFlag flag2);
    void registerAlias(std::string name, std::string alias);

private:
    using CommandOverrideFunctor = std::function<void(const std::string &, CommandFlag &, CommandPermissionLevel &)>;

public:
    [[nodiscard]] AvailableCommandsPacket serializeAvailableCommands() const;

private:
    using ScoreboardScoreAccessor = std::function<int(bool &, const std::string &, const Actor &)>;

public:
    void setSoftEnumValues(const std::string &enum_name, std::vector<std::string> values);
    void addSoftEnumValues(const std::string &enum_name, std::vector<std::string> values);

    static const int NonTerminalBit = 0x100000;
    static const int FirstNonTerminal = 0x100001;

    enum class HardNonTerminal {  // Endstone: private -> public
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
    public:
        Symbol() = default;
        Symbol(size_t value) : value_(static_cast<int>(value)) {};
        Symbol(HardNonTerminal value) : value_(static_cast<int>(value)) {};
        Symbol(const Symbol &other)
        {
            value_ = other.value_;
        }

        bool operator==(const Symbol &other) const
        {
            return value_ == other.value_;
        }

        [[nodiscard]] int value() const
        {
            return value_;
        }

        [[nodiscard]] std::size_t toIndex() const
        {
            return value_ & 0xE00FFFFF;
        }

        [[nodiscard]] bool isTerminal() const;
        [[nodiscard]] bool isEnum() const;
        [[nodiscard]] bool isChainedSubCommand() const;
        [[nodiscard]] bool isOptional() const;
        [[nodiscard]] bool isFactorization() const;
        [[nodiscard]] bool isPostfix() const;
        [[nodiscard]] bool isEnumValue() const;
        [[nodiscard]] bool isChainedSubcommandValue() const;
        [[nodiscard]] bool isSoftEnum() const;
        static Symbol fromEnumIndex(size_t index)
        {
            return {index | EnumBit | NonTerminalBit};
        }
        static Symbol fromOptionalIndex(size_t index);
        static Symbol fromFactorizationIndex(size_t index);
        static Symbol fromPostfixIndex(size_t index);
        static Symbol fromEnumValueIndex(size_t index)
        {
            return {index | EnumValueBit};
        }
        static Symbol fromSoftEnumIndex(size_t index);
        static Symbol fromChainedSubcommandIndex(size_t index);
        static Symbol fromChainedSubcommandValueIndex(size_t index);

    private:
        static const int EnumBit = 0x200000;
        static const int OptionalBit = 0x400000;
        static const int FactorizationBit = 0x800000;
        static const int PostfixBit = 0x1000000;
        static const int EnumValueBit = 0x2000000;
        static const int SoftEnumBit = 0x4000000;
        static const int ChainedSubcommandBit = 0x8000000;
        static const int ChainedSubcommandValueBit = 0x10000000;
        int value_ = 0;
    };

    using NonTerminal = Symbol;
    using Terminal = Symbol;
    using SymbolVector = std::vector<Symbol>;

    struct Overload {
        using AllocFunction = brstd::copyable_function<std::unique_ptr<Command>() const>;
        Overload(const CommandVersion &version, AllocFunction alloc);

        CommandVersion version;
        AllocFunction alloc;
        std::vector<CommandParameterData> params;
        std::int32_t version_offset{-1};
        bool is_chaining{false};
        SymbolVector params_symbol;

        static_assert(sizeof(AllocFunction) == 64);
    };

    struct Signature {
        std::string name;                                       // +0
        std::string description;                                // +32
        std::vector<Overload> overloads;                        // +64
        std::vector<std::uint32_t> chained_subcommand_indexes;  // +88
        CommandPermissionLevel permission_level;                // +112
        Symbol command_symbol;                                  // +116
        Symbol command_alias_enum;                              // +120
        CommandFlag flags;                                      // +124
        int first_rule;                                         // +128
        int first_factorization;                                // +132
        int first_optional;                                     // +136
        bool runnable;                                          // +140
        bool has_chained_subcommands;                           // +141
        bool finalized_chained_subcommands;                     // +142
        std::int64_t rule_counter;                              // +144
    };

    struct ParseToken {
        std::unique_ptr<ParseToken> child;  // +0
        std::unique_ptr<ParseToken> next;   // +8
        ParseToken *parent;                 // +16
        char const *text;                   // +24
        std::uint32_t length;               // +32
        Symbol type;                        // +36
    };
    BEDROCK_STATIC_ASSERT_SIZE(CommandRegistry::ParseToken, 40, 40);

    struct ParseRule;
    struct OptionalParameterChain;
    struct Factorization;
    using ParseFunction = bool (CommandRegistry::*)(void *, const ParseToken &, const CommandOrigin &, int,
                                                    std::string &, std::vector<std::string> &) const;
    struct Enum {
        std::string name;
        Bedrock::typeid_t<CommandRegistry> type;
        ParseFunction parse;
        std::vector<std::pair<std::uint32_t, std::uint32_t>> values;
    };
    struct ChainedSubcommand;
    struct SoftEnum {
        std::string name;
        std::vector<std::string> values;
    };
    struct ConstrainedValue {
        Symbol value;
        Symbol enum_symbol;
        std::vector<unsigned char> constraints;
    };
    struct ParamSymbols {
        Terminal x;              // +0
        Terminal y;              // +4
        Terminal z;              // +8
        Terminal dx;             // +12
        Terminal dy;             // +16
        Terminal dz;             // +20
        Terminal r;              // +24
        Terminal rm;             // +28
        Terminal rx;             // +32
        Terminal rxm;            // +36
        Terminal ry;             // +40
        Terminal rym;            // +44
        Terminal l;              // +48
        Terminal lm;             // +52
        Terminal c;              // +56
        Terminal m;              // +60
        Terminal name;           // +64
        Terminal type;           // +68
        Terminal family;         // +72
        Terminal score;          // +76
        Terminal tag;            // +80
        Terminal hasitem;        // +84
        Terminal haspermission;  // +88
        Terminal hasproperty;    // +92
        Terminal codebuilder;    // +96
    };
    BEDROCK_STATIC_ASSERT_SIZE(ParamSymbols, 100, 100);

    struct RegistryState;

    // Endstone begins
    friend class endstone::core::EndstoneCommandMap;
    [[nodiscard]] std::string describe(const Signature &signature, const Overload &overload) const
    {
        return describe(signature, signature.name, overload, 0, nullptr, nullptr);
    }

    template <typename T>
    bool parse(void *storage, const ParseToken &token, const CommandOrigin &origin, int version, std::string &error,
               std::vector<std::string> &error_params) const;

    template <typename CommandType>
    static std::unique_ptr<Command> allocateCommand()
    {
        return std::move(std::make_unique<CommandType>());
    }

    template <typename CommandType>
    const Overload *registerOverload(const char *name, CommandVersion version,
                                     std::vector<CommandParameterData> params);
    // Endstone ends

private:
    struct ParseTable;

    [[nodiscard]] const Signature *findCommand(const std::string &name) const;
    [[nodiscard]] NonTerminal findEnum(const std::string &name) const;
    [[nodiscard]] NonTerminal findSoftEnum(const std::string &name) const;
    [[nodiscard]] Terminal findEnumValue(const std::string &name) const;
    [[nodiscard]] NonTerminal findChainedSubcommand(const std::string &name) const;
    [[nodiscard]] NonTerminal findChainedSubcommandValue(const std::string &name) const;
    [[nodiscard]] Terminal findPostfix(const std::string &name) const;
    [[nodiscard]] Terminal findIdentifierInfo(const std::string &name) const;
    [[nodiscard]] std::unique_ptr<Command> createCommand(const ParseToken &parse_token, const CommandOrigin &origin,
                                                         int version, std::string &error_message,
                                                         std::vector<std::string> &error_params) const;
    [[nodiscard]] std::string describe(CommandParameterData const &) const;
    [[nodiscard]] std::string describe(const Signature &command, const std::string &alias, const Overload &overload,
                                       unsigned int highlight, unsigned int *start, unsigned int *length) const;
    [[nodiscard]] std::string symbolToString(Symbol) const;
    void registerOverloadInternal(Signature &signature, Overload &overload);

    std::function<void(Packet const &)> network_update_callback_;                                // +0
    ScoreboardScoreAccessor get_score_for_objective_;                                            // +56
    bool is_edu_mode_;                                                                           // +128
    std::vector<ParseRule> rules_;                                                               // +136
    using ParseTableMap = std::map<std::uint32_t, ParseTable>;                                   //
    ParseTableMap parse_tables_;                                                                 // +160
    std::vector<OptionalParameterChain> optionals_;                                              // +176
    std::vector<std::string> enum_values_;                                                       // +200
    std::vector<Enum> enums_;                                                                    // +224
    std::vector<std::string> chained_subcommand_values_;                                         // +248
    std::vector<ChainedSubcommand> chained_subcommands_;                                         // +272
    std::vector<Factorization> factorizations_;                                                  // +296
    std::vector<std::string> postfixes_;                                                         // +320
    std::map<std::string, std::uint32_t> enum_lookup_;                                           // +344
    std::map<std::string, std::uint32_t> enum_value_lookup_;                                     // +360
    std::map<std::string, std::uint32_t> chained_subcommand_lookup_;                             // +376
    std::map<std::string, std::uint32_t> chained_subcommand_value_lookup_;                       // +392
    std::vector<Symbol> command_symbols_;                                                        // +408
    std::map<std::string, Signature> signatures_;                                                // +432
    std::map<Bedrock::typeid_t<CommandRegistry>, std::int32_t> type_lookup_;                     // +448
    std::map<std::string, std::string> aliases_;                                                 // +464
    std::vector<SemanticConstraint> semantic_constraints_;                                       // +480
    std::map<SemanticConstraint, std::uint8_t> semantic_constraint_lookup_;                      // +504
    std::vector<ConstrainedValue> constrained_values_;                                           // +520
    std::map<std::pair<std::uint64_t, std::uint32_t>, std::uint32_t> constrained_value_lookup_;  // +544
    std::vector<SoftEnum> soft_enums_;                                                           // +560
    std::map<std::string, std::uint32_t> soft_enum_lookup_;                                      // +584
    std::vector<RegistryState> state_stack_;                                                     // +600
    ParamSymbols args_;                                                                          // +624
    std::unordered_set<int> skip_on_eps_autocomplete_symbols_;                                   // +728
    std::unordered_set<int> allow_empty_symbols_;                                                // +792
    CommandOverrideFunctor command_override_functor_;
    std::unique_ptr<CommandRunStats> command_run_stats_;
};

enum class CommandParameterOption : std::uint8_t {
    None = 0,
    EnumAutocompleteExpansion = 1,
    HasSemanticConstraint = 2,
    EnumAsChainedCommand = 4
};

class CommandParameterData {
    using ParseFunction = CommandRegistry::ParseFunction;
    using CustomStorageGetFn = CommandRegistry::CustomStorageGetFn;
    using CustomStorageIsSetFn = CommandRegistry::CustomStorageIsSetFn;

public:
    CommandParameterData(Bedrock::typeid_t<CommandRegistry> type_index, ParseFunction parse, char const *name,
                         CommandParameterDataType param_type, char const *enum_name_or_postfix,
                         char const *chained_subcommand, int offset, bool is_optional, int set_offset)
        : type_index(type_index), parse(parse), name(name), enum_name_or_postfix(enum_name_or_postfix),
          chained_subcommand(chained_subcommand), param_type(param_type), offset(offset), set_offset(set_offset),
          is_optional(is_optional)
    {
    }
    CommandParameterData(Bedrock::typeid_t<CommandRegistry>, ParseFunction, const char *, int, bool, CustomStorageGetFn,
                         CustomStorageIsSetFn);

    Bedrock::typeid_t<CommandRegistry> type_index;                 // +0
    ParseFunction parse;                                           // +8
    std::string name;                                              // +16
    const char *enum_name_or_postfix;                              // +48
    int enum_or_postfix_symbol{-1};                                // +56
    const char *chained_subcommand;                                // +64
    int chained_subcommand_symbol{-1};                             // +72
    CommandParameterDataType param_type;                           // +76
    int offset;                                                    // +80
    int set_offset;                                                // +84
    bool is_optional;                                              // +88
    CommandParameterOption options{CommandParameterOption::None};  // +89
    CustomStorageGetFn value_get_fn{nullptr};
    CustomStorageIsSetFn value_is_set_fn{nullptr};
};

template <typename CommandType>
const CommandRegistry::Overload *CommandRegistry::registerOverload(const char *name, CommandVersion version,
                                                                   std::vector<CommandParameterData> params)
{
    auto *signature = const_cast<Signature *>(findCommand(name));
    if (!signature) {
        return nullptr;
    }

    auto overload = Overload(version, &allocateCommand<CommandType>);
    overload.params = std::move(params);

    signature->overloads.push_back(overload);
    registerOverloadInternal(*signature, overload);
    return &signature->overloads.back();
}

template <>
struct fmt::formatter<CommandRegistry::ParseToken> : formatter<string_view> {
    auto format(const CommandRegistry::ParseToken &token, format_context &ctx) const -> format_context::iterator
    {
        auto out = fmt::format_to(ctx.out(), "[");
        for (const auto *it = &token; it; it = it->next.get()) {
            if (it != &token) {
                out = fmt::format_to(out, ", ");
            }
            out = fmt::format_to(out, R"({{"type": "{}")", it->type.value());
            if (it->text != nullptr && it->length > 0) {
                std::string_view text(it->text, it->length);
                if (it->type == CommandRegistry::HardNonTerminal::Id && text.size() >= 2 && text.front() == '"' &&
                    text.back() == '"') {
                    text = text.substr(1, text.size() - 2);
                }
                out = fmt::format_to(out, R"(, "text": "{}")", text);
            }
            if (it->child != nullptr) {
                out = fmt::format_to(out, R"(, "children": {})", *it->child);
            }
            out = fmt::format_to(out, "}}");
        }
        out = fmt::format_to(ctx.out(), "]");
        return out;
    }
};  // namespace fmt
