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

#include "endstone/util/color_format.h"

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
        {ColorFormat::BLACK.back(), "\x1b[30m"},
        {ColorFormat::DARK_BLUE.back(), "\x1b[34m"},
        {ColorFormat::DARK_GREEN.back(), "\x1b[32m"},
        {ColorFormat::DARK_AQUA.back(), "\x1b[36m"},
        {ColorFormat::DARK_RED.back(), "\x1b[31m"},
        {ColorFormat::DARK_PURPLE.back(), "\x1b[35m"},
        {ColorFormat::GOLD.back(), "\x1b[33m"},
        {ColorFormat::GRAY.back(), "\x1b[37m"},
        {ColorFormat::DARK_GRAY.back(), "\x1b[90m"},
        {ColorFormat::BLUE.back(), "\x1b[94m"},
        {ColorFormat::GREEN.back(), "\x1b[92m"},
        {ColorFormat::AQUA.back(), "\x1b[96m"},
        {ColorFormat::RED.back(), "\x1b[91m"},
        {ColorFormat::LIGHT_PURPLE.back(), "\x1b[95m"},
        {ColorFormat::YELLOW.back(), "\x1b[93m"},
        {ColorFormat::WHITE.back(), "\x1b[97m"},
        {ColorFormat::MINECOIN_GOLD.back(), "\x1b[38;2;221;214;5m"},
        {ColorFormat::MATERIAL_QUARTZ.back(), "\x1b[38;2;227;212;209m"},
        {ColorFormat::MATERIAL_IRON.back(), "\x1b[38;2;206;202;202m"},
        {ColorFormat::MATERIAL_NETHERITE.back(), "\x1b[38;2;68;58;59m"},
        {ColorFormat::MATERIAL_REDSTONE.back(), "\x1b[38;2;151;22;7m"},
        {ColorFormat::MATERIAL_COPPER.back(), "\x1b[38;2;180;104;77m"},
        {ColorFormat::MATERIAL_GOLD.back(), "\x1b[38;2;222;177;45m"},
        {ColorFormat::MATERIAL_EMERALD.back(), "\x1b[38;2;17;160;54m"},
        {ColorFormat::MATERIAL_DIAMOND.back(), "\x1b[38;2;44;186;168m"},
        {ColorFormat::MATERIAL_LAPIS.back(), "\x1b[38;2;33;73;123m"},
        {ColorFormat::MATERIAL_AMETHYST.back(), "\x1b[38;2;154;92;198m"},

        // Formatting codes
        {ColorFormat::OBFUSCATED.back(), "\x1b[8m"},
        {ColorFormat::BOLD.back(), "\x1b[1m"},
        {ColorFormat::ITALIC.back(), "\x1b[3m"},
        {ColorFormat::RESET.back(), "\x1b[0m"},
    };

    bool should_do_colors_;
};

}  // namespace endstone::detail
