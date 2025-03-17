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

#include <string_view>
#include <vector>

#include <nonstd/expected.hpp>

#include "endstone/core/command/command_lexer.h"

namespace endstone::core {
class CommandUsageParser {
public:
    struct Parameter {
        std::string name;
        std::string type;
        bool optional = false;
        bool is_enum = false;
        std::vector<std::string> values;
    };

    struct Result {
        std::string command_name;
        std::vector<Parameter> parameters;
    };

    explicit CommandUsageParser(std::string_view input) : lexer_(input) {}

    nonstd::expected<Result, std::string> parse() noexcept;

private:
    nonstd::expected<std::string_view, std::string> parseToken(CommandLexer::TokenType type, std::string what);
    nonstd::expected<std::string_view, std::string> parseIdentifier(std::string what);
    nonstd::expected<Parameter, std::string> parseParameter();
    nonstd::expected<Parameter, std::string> parseEnum();

    CommandLexer lexer_;
};

}  // namespace endstone::core
