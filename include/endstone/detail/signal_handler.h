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

#include <iostream>

#include <cpptrace/cpptrace.hpp>
#include <fmt/format.h>

#include "endstone/detail/os.h"
#include "endstone/endstone.h"

namespace endstone::detail {

inline void print_frame(std::ostream &stream, bool color, unsigned frame_number_width, std::size_t counter,
                        const cpptrace::stacktrace_frame &frame)
{
    const auto *reset = color ? "\033[0m" : "";
    const auto *green = color ? "\033[32m" : "";
    const auto *yellow = color ? "\033[33m" : "";
    const auto *blue = color ? "\033[34m" : "";
    std::string line = fmt::format("#{:<{}} ", counter, frame_number_width);
    if (frame.is_inline) {
        line += fmt::format("{:<{}}", "(inlined)", 2 * sizeof(cpptrace::frame_ptr) + 2);
    }
    else {
        line += fmt::format("{}0x{:<{}x}{}", blue, frame.raw_address, 2 * sizeof(cpptrace::frame_ptr), reset);
        line += fmt::format(" {}(0x{:09x}){}", green, frame.object_address, reset);
    }
    if (!frame.symbol.empty()) {
        line += fmt::format(" in {}{}{}", yellow, frame.symbol, reset);
    }
    if (!frame.filename.empty()) {
        line += fmt::format(" at {}{}{}", green, frame.filename, reset);
        if (frame.line.has_value()) {
            line += fmt::format(":{}{}{}", blue, frame.line.value(), reset);
            if (frame.column.has_value()) {
                line += fmt::format(":{}{}{}", blue, frame.column.value(), reset);
            }
        }
    }
    stream << line;
}

inline void print_crash_dump(const std::string &message, std::size_t skip = 0)
{
    printf("=== ENDSTONE CRASHED! - PLEASE REPORT THIS AS AN ISSUE ON GITHUB ===\n");
    printf("Operation system: %s\n", os::get_name().c_str());
    printf("Endstone version: %s\n", ENDSTONE_VERSION);
    printf("Api version     : %s\n", ENDSTONE_API_VERSION);
    printf("Description     : %s\n", message.c_str());

    auto stacktrace = cpptrace::generate_trace(skip + 1);
    auto &stream = std::cerr;
    auto color = cpptrace::isatty(cpptrace::stderr_fileno);
    stream << "Stack trace (most recent call first):" << '\n';

    auto &frames = stacktrace.frames;
    std::size_t counter = 0;
    if (frames.empty()) {
        stream << "<empty trace>" << '\n';
        return;
    }
    const auto frame_number_width = std::to_string(frames.size()).length();
    for (const auto &frame : frames) {
        print_frame(stream, color, frame_number_width, counter, frame);
        stream << '\n';
        if (frame.line.has_value() && !frame.filename.empty()) {
            stream << cpptrace::get_snippet(frame.filename, frame.line.value(), 2, color);
        }
        counter++;
    }
}

void register_signal_handler();

}  // namespace endstone::detail
