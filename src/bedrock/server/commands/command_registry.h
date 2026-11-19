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

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <format>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <stack>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include <entt/container/dense_map.hpp>
#include <entt/container/dense_set.hpp>

#include "bedrock/bedrock.h"
#include "bedrock/core/utility/type_id.h"
#include "bedrock/forward.h"
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
class CommandPermissions;
class EndstoneCommandMap;
class EndstonePlayer;
class MinecraftCommandPermissions;
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
    void removeSoftEnumValues(const std::string &enum_name, std::vector<std::string> values);

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
        ClockTimeMarkerName = 0x100056,
        PoiTag = 0x100057,
        PoiType = 0x100058,
        Command = 0x100059,
        SlashCommand = 0x10005a,
        CodeBuilderArg = 0x10005b,
        CodeBuilderArgs = 0x10005c,
        CodeBuilderSelectParam = 0x10005d,
        CodeBuilderSelector = 0x10005e,
    };

    class Symbol {
    public:
        Symbol() = default;
        Symbol(size_t value) : value_(static_cast<int>(value)) {};
        Symbol(HardNonTerminal value) : value_(static_cast<int>(value)) {};
        Symbol(const Symbol &other) { value_ = other.value_; }

        bool operator==(const Symbol &other) const { return value_ == other.value_; }

        [[nodiscard]] int value() const { return value_; }

        [[nodiscard]] std::size_t toIndex() const { return value_ & 0xE00FFFFF; }

        [[nodiscard]] bool isTerminal() const;
        [[nodiscard]] bool isEnum() const;
        [[nodiscard]] bool isChainedSubCommand() const;
        [[nodiscard]] bool isOptional() const;
        [[nodiscard]] bool isFactorization() const;
        [[nodiscard]] bool isPostfix() const;
        [[nodiscard]] bool isEnumValue() const;
        [[nodiscard]] bool isChainedSubcommandValue() const;
        [[nodiscard]] bool isSoftEnum() const;
        static Symbol fromEnumIndex(size_t index) { return {index | EnumBit | NonTerminalBit}; }
        static Symbol fromOptionalIndex(size_t index);
        static Symbol fromFactorizationIndex(size_t index);
        static Symbol fromPostfixIndex(size_t index);
        static Symbol fromEnumValueIndex(size_t index) { return {index | EnumValueBit}; }
        static Symbol fromSoftEnumIndex(size_t index) { return {index | SoftEnumBit | NonTerminalBit}; }
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

    struct SymbolHasher {
        std::size_t operator()(const Symbol &s) const;
    };
    struct SymbolPairHasher {
        std::size_t operator()(const std::pair<Symbol, Symbol> &s) const;
    };

    using NonTerminal = Symbol;
    using Terminal = Symbol;
    using SymbolVector = std::vector<Symbol>;
    using SymbolSet = entt::dense_set<Symbol, SymbolHasher, std::equal_to<void>, std::allocator<Symbol>>;
    BEDROCK_STATIC_ASSERT_SIZE(SymbolSet, 72, 56);
    using ParseMap = std::map<Symbol, SymbolSet>;

    struct Overload {
        using AllocFunction = brstd::copyable_function<std::unique_ptr<Command>() const>;
        Overload(CommandVersion version, AllocFunction alloc);

        CommandVersion version;
        AllocFunction alloc;
        std::vector<CommandParameterData> params;
        std::int32_t version_offset{-1};
        bool is_chaining{false};
        SymbolVector params_symbol;

        BEDROCK_STATIC_ASSERT_SIZE(AllocFunction, 64, 64);
    };
    BEDROCK_STATIC_ASSERT_SIZE(Overload, 128, 144);

    struct Signature {
        std::string name;
        std::string description;
        std::vector<Overload> overloads;
        std::vector<std::uint32_t> chained_subcommand_indexes;
        CommandPermissionLevel permission_level;
        Symbol command_symbol;
        Symbol command_alias_enum;
        CommandFlag flags;
        int first_rule;
        int first_factorization;
        int first_optional;
        bool runnable;
        std::size_t rule_counter;
    };
    BEDROCK_STATIC_ASSERT_SIZE(Signature, 152, 136);

    struct ParseToken {
        std::unique_ptr<ParseToken> child;  // +0
        std::unique_ptr<ParseToken> next;   // +8
        ParseToken *parent;                 // +16
        char const *text;                   // +24
        std::uint32_t length;               // +32
        Symbol type;                        // +36
    };
    BEDROCK_STATIC_ASSERT_SIZE(CommandRegistry::ParseToken, 40, 40);

    using ProcessFunction = std::function<ParseToken *(ParseToken &, Symbol)>;
    struct ParseRule {
        ParseRule(ParseRule &&) = default;
        ParseRule &operator=(ParseRule &&) = default;
        // Endstone begins
        ParseRule() = default;
        ParseRule(const ParseRule &) = default;
        ParseRule &operator=(const ParseRule &) = default;
        // Endstone ends

        NonTerminal non_terminal;
        ProcessFunction process;
        SymbolVector derivation;
        CommandVersion versions;
    };
    BEDROCK_STATIC_ASSERT_SIZE(ParseRule, 104, 96);

    using RuleIndex = int;
    struct OptionalParameterChain {
        int parameter_count;
        RuleIndex following_rule_index;
        Symbol param_symbol;
    };
    BEDROCK_STATIC_ASSERT_SIZE(OptionalParameterChain, 12, 12);

    struct Factorization {
        Factorization(Factorization &&) = default;
        Factorization &operator=(Factorization &&) = default;
        // Endstone begins
        Factorization() = default;
        Factorization(const Factorization &) = default;
        Factorization &operator=(const Factorization &) = default;
        // Endstone ends

        Terminal command_symbol;
    };
    BEDROCK_STATIC_ASSERT_SIZE(Factorization, 4, 4);

    using ParseFunction = bool (CommandRegistry::*)(void *, const ParseToken &, const CommandOrigin &, int,
                                                    std::string &, std::vector<std::string> &) const;
    struct ParamParseRule {
        ParseFunction parse;
        Symbol symbol;
    };
    BEDROCK_STATIC_ASSERT_SIZE(ParamParseRule, 16, 24);

    template <typename T>
    struct ParseRuleFor {
        static const ParamParseRule instance;
    };
    struct Enum {
        Enum(Enum &&) = default;
        Enum &operator=(Enum &&) = default;
        // Endstone begins
        Enum() = default;
        Enum(const Enum &) = default;
        Enum &operator=(const Enum &) = default;
        // Endstone ends

        std::string name;
        Bedrock::typeid_t<CommandRegistry> type;
        ParseFunction parse;
        std::vector<std::pair<std::uint64_t, std::uint64_t>> values;
    };
    BEDROCK_STATIC_ASSERT_SIZE(Enum, 72, 72);

    struct ChainedSubcommand {
        ChainedSubcommand(ChainedSubcommand &&) = default;
        ChainedSubcommand &operator=(ChainedSubcommand &&) = default;
        // Endstone begins
        ChainedSubcommand() = default;
        ChainedSubcommand(const ChainedSubcommand &) = default;
        ChainedSubcommand &operator=(const ChainedSubcommand &) = default;
        // Endstone ends

        std::string name;
        Bedrock::typeid_t<CommandRegistry> type;
        ParseFunction parse;
        std::vector<std::pair<std::uint64_t, std::uint32_t>> values;
    };
    BEDROCK_STATIC_ASSERT_SIZE(ChainedSubcommand, 72, 72);

    struct SoftEnum {
        std::string name;
        std::vector<std::string> values;
    };
    BEDROCK_STATIC_ASSERT_SIZE(SoftEnum, 56, 48);

    struct ConstrainedValue {
        ConstrainedValue(ConstrainedValue &&) = default;
        ConstrainedValue &operator=(ConstrainedValue &&) = default;
        // Endstone begins
        ConstrainedValue() = default;
        ConstrainedValue(const ConstrainedValue &) = default;
        ConstrainedValue &operator=(const ConstrainedValue &) = default;
        // Endstone ends

        Symbol value;
        Symbol enum_symbol;
        std::vector<unsigned char> constraints;
    };
    BEDROCK_STATIC_ASSERT_SIZE(ConstrainedValue, 32, 32);
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
    friend class endstone::core::CommandPermissions;
    friend class endstone::core::EndstoneCommandMap;
    friend class endstone::core::EndstonePlayer;
    friend class endstone::core::MinecraftCommandPermissions;
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

    template <typename T>
    static const ParamParseRule *getParseRule();

    template <typename T>
    static Bedrock::typeid_t<CommandRegistry> getTypeId();
    // Endstone ends

private:
    using PredictTable = entt::dense_map<std::pair<Symbol, Symbol>, int, SymbolPairHasher, std::equal_to<void>,
                                         std::allocator<std::pair<const std::pair<Symbol, Symbol>, int>>>;
    BEDROCK_STATIC_ASSERT_SIZE(PredictTable, 72, 56);
    struct ParseTable {
        ParseMap first;
        ParseMap follow;
        PredictTable predict;
        SymbolSet nullable_symbols;
        std::chrono::steady_clock::duration build_duration;
    };
    BEDROCK_STATIC_ASSERT_SIZE(ParseTable, 184, 168);

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
    Symbol _addEnumValuesInternal(const std::string &name,
                                  const std::vector<std::pair<std::string, std::uint64_t>> &values,
                                  Bedrock::typeid_t<CommandRegistry> type, ParseFunction parse);
    void registerOverloadInternal(Signature &signature, Overload &overload);

    std::function<void(Packet const &)> network_update_callback_;                                // +0
    ScoreboardScoreAccessor get_score_for_objective_;                                            // +56
    bool is_edu_mode_;                                                                           // +128
    bool is_code_builder_enabled_;                                                               // +129
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
    std::map<std::string, std::uint64_t> enum_value_lookup_;                                     // +360
    std::map<std::string, std::uint32_t> chained_subcommand_lookup_;                             // +376
    std::map<std::string, std::uint64_t> chained_subcommand_value_lookup_;                       // +392
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
    std::string (*auto_complete_player_mention_)(const std::string &, const PlayerListEntries &,
                                                 AutoCompleteInformation &);
};

