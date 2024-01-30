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

class Command;
class CommandParameterData;
class CommandRegistry {
public:
    struct Overload {
        Overload(const CommandVersion &version, std::unique_ptr<Command> (*factory)())
            : version(version), factory(factory)
        {
        }

        CommandVersion version;
        std::unique_ptr<Command> (*factory)();
        std::vector<CommandParameterData> params;
        uint8_t unknown1 = 0;
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

    class ParseToken;

    using ParseRule = bool (CommandRegistry::*)(void *, const CommandRegistry::ParseToken &, const CommandOrigin &, int,
                                                std::string &, std::vector<std::string> &);

    template <typename CommandType>
    static std::unique_ptr<Command> allocateCommand()
    {
        return std::move(std::make_unique<CommandType>());
    }

    template <typename CommandType, typename... Params>
    const CommandRegistry::Overload *registerOverload(const char *name, CommandVersion version, const Params &...params)
    {
        // TODO: can we get rid of const_cast?
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

    BEDROCK_API void registerCommand(const std::string &name, char const *description, CommandPermissionLevel level,
                                     CommandFlag flag1, CommandFlag flag2);

private:
    [[nodiscard]] BEDROCK_API const CommandRegistry::Signature *findCommand(const std::string &name) const;
    [[nodiscard]] BEDROCK_API std::string describe(CommandParameterData const &) const;
    BEDROCK_API void registerOverloadInternal(CommandRegistry::Signature &signature,
                                              CommandRegistry::Overload &overload);
};

enum CommandParameterDataType : int;
enum CommandParameterOption : char;

class CommandParameterData {

public:
    template <typename Command, typename Type>
    static CommandParameterData create(const char *name, Type Command::*data)
    {
        union {
            Type Command::*p;
            uint32_t v;
        } tmp;
        tmp.p = data;
        return create(Bedrock::type_id<CommandRegistry, Type>(), name, tmp.v, false, -1);
    }

    template <typename Command, typename Type>
    static CommandParameterData create(const char *name, std::optional<Type> Command::*data)
    {
        union {
            std::optional<Type> Command::*p;
            uint32_t v;
        } tmp;
        tmp.p = data;
        // tmp.v + sizeof(Type) is based on the assumption of std::optional implementation
        // https://github.com/llvm/llvm-project/blob/659ce8f66597ba19845e407d06156ff33c8c7fb1/libc/src/__support/CPP/optional.h#L30
        // https://github.com/microsoft/STL/blob/32e65ac3e54a7150202cc9b3cc4de7330d58d24e/stl/inc/optional#L97
        return create(Bedrock::type_id<CommandRegistry, Type>(), name, tmp.v, true, tmp.v + sizeof(Type));
    }

private:
    static CommandParameterData create(const Bedrock::typeid_t<CommandRegistry> &type_id, const char *name,
                                       int offset_value, bool optional, int offset_has_value);

public:
    CommandParameterData(const Bedrock::typeid_t<CommandRegistry> &type_id, CommandRegistry::ParseRule parse_rule,
                         const char *name, CommandParameterDataType type, const char *enum_name,
                         const char *postfix_name, int offset_value, bool optional, int offset_has_value)
        : type_id_(type_id), parse_rule_(parse_rule), name_(name), type_(type), enum_name_(enum_name),
          postfix_name_(postfix_name), offset_value_(offset_value), optional_(optional),
          offset_has_value_(offset_has_value)
    {
    }

    [[nodiscard]] Bedrock::typeid_t<CommandRegistry> getTypeId() const
    {
        return type_id_;
    }

    [[nodiscard]] CommandRegistry::ParseRule getParseRule() const
    {
        return parse_rule_;
    }
    [[nodiscard]] const std::string &getName() const
    {
        return name_;
    }
    [[nodiscard]] const char *getEnumName() const
    {
        return enum_name_;
    }
    [[nodiscard]] const CommandRegistry::Symbol &getEnumSymbol() const
    {
        return enum_symbol_;
    }
    [[nodiscard]] const char *getPostfixName() const
    {
        return postfix_name_;
    }
    [[nodiscard]] const CommandRegistry::Symbol &getPostfixSymbol() const
    {
        return postfix_symbol_;
    }
    [[nodiscard]] CommandParameterDataType getType() const
    {
        return type_;
    }
    [[nodiscard]] bool isOptional() const
    {
        return optional_;
    }
    [[nodiscard]] CommandParameterOption getOption() const
    {
        return option_;
    }

private:
    Bedrock::typeid_t<CommandRegistry> type_id_;  // +0
    CommandRegistry::ParseRule parse_rule_;       // +8
    std::string name_;                            // +16
    const char *enum_name_;                       // +48
    CommandRegistry::Symbol enum_symbol_;         // +56
    const char *postfix_name_;                    // +64
    CommandRegistry::Symbol postfix_symbol_;      // +72
    CommandParameterDataType type_;               // +76
    int offset_value_;                            // +80
    int offset_has_value_;                        // +84
    bool optional_;                               // +88
    CommandParameterOption option_;               // +89
};
