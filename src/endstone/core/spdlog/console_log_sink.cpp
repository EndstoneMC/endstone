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

#include "endstone/core/spdlog/console_log_sink.h"

#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/sinks/base_sink-inl.h>

#include "endstone/core/console/console.h"
#include "endstone/core/spdlog/level_formatter.h"
#include "endstone/core/spdlog/text_formatter.h"

namespace endstone::core {
/// Formatting codes
constexpr auto reset = "\033[m";
constexpr auto bold = "\033[1m";
constexpr auto dark = "\033[2m";
constexpr auto underline = "\033[4m";
constexpr auto blink = "\033[5m";
constexpr auto reverse = "\033[7m";
constexpr auto concealed = "\033[8m";
constexpr auto clear_line = "\033[K";

/// Foreground colors
constexpr auto black = "\033[30m";
constexpr auto red = "\033[31m";
constexpr auto green = "\033[32m";
constexpr auto yellow = "\033[33m";
constexpr auto blue = "\033[34m";
constexpr auto magenta = "\033[35m";
constexpr auto cyan = "\033[36m";
constexpr auto white = "\033[37m";

/// Background colors
constexpr auto on_black = "\033[40m";
constexpr auto on_red = "\033[41m";
constexpr auto on_green = "\033[42m";
constexpr auto on_yellow = "\033[43m";
constexpr auto on_blue = "\033[44m";
constexpr auto on_magenta = "\033[45m";
constexpr auto on_cyan = "\033[46m";
constexpr auto on_white = "\033[47m";

/// Bold colors
constexpr auto yellow_bold = "\033[33m\033[1m";
constexpr auto red_bold = "\033[31m\033[1m";
constexpr auto bold_on_red = "\033[1m\033[41m";

ConsoleLogSink::ConsoleLogSink(FILE *target_file, spdlog::color_mode mode) : target_file_(target_file)
{
    setColorMode(mode);
    for (auto &formatter : formatters_) {
        formatter.add_flag<LevelFormatter>('L');
        formatter.add_flag<TextFormatter>('v', should_do_colors_);
    }
    formatters_[0].set_pattern("%^[%H:%M:%S %L]: [%n] %v%$");
    formatters_[1].set_pattern("%^[%H:%M:%S %L]: %v%$");
    colors_.at(spdlog::level::trace) = white;
    colors_.at(spdlog::level::debug) = cyan;
    colors_.at(spdlog::level::info) = reset;
    colors_.at(spdlog::level::warn) = yellow_bold;
    colors_.at(spdlog::level::err) = red_bold;
    colors_.at(spdlog::level::critical) = bold_on_red;
    colors_.at(spdlog::level::off) = reset;
}

void ConsoleLogSink::setColorMode(spdlog::color_mode mode)
{
    switch (mode) {
    case spdlog::color_mode::always:
        should_do_colors_ = true;
        return;
    case spdlog::color_mode::automatic:
        should_do_colors_ = spdlog::details::os::in_terminal(target_file_) && spdlog::details::os::is_color_terminal();
        return;
    case spdlog::color_mode::never:
    default:
        should_do_colors_ = false;
    }
}

void ConsoleLogSink::sink_it_(const spdlog::details::log_msg &msg)
{
    static auto &console = EndstoneConsole::getInstance();
    msg.color_range_start = 0;
    msg.color_range_end = 0;
    spdlog::memory_buf_t formatted;
    auto& formatter = msg.logger_name.size() > 0 ? formatters_[0] : formatters_[1];
    formatter.format(msg, formatted);
    if (should_do_colors_ && msg.color_range_end > msg.color_range_start) {
        std::string out;
        out.reserve(formatted.size() + 32);
        // before color range
        out.append(formatted.data(), formatted.data() + msg.color_range_start);
        // in color range
        out.append(colors_.at(static_cast<std::size_t>(msg.level)));
        out.append(formatted.data() + msg.color_range_start, formatted.data() + msg.color_range_end);
        out.append(reset);
        // after color range
        out.append(formatted.data() + msg.color_range_end, formatted.data() + formatted.size());
        console.printAbove(out);
    }
    else  // no color
    {
        console.printAbove({formatted.data(), formatted.size()});
    }
}

void ConsoleLogSink::flush_() {}

}  // namespace endstone::core
