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

#include <spdlog/pattern_formatter.h>
#include <spdlog/spdlog.h>

class BedrockTextFormatter : public spdlog::custom_flag_formatter {
public:
    explicit BedrockTextFormatter(bool should_do_colors) : should_do_colors_(should_do_colors){};
    void format(const spdlog::details::log_msg &msg, const std::tm &, spdlog::memory_buf_t &dest) override;
    [[nodiscard]] std::unique_ptr<custom_flag_formatter> clone() const override;

private:
    // TODO: use enum instead of hard-coded char: https://github.com/EndstoneMC/endstone/blob/legacy/src/lib/hook/bedrock/bedrock_log.cpp

    const std::unordered_map<unsigned char, spdlog::string_view_t> color_translation = {
        // References: https://minecraft.wiki/w/Formatting_codes

        // Color codes
        {'0', "\x1b[30m"},
        {'1', "\x1b[34m"},
        {'2', "\x1b[32m"},
        {'3', "\x1b[36m"},
        {'4', "\x1b[31m"},
        {'5', "\x1b[35m"},
        {'6', "\x1b[33m"},
        {'7', "\x1b[37m"},
        {'8', "\x1b[90m"},
        {'9', "\x1b[94m"},
        {'a', "\x1b[92m"},
        {'b', "\x1b[96m"},
        {'c', "\x1b[91m"},
        {'d', "\x1b[95m"},
        {'e', "\x1b[93m"},
        {'f', "\x1b[97m"},
        {'g', "\x1b[38;2;221;214;5m"},
        {'h', "\x1b[38;2;227;212;209m"},
        {'i', "\x1b[38;2;206;202;202m"},
        {'j', "\x1b[38;2;68;58;59m"},
        {'m', "\x1b[38;2;151;22;7m"},
        {'n', "\x1b[38;2;180;104;77m"},
        {'p', "\x1b[38;2;222;177;45m"},
        {'q', "\x1b[38;2;222;177;45m"},
        {'s', "\x1b[38;2;44;186;168m"},
        {'t', "\x1b[38;2;33;73;123m"},
        {'u', "\x1b[38;2;154;92;198m"},

        // Formatting codes
        {'k', "\x1b[8m"},
        {'l', "\x1b[1m"},
        {'o', "\x1b[3m"},
        {'r', "\x1b[0m"},
    };

    bool should_do_colors_;
};