enum class CommandParameterOption : std::uint8_t {
    None = 0,
    EnumAutocompleteExpansion = 1,
    HasSemanticConstraint = 2,
    EnumAsChainedCommand = 4
};

class CommandParameterData {
    using ParseFunction = CommandRegistry::ParseFunction;
    using ParamParseRule = CommandRegistry::ParamParseRule;
    using CustomStorageGetFn = CommandRegistry::CustomStorageGetFn;
    using CustomStorageIsSetFn = CommandRegistry::CustomStorageIsSetFn;

public:
    CommandParameterData(Bedrock::typeid_t<CommandRegistry> type_index, const ParamParseRule *parse_rule,
                         char const *name, CommandParameterDataType param_type, char const *enum_name_or_postfix,
                         char const *chained_subcommand, int offset, bool is_optional, int set_offset)
        : type_index(type_index), parse_rule(parse_rule), name(name), enum_name_or_postfix(enum_name_or_postfix),
          chained_subcommand(chained_subcommand), param_type(param_type), offset(offset), set_offset(set_offset),
          is_optional(is_optional)
    {
    }
    CommandParameterData(Bedrock::typeid_t<CommandRegistry> type_index, const ParamParseRule *parse_rule,
                         const char *name, CommandParameterDataType param_type, const char *enum_name_or_postfix,
                         int index, bool is_optional, CustomStorageGetFn get_fn, CustomStorageIsSetFn is_set_fn)
        : type_index(type_index), parse_rule(parse_rule), name(name), enum_name_or_postfix(enum_name_or_postfix),
          chained_subcommand(nullptr), param_type(param_type), offset(index), set_offset(index),
          is_optional(is_optional), value_get_fn(get_fn), value_is_set_fn(is_set_fn)
    {
    }

