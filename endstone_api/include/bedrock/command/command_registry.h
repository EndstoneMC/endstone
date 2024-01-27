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
#include <string>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/command/command.h"
#include "bedrock/command/command_parameter_data.h"
#include "bedrock/command/command_version.h"

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
        uint8_t unk1 = 0;
        uint16_t unk2 = -1;
        uint64_t unk3 = 0;
        uint64_t unk4 = 0;
        uint64_t unk5 = 0;
    };

    struct Symbol {
        uint32_t value;
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

    template <typename Cmd>
    static std::unique_ptr<Command> allocateCommand()
    {
        printf("allocateCommand\n");
        return std::move(std::make_unique<Cmd>());
    }

    template <typename Cmd, typename... Params>
    const CommandRegistry::Overload *registerOverload(const char *name, CommandVersion version, const Params &...params)
    {
        // TODO: can we get rid of const_cast?
        auto *signature = const_cast<CommandRegistry::Signature *>(findCommand(name));

        if (!signature) {
            return nullptr;
        }

        // TODO: add params support, currently ignored
        auto overload = CommandRegistry::Overload(version, CommandRegistry::allocateCommand<Cmd>);

        signature->overloads.push_back(overload);
        registerOverloadInternal(*signature, overload);
        return &signature->overloads.back();
    }

    BEDROCK_API void registerCommand(const std::string &name, char const *description, CommandPermissionLevel level,
                                     CommandFlag flag1, CommandFlag flag2);

private:
    [[nodiscard]] BEDROCK_API const CommandRegistry::Signature *findCommand(const std::string &name) const;
    BEDROCK_API void registerOverloadInternal(CommandRegistry::Signature &signature,
                                              CommandRegistry::Overload &overload);
};
