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

#include "endstone/detail/spdlog/file_log_sink.h"

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <spdlog/details/os.h>
#include <spdlog/pattern_formatter.h>

#include "endstone/detail/spdlog/level_formatter.h"
#include "endstone/detail/spdlog/text_formatter.h"

namespace endstone::detail {

FileLogSink::FileLogSink(spdlog::filename_t base_filename, spdlog::filename_t file_pattern, uint16_t max_files,
                         const spdlog::file_event_handlers &event_handlers)
    : base_filename_(std::move(base_filename)), file_pattern_(std::move(file_pattern)), max_files_(max_files),
      file_helper_{event_handlers}
{
    using spdlog::details::os::path_exists;

    auto filename = calcFilename(base_filename_, file_pattern_, 0);
    auto should_rotate = path_exists(filename);

    file_helper_.open(filename);
    if (should_rotate) {
        rotate();
    }
    rotation_tp_ = nextRotation();

    auto *formatter = dynamic_cast<spdlog::pattern_formatter *>(formatter_.get());
    formatter->add_flag<LevelFormatter>('L');
    formatter->add_flag<TextFormatter>('v', false);
    formatter->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e %L] [%n] %v%$");
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
    formatter_->format(msg, formatted);
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

        if (!rename(src, target)) {
            // if failed try again after a small delay.
            spdlog::details::os::sleep_for_millis(100);
            if (!rename(src, target)) {
                spdlog::throw_spdlog_ex(
                    "FileLogSink: failed renaming " + filename_to_str(src) + " to " + filename_to_str(target), errno);
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

}  // namespace endstone::detail
