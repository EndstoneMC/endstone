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

#include "endstone/core/spdlog/file_log_sink.h"

#include <fstream>
#include <zstr.hpp>

#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include <spdlog/details/os.h>
#include <spdlog/pattern_formatter.h>

#include "endstone/core/spdlog/level_formatter.h"
#include "endstone/core/spdlog/text_formatter.h"

namespace endstone::core {

FileLogSink::FileLogSink(spdlog::filename_t base_filename, spdlog::filename_t file_pattern, uint16_t max_files,
                         const spdlog::file_event_handlers &event_handlers)
    : base_filename_(std::move(base_filename)), file_pattern_(std::move(file_pattern)), file_helper_{event_handlers},
      max_files_(max_files)
{
    using spdlog::details::os::path_exists;

    auto filename = calcFilename(base_filename_, file_pattern_, 0);
    auto should_rotate = path_exists(filename);

    file_helper_.open(filename);
    if (should_rotate) {
        rotate();
    }
    rotation_tp_ = nextRotation();

    for (auto &formatter : formatters_) {
        formatter.add_flag<LevelFormatter>('L');
        formatter.add_flag<TextFormatter>('v', false);
    }
    formatters_[0].set_pattern("%^[%H:%M:%S] [%t/%L]: [%n] %v%$");
    formatters_[1].set_pattern("%^[%H:%M:%S] [%t/%L]: %v%$");
}

spdlog::filename_t FileLogSink::filename()
{
    std::lock_guard lock(mutex_);
    return file_helper_.filename();
}

spdlog::filename_t FileLogSink::calcFilename(const spdlog::filename_t &base_filename,
                                             const spdlog::filename_t &file_pattern, std::size_t index)
{
    if (index == 0) {
        return base_filename;
    }

    const auto now_tp = spdlog::log_clock::now();
    return format(fmt::runtime(file_pattern), localtime(now_tp), index);
}

void FileLogSink::sink_it_(const spdlog::details::log_msg &msg)
{
    if (auto time = msg.time; time >= rotation_tp_) {
        rotate();
        rotation_tp_ = nextRotation();
    }

    spdlog::memory_buf_t formatted;
    auto &formatter = msg.logger_name.size() > 0 ? formatters_[0] : formatters_[1];
    formatter.format(msg, formatted);
    file_helper_.write(formatted);
}

void FileLogSink::flush_()
{
    file_helper_.flush();
}

void FileLogSink::rotate()
{
    using spdlog::details::os::filename_to_str;
    using spdlog::details::os::path_exists;

    file_helper_.close();
    for (auto i = max_files_; i > 0; --i) {
        spdlog::filename_t src = calcFilename(base_filename_, file_pattern_, i - 1);
        if (!path_exists(src)) {
            continue;
        }
        spdlog::filename_t target = calcFilename(base_filename_, file_pattern_, i);
        auto func = (i == 1) ? compress : rename;
        if (!func(src, target)) {
            // if failed try again after a small delay.
            spdlog::details::os::sleep_for_millis(100);
            if (!func(src, target)) {
                fmt::print(fmt::fg(fmt::color::red), "FileLogSink: failed rotating {} to {}\n", filename_to_str(src),
                           filename_to_str(target));
            }
        }
    }
    file_helper_.reopen(true);
}

std::tm FileLogSink::localtime(spdlog::log_clock::time_point tp)
{
    std::time_t time_now = spdlog::log_clock::to_time_t(tp);
    return spdlog::details::os::localtime(time_now);
}

spdlog::log_clock::time_point FileLogSink::nextRotation()
{
    auto now_tp = spdlog::log_clock::now();
    std::tm date = localtime(now_tp);
    date.tm_hour = 0;
    date.tm_min = 0;
    date.tm_sec = 0;
    auto rotation_tp = spdlog::log_clock::from_time_t(std::mktime(&date));
    if (rotation_tp > now_tp) {
        return rotation_tp;
    }
    return {rotation_tp + std::chrono::hours(24)};
}

bool FileLogSink::rename(const spdlog::filename_t &src_filename, const spdlog::filename_t &target_filename)
{
    (void)spdlog::details::os::remove(target_filename);
    return spdlog::details::os::rename(src_filename, target_filename) == 0;
}

bool FileLogSink::compress(const spdlog::filename_t &src_filename, const spdlog::filename_t &target_filename)
{
    try {
        (void)spdlog::details::os::remove(target_filename);
        std::ifstream input(src_filename, std::ios::in | std::ios::binary);
        if (!input) {
            return false;
        }
        zstr::ofstream output(target_filename, std::ios::out | std::ios::binary);
        if (!output) {
            return false;
        }
        output << input.rdbuf();
        output.flush();
        output.close();
        input.close();
        return spdlog::details::os::remove(src_filename) == 0;
    }
    catch (...) {
        return false;
    }
}

}  // namespace endstone::core
