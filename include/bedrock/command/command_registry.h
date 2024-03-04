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

enum SemanticConstraint {
};

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
        uint8_t unknown1 = 0;                      // +40
        uint16_t unknown2 = -1;
        uint64_t unknown3 = 0;
        uint64_t unknown4 = 0;
        uint64_t unknown5 = 0;
    };

    struct Symbol {
        int value = -1;
    };

    struct Signature {
        std::string name;                                  // +0
        std::string description;                           // +32
        std::vector<CommandRegistry::Overload> overloads;  // +64
        char pad1[24];                                     // +88
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

    class ParseTable;
    class Enum;

    using ParseRule = bool (CommandRegistry::*)(void *, const CommandRegistry::ParseToken &, const CommandOrigin &, int,
                                                std::string &, std::vector<std::string> &);

    BEDROCK_API void registerCommand(const std::string &name, char const *description, CommandPermissionLevel level,
                                     CommandFlag flag1, CommandFlag flag2);
    BEDROCK_API void registerAlias(std::string name, std::string alias);

    template <typename CommandType>
    static std::unique_ptr<Command> allocateCommand()
    {
        return std::move(std::make_unique<CommandType>());
    }

    template <typename CommandType, typename... Params>
    const CommandRegistry::Overload *registerOverload(const char *name, CommandVersion version, const Params &...params)
    {
        auto *signature = const_cast<CommandRegistry::Signature *>(findCommand(name));
        if (!signature) {
            return nullptr;
        }

        auto overload = CommandRegistry::Overload(version, CommandRegistry::allocateCommand<CommandType>);
        overload.params = {params...};

        signature->overloads.push_back(overload);
        registerOverloadInternal(*signature, overload);
        return &signature->overloads.back();
    }

    std::function<void(class Packet const &)> network_update_callback;                      // +0
    std::function<int(bool &, std::string const &, class Actor const &)> score_callback;    // +56
    std::vector<void *> unknown1;                                                           // +128
    std::map<uint32_t, CommandRegistry::ParseTable> parse_tables;                           // +152
    std::vector<void *> unknown2;                                                           // +168
    std::vector<std::string> enums;                                                         // +192
    std::vector<CommandRegistry::Enum> enum_data;                                           // +216
    std::vector<std::string> subcommands;                                                   // +240
    std::vector<CommandRegistry::Enum> subcommand_data;                                     // +264
    std::vector<void *> unknown3;                                                           // +288
    std::vector<std::string> postfixes;                                                     // +312
    std::map<std::string, uint32_t> unknown4;                                               // +336
    std::map<std::string, CommandRegistry::Symbol> enum_symbols;                            // +352
    std::map<std::string, uint32_t> unknown5;                                               // +368
    std::map<std::string, uint64_t> unknown6;                                               // +384
    std::vector<void *> unknown7;                                                           // +400
    std::map<std::string, CommandRegistry::Signature> commands;                             // +424
    std::map<Bedrock::typeid_t<CommandRegistry>, int> type_ids;                             // +440
    std::map<std::string, std::string> aliases;                                             // +456
    std::vector<SemanticConstraint> semantic_constraints;                                   // +472
    std::map<SemanticConstraint, unsigned char> constrained_values;                         // +496
    std::vector<void *> constrained_value_data;                                             // +512
    std::map<std::pair<uint64_t, uint32_t>, uint32_t> unknown8;                             // +536
    std::vector<void *> soft_enum_data;                                                     // +552
    std::map<std::string, uint32_t> unknown9;                                               // +576
    std::vector<void *> unknown10;                                                          // +592
#ifdef __linux__                                                                            // __linux__
    char param_symbols[120];                                                                //
#elif _WIN32                                                                                // _WIN32
    char param_symbols[96];
#endif                                                                                      // __linux__
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
    BEDROCK_API void registerOverloadInternal(CommandRegistry::Signature &signature,
                                              CommandRegistry::Overload &overload);
};

enum CommandParameterDataType : int;
enum CommandParameterOption : char;

struct CommandParameterData {

    CommandParameterData(const Bedrock::typeid_t<CommandRegistry> &type_id, CommandRegistry::ParseRule parse_rule,
                         const char *name, CommandParameterDataType type, const char *enum_name,
                         const char *postfix_name, int offset_value, bool optional, int offset_has_value)
        : type_id(type_id), parse_rule(parse_rule), name(name), type(type), enum_name(enum_name),
          postfix_name(postfix_name), offset_value(offset_value), optional(optional), offset_has_value(offset_has_value)
    {
    }

    Bedrock::typeid_t<CommandRegistry> type_id;  // +0
    CommandRegistry::ParseRule parse_rule;       // +8
    std::string name;                            // +16
    const char *enum_name;                       // +48
    CommandRegistry::Symbol enum_symbol;         // +56
    const char *postfix_name;                    // +64
    CommandRegistry::Symbol postfix_symbol;      // +72
    CommandParameterDataType type;               // +76
    int offset_value;                            // +80
    int offset_has_value;                        // +84
    bool optional;                               // +88
    CommandParameterOption option;               // +89
};
