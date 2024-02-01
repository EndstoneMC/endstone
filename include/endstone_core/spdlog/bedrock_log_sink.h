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

#include <array>
#include <string_view>

#include <spdlog/details/console_globals.h>
#include <spdlog/details/os.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>

class BedrockLogSink : public spdlog::sinks::base_sink<spdlog::details::console_mutex::mutex_t> {
public:
    explicit BedrockLogSink(FILE *target_file, spdlog::color_mode mode = spdlog::color_mode::automatic);
    void set_color_mode(spdlog::color_mode mode);

protected:
    void sink_it_(const spdlog::details::log_msg &msg) override;
    void flush_() override;

public:
    // Formatting codes
    const spdlog::string_view_t reset = "\033[m";
    const spdlog::string_view_t bold = "\033[1m";
    const spdlog::string_view_t dark = "\033[2m";
    const spdlog::string_view_t underline = "\033[4m";
    const spdlog::string_view_t blink = "\033[5m";
    const spdlog::string_view_t reverse = "\033[7m";
    const spdlog::string_view_t concealed = "\033[8m";
    const spdlog::string_view_t clear_line = "\033[K";

    // Foreground colors
    const spdlog::string_view_t black = "\033[30m";
    const spdlog::string_view_t red = "\033[31m";
    const spdlog::string_view_t green = "\033[32m";
    const spdlog::string_view_t yellow = "\033[33m";
    const spdlog::string_view_t blue = "\033[34m";
    const spdlog::string_view_t magenta = "\033[35m";
    const spdlog::string_view_t cyan = "\033[36m";
    const spdlog::string_view_t white = "\033[37m";

    /// Background colors
    const spdlog::string_view_t on_black = "\033[40m";
    const spdlog::string_view_t on_red = "\033[41m";
    const spdlog::string_view_t on_green = "\033[42m";
    const spdlog::string_view_t on_yellow = "\033[43m";
    const spdlog::string_view_t on_blue = "\033[44m";
    const spdlog::string_view_t on_magenta = "\033[45m";
    const spdlog::string_view_t on_cyan = "\033[46m";
    const spdlog::string_view_t on_white = "\033[47m";

    /// Bold colors
    const spdlog::string_view_t yellow_bold = "\033[33m\033[1m";
    const spdlog::string_view_t red_bold = "\033[31m\033[1m";
    const spdlog::string_view_t bold_on_red = "\033[1m\033[41m";

private:
    void print_ccode_(const spdlog::string_view_t &color_code);
    void print_range_(const spdlog::memory_buf_t &formatted, size_t start, size_t end);
    static std::string to_string(const spdlog::string_view_t &sv);

    FILE *target_file_;
    bool should_do_colors_;
    std::array<std::string, spdlog::level::n_levels> colors_;
};
