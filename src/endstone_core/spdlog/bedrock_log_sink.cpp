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

#include "endstone_core/spdlog/bedrock_log_sink.h"

#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/base_sink-inl.h>

#include "endstone_core/spdlog/bedrock_level_formatter.h"
#include "endstone_core/spdlog/bedrock_text_formatter.h"

BedrockLogSink::BedrockLogSink(FILE *target_file, spdlog::color_mode mode)
    : target_file_(target_file), spdlog::sinks::base_sink<spdlog::details::console_mutex::mutex_t>(
                                     spdlog::details::make_unique<spdlog::pattern_formatter>())
{
    set_color_mode(mode);
    auto *formatter = dynamic_cast<spdlog::pattern_formatter *>(formatter_.get());
    formatter->add_flag<BedrockLevelFormatter>('L');
    formatter->add_flag<BedrockTextFormatter>('v', should_do_colors_);
    formatter->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e %L] [%n] %v%$");
    colors_.at(spdlog::level::trace) = to_string(white);
    colors_.at(spdlog::level::debug) = to_string(cyan);
    colors_.at(spdlog::level::info) = to_string(reset);
    colors_.at(spdlog::level::warn) = to_string(yellow_bold);
    colors_.at(spdlog::level::err) = to_string(red_bold);
    colors_.at(spdlog::level::critical) = to_string(bold_on_red);
    colors_.at(spdlog::level::off) = to_string(reset);
}

void BedrockLogSink::set_color_mode(spdlog::color_mode mode)
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

std::string BedrockLogSink::to_string(const spdlog::string_view_t &sv)
{
    return {sv.data(), sv.size()};
}

void BedrockLogSink::sink_it_(const spdlog::details::log_msg &msg)
{
    msg.color_range_start = 0;
    msg.color_range_end = 0;
    spdlog::memory_buf_t formatted;
    formatter_->format(msg, formatted);
    if (should_do_colors_ && msg.color_range_end > msg.color_range_start) {
        // before color range
        print_range_(formatted, 0, msg.color_range_start);
        // in color range
        print_ccode_(colors_.at(static_cast<size_t>(msg.level)));
        print_range_(formatted, msg.color_range_start, msg.color_range_end);
        print_ccode_(reset);
        // after color range
        print_range_(formatted, msg.color_range_end, formatted.size());
    }
    else  // no color
    {
        print_range_(formatted, 0, formatted.size());
    }
    fflush(target_file_);
}

void BedrockLogSink::flush_()
{
    fflush(target_file_);
}

void BedrockLogSink::print_ccode_(const spdlog::string_view_t &color_code)
{
    fwrite(color_code.data(), sizeof(char), color_code.size(), target_file_);
}

void BedrockLogSink::print_range_(const spdlog::memory_buf_t &formatted, size_t start, size_t end)
{
    fwrite(formatted.data() + start, sizeof(char), end - start, target_file_);
}
