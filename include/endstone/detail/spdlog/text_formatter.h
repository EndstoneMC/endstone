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

#include "endstone/color_format.h"

namespace endstone::detail {

class TextFormatter : public spdlog::custom_flag_formatter {
public:
    explicit TextFormatter(bool should_do_colors) : should_do_colors_(should_do_colors){};
    void format(const spdlog::details::log_msg &msg, const std::tm &, spdlog::memory_buf_t &dest) override;
    [[nodiscard]] std::unique_ptr<custom_flag_formatter> clone() const override;

private:
    const std::unordered_map<unsigned char, spdlog::string_view_t> ansi_codes = {
        // References: https://minecraft.wiki/w/Formatting_codes

        // Color codes
        {ColorFormat::Black.back(), "\x1b[30m"},
        {ColorFormat::DarkBlue.back(), "\x1b[34m"},
        {ColorFormat::DarkGreen.back(), "\x1b[32m"},
        {ColorFormat::DarkAqua.back(), "\x1b[36m"},
        {ColorFormat::DarkRed.back(), "\x1b[31m"},
        {ColorFormat::DarkPurple.back(), "\x1b[35m"},
        {ColorFormat::Gold.back(), "\x1b[33m"},
        {ColorFormat::Gray.back(), "\x1b[37m"},
        {ColorFormat::DarkGray.back(), "\x1b[90m"},
        {ColorFormat::Blue.back(), "\x1b[94m"},
        {ColorFormat::Green.back(), "\x1b[92m"},
        {ColorFormat::Aqua.back(), "\x1b[96m"},
        {ColorFormat::Red.back(), "\x1b[91m"},
        {ColorFormat::LightPurple.back(), "\x1b[95m"},
        {ColorFormat::Yellow.back(), "\x1b[93m"},
        {ColorFormat::White.back(), "\x1b[97m"},
        {ColorFormat::MinecoinGold.back(), "\x1b[38;2;221;214;5m"},
        {ColorFormat::MaterialQuartz.back(), "\x1b[38;2;227;212;209m"},
        {ColorFormat::MaterialIron.back(), "\x1b[38;2;206;202;202m"},
        {ColorFormat::MaterialNetherite.back(), "\x1b[38;2;68;58;59m"},
        {ColorFormat::MaterialRedstone.back(), "\x1b[38;2;151;22;7m"},
        {ColorFormat::MaterialCopper.back(), "\x1b[38;2;180;104;77m"},
        {ColorFormat::MaterialGold.back(), "\x1b[38;2;222;177;45m"},
        {ColorFormat::MaterialEmerald.back(), "\x1b[38;2;17;160;54m"},
        {ColorFormat::MaterialDiamond.back(), "\x1b[38;2;44;186;168m"},
        {ColorFormat::MaterialLapis.back(), "\x1b[38;2;33;73;123m"},
        {ColorFormat::MaterialAmethyst.back(), "\x1b[38;2;154;92;198m"},

        // Formatting codes
        {ColorFormat::Obfuscated.back(), "\x1b[8m"},
        {ColorFormat::Bold.back(), "\x1b[1m"},
        {ColorFormat::Italic.back(), "\x1b[3m"},
        {ColorFormat::Reset.back(), "\x1b[0m"},
    };

    bool should_do_colors_;
};

}  // namespace endstone::detail
