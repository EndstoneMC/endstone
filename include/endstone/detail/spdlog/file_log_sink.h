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

#include <cstdint>
#include <ctime>
#include <iomanip>
#include <mutex>
#include <sstream>

#include <spdlog/details/file_helper.h>
#include <spdlog/details/os.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>

namespace endstone::detail {

class FileLogSink final : public spdlog::sinks::base_sink<std::mutex> {
public:
    explicit FileLogSink(spdlog::filename_t base_filename, spdlog::filename_t file_pattern, std::uint16_t max_files,
                         const spdlog::file_event_handlers &event_handlers = {});
    spdlog::filename_t filename();
    static spdlog::filename_t calcFilename(const spdlog::filename_t &base_filename,
                                           const spdlog::filename_t &file_pattern, std::size_t index);

protected:
    void sink_it_(const spdlog::details::log_msg &msg) override;
    void flush_() override;

private:
    void rotate();
    static bool rename(const spdlog::filename_t &src_filename, const spdlog::filename_t &target_filename);

    static std::tm localtime(spdlog::log_clock::time_point tp);
    static spdlog::log_clock::time_point nextRotation();

    spdlog::filename_t base_filename_;
    spdlog::filename_t file_pattern_;
    spdlog::log_clock::time_point rotation_tp_;
    spdlog::details::file_helper file_helper_;
    uint16_t max_files_;
};

}  // namespace endstone::detail
