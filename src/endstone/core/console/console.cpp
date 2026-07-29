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

#include <cstdlib>
#include <iostream>

namespace endstone::core {
namespace {
bool is_true(const char *v)
{
    if (!v) {
        return false;
    }
    std::string s(v);
    for (auto &c : s) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return (s == "1" || s == "true" || s == "yes" || s == "on");
}

bool use_interactive_console()
{
#if defined(_WIN32)
    char *value = nullptr;
    size_t value_length = 0;
    if (_dupenv_s(&value, &value_length, "ENDSTONE_USE_INTERACTIVE_CONSOLE") != 0) {
        return false;
    }
    const bool result = is_true(value);
    std::free(value);
    return result;
#else
    return is_true(std::getenv("ENDSTONE_USE_INTERACTIVE_CONSOLE"));
#endif
}
}  // namespace

EndstoneConsole::EndstoneConsole()
{
    if (use_interactive_console()) {
        rx_ = replxx::Replxx{};
        // commit on LF too, so LF-terminated piped consoles (e.g. Pterodactyl) submit commands
        rx_->bind_key_internal(replxx::Replxx::KEY::control('J'), "commit_line");
        rx_->history_load(".console_history");
    }
}
EndstoneConsole::~EndstoneConsole()
{
    if (rx_.has_value()) {
        rx_->history_save(".console_history");
    }
}

std::optional<std::string> EndstoneConsole::readLine(const std::string &prompt)
{
    std::string line;
    if (rx_.has_value()) {
        const char *cstr = rx_->input(prompt);
        if (!cstr) {
            return std::nullopt;
        }
        line = cstr;
        if (!line.empty()) {
            rx_->history_add(line);
        }
    }
    else {
        std::getline(std::cin, line);
        if (!std::cin.good()) {
            return std::nullopt;
        }
    }
    return line;
}

void EndstoneConsole::printAbove(std::string_view message)
{
    const bool has_nl = !message.empty() && message.back() == '\n';
    if (rx_.has_value()) {
        if (has_nl) {
            rx_->print("%.*s", message.size(), message.data());
        }
        else {
            rx_->print("%.*s\n", message.size(), message.data());
        }
    }
    else {
        if (has_nl) {
            std::cout.write(message.data(), static_cast<std::streamsize>(message.size()));
        }
        else {
            std::cout.write(message.data(), static_cast<std::streamsize>(message.size()));
            std::cout.put('\n');
        }
        std::cout.flush();
    }
}

EndstoneConsole &EndstoneConsole::getInstance()
{
    static EndstoneConsole console;
    return console;
}
}  // namespace endstone::core