    Bedrock::typeid_t<CommandRegistry> type_index;
    const ParamParseRule *parse_rule;
    ParseFunction parse_override{nullptr};
    std::string name;
    const char *enum_name_or_postfix;
    int enum_or_postfix_symbol{-1};
    const char *chained_subcommand;
    int chained_subcommand_symbol{-1};
    CommandParameterDataType param_type;
    int offset;
    int set_offset;
    bool is_optional;
    CommandParameterOption options{CommandParameterOption::None};
    CustomStorageGetFn value_get_fn{nullptr};
    CustomStorageIsSetFn value_is_set_fn{nullptr};
};
BEDROCK_STATIC_ASSERT_SIZE(CommandParameterData, 120, 120);

inline CommandRegistry::Overload::Overload(CommandVersion version, AllocFunction alloc)
    : version(version), alloc(std::move(alloc))
{
}

template <typename CommandType>
const CommandRegistry::Overload *CommandRegistry::registerOverload(const char *name, CommandVersion version,
                                                                   std::vector<CommandParameterData> params)
{
    auto *signature = const_cast<Signature *>(findCommand(name));
    if (!signature) {
        return nullptr;
    }

    auto &overload = signature->overloads.emplace_back(version, &allocateCommand<CommandType>);
    overload.params = std::move(params);
    registerOverloadInternal(*signature, overload);
    return &overload;
}

// Endstone begins
template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<std::string>();
template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<int>();
template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<float>();
template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<CommandMessage>();
template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<CommandRawText>();
template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<CommandSelector<Actor>>();
template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<CommandSelector<Player>>();
template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<CommandPosition>();
template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<CommandPositionFloat>();
template <>
const CommandRegistry::ParamParseRule *CommandRegistry::getParseRule<Json::Value>();
template <>
Bedrock::typeid_t<CommandRegistry> CommandRegistry::getTypeId<std::string>();
// Endstone ends

template <>
struct std::formatter<CommandRegistry::ParseToken> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const CommandRegistry::ParseToken &token, FormatContext &ctx) const
    {
        auto out = std::format_to(ctx.out(), "[");
        for (const auto *it = &token; it; it = it->next.get()) {
            if (it != &token) {
                out = std::format_to(out, ", ");
            }
            out = std::format_to(out, R"({{"type": "{}")", it->type.value());
            if (it->text != nullptr && it->length > 0) {
                std::string_view text(it->text, it->length);
                if (it->type == CommandRegistry::HardNonTerminal::Id && text.size() >= 2 && text.front() == '"' &&
                    text.back() == '"') {
                    text = text.substr(1, text.size() - 2);
                }
                out = std::format_to(out, R"(, "text": "{}")", text);
            }
            if (it->child != nullptr) {
                out = std::format_to(out, R"(, "children": {})", *it->child);
            }
            out = std::format_to(out, "}}");
        }
        out = std::format_to(ctx.out(), "]");
        return out;
    }
};
