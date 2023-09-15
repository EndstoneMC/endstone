// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "bedrock/internal.h"
#include "endstone/command/command.h"

struct CommandFlag {
    uint16_t value;
};

enum class CommandParameterDataType : int {
    Value,
    Enum,
};

class CommandParameterData {
    const char *type_id_;            // +0
    void *parse_fn_;                 // +8
    std::string name_;               // +16
    const char *enum_name_;          // +48
    int unknown_1_;                  // +56
    const char *unknown_2_;          // +64
    int unknown_3_;                  // +72
    CommandParameterDataType type_;  // +76
    int unknown_4_;                  // +80
    int unknown_5_;                  // +84
    bool is_value_;                  // +88
    char options_;                   // +89
};
static_assert(sizeof(CommandParameterData) == 96);

class CommandVersion {
    uint64_t version_;
};

class CommandRegistry {
public:
    struct Overload {
        CommandVersion version;                                     // +0
        [[maybe_unused]] std::unique_ptr<Command> (*factory_fn)();  // +8
        std::vector<CommandParameterData> parameters;               // +16
        char unknown[32];                                           // +44
    };
    static_assert(sizeof(Overload) == 72);

    struct Signature {
        std::string label;                                 // +0
        std::string description;                           // +32
        std::vector<CommandRegistry::Overload> overloads;  // +64
        std::string str3;                                  // +88
        char permission_level;                             // +120
        char pad[28];                                      // +124
    };
    static_assert(sizeof(Signature) == 152);

    BEDROCK_API void registerCommand(const std::string &name, char const *description,
                                     enum class CommandPermissionLevel level, CommandFlag flag1, CommandFlag flag2);
    BEDROCK_API void registerAlias(std::string name, std::string alias);

    inline static std::unordered_map<std::string, std::shared_ptr<Command>> bedrock_commands_{};

private:
    BEDROCK_API void registerOverloadInternal(CommandRegistry::Signature &signature,
                                              CommandRegistry::Overload &overload);
    [[nodiscard]] BEDROCK_API std::string describe(CommandParameterData const &parameter) const;
};
