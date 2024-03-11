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
#include "bedrock/command/command_origin.h"
#include "bedrock/command/command_version.h"
#include "bedrock/type_id.h"

struct CommandFlag {
    uint16_t value;
};

enum class CommandPermissionLevel : std::uint8_t {
    Any = 0,
    GameDirectors = 1,
    Admin = 2,
    Host = 3,
    Owner = 4,
    Internal = 5
};

enum SemanticConstraint : uint8_t;

class Command;
class CommandParameterData;

class CommandRegistry {
public:
    struct Overload {
        Overload(const CommandVersion &version, std::unique_ptr<Command> (*factory)())
            : version(version), factory(factory)
        {
        }

        CommandVersion version;                    // +0
        std::unique_ptr<Command> (*factory)();     // +8
        std::vector<CommandParameterData> params;  // +16
        int32_t unknown1{-1};                      // +40
        int8_t unknown2{0};                        // +44
        uint64_t unknown3{0};                      // +48
        uint64_t unknown4{0};                      // +56
        uint64_t unknown5{0};                      // +64
    };

    class Symbol {
    public:
        int value = -1;
        static const Symbol INT;
        static const Symbol FLOAT;
        static const Symbol RELATIVE_FLOAT;
        static const Symbol WILDCARD_INT;
        static const Symbol OPERATOR;
        static const Symbol COMPARE_OPERATOR;
        static const Symbol SELECTOR;
        static const Symbol WILDCARD_ACTOR_SELECTOR;
        static const Symbol FILE_PATH;
        static const Symbol INTEGER_RANGE;
        static const Symbol EQUIPMENT_SLOT;
        static const Symbol STRING;
        static const Symbol POSITION;
        static const Symbol POSITION_FLOAT;
        static const Symbol MESSAGE;
        static const Symbol RAW_TEXT;
        static const Symbol JSON;
        static const Symbol BLOCK_STATE;
        static const Symbol COMMAND;
    };

    struct Signature {
        std::string name;                                  // +0
        std::string description;                           // +32
        std::vector<CommandRegistry::Overload> overloads;  // +64
        char unknown1[24];                                 // +88
        CommandPermissionLevel permission_level;           // +112
        CommandRegistry::Symbol symbol;                    // +116
        int32_t unknown2;                                  // +120
        CommandFlag command_flag;                          // +124
        int32_t unknown3;                                  // +128
        int32_t unknown4;                                  // +132
        int32_t unknown5;                                  // +136
        char unknown6;                                     // +140
        int64_t unknown7;                                  // +144
    };

    struct ParseToken {
        std::unique_ptr<ParseToken> child;  // +0
        std::unique_ptr<ParseToken> next;   // +8
        ParseToken *parent;                 // +16
        char const *data;                   // +24
        uint32_t size;                      // +32
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
        std::string name;                                   // +0
        Bedrock::typeid_t<CommandRegistry> type_id;         // +32
        ParseRule parse_rule;                               // +40
        std::vector<std::pair<uint64_t, uint64_t>> values;  // +48
    };
    class SoftEnum;

    BEDROCK_API void registerCommand(const std::string &name, char const *description, CommandPermissionLevel level,
                                     CommandFlag flag1, CommandFlag flag2);
    BEDROCK_API void registerAlias(std::string name, std::string alias);
    BEDROCK_API int addEnumValues(const std::string &name, const std::vector<std::string> &values);

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
    std::map<uint32_t, CommandRegistry::ParseTable> parse_tables;                           // +152
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
    std::vector<void *> unknown7;                                                           // +400
    std::map<std::string, CommandRegistry::Signature> commands;                             // +424
    std::map<Bedrock::typeid_t<CommandRegistry>, int> type_ids;                             // +440
    std::map<std::string, std::string> aliases;                                             // +456
    std::vector<SemanticConstraint> semantic_constraints;                                   // +472
    std::map<SemanticConstraint, unsigned char> constrained_values;                         // +496
    std::vector<void *> constrained_value_data;                                             // +512
    std::map<std::pair<uint64_t, uint32_t>, uint32_t> unknown8;                             // +536
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

enum CommandParameterOption : char;

struct CommandParameterData {
    CommandParameterData(Bedrock::typeid_t<CommandRegistry> type_id, CommandRegistry::ParseRule parse_rule,
                         char const *name, CommandParameterDataType type, char const *type_name,
                         char const *fallback_typename, int offset_value, bool optional, int offset_has_value)
        : type_id(type_id), parse_rule(parse_rule), name(name), enum_name(type_name),
          fallback_typename(fallback_typename), type(type), offset_value(offset_value),
          offset_has_value(offset_has_value), optional(optional)
    {
    }

    Bedrock::typeid_t<CommandRegistry> type_id;   // +0
    CommandRegistry::ParseRule parse_rule;        // +8
    std::string name;                             // +16
    const char *enum_name;                        // +48
    CommandRegistry::Symbol enum_symbol{-1};      // +56
    const char *fallback_typename;                // +64
    CommandRegistry::Symbol fallback_symbol{-1};  // +72
    CommandParameterDataType type;                // +76
    int offset_value;                             // +80
    int offset_has_value;                         // +84
    bool optional;                                // +88
    CommandParameterOption option{0};             // +89
};
