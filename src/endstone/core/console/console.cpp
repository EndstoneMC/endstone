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

#include "endstone/core/console/console.h"

namespace endstone::core {
std::string EndstoneConsole::readLine(const std::string &prompt)
{
    const char *cstr = rx_.input(prompt);
    if (!cstr) {
        return {};
    }
    std::string line(cstr);
    if (!line.empty()) {
        rx_.history_add(line);
    }
    return line;
}

void EndstoneConsole::printAbove(std::string_view message)
{
    if (!message.empty() && message.back() == '\n') {
        rx_.print("%.*s", static_cast<int>(message.size()), message.data());
    }
    else {
        rx_.print("%.*s\n", static_cast<int>(message.size()), message.data());
    }
}

EndstoneConsole &EndstoneConsole::getInstance()
{
    static EndstoneConsole console;
    return console;
}
}  // namespace endstone::core
