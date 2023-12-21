// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include "endstone_core/logger_factory.h"

#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>

#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/ansicolor_sink-inl.h>
#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/spdlog.h>

template class SPDLOG_API spdlog::sinks::ansicolor_stdout_sink<spdlog::details::console_mutex>;

class LevelFormatter : public spdlog::custom_flag_formatter {
public:
    void format(const spdlog::details::log_msg &msg, const std::tm &, spdlog::memory_buf_t &dest) override
    {
        static const std::unordered_map<spdlog::level::level_enum, std::string_view> SpdlogLevelNames = {
            {spdlog::level::trace, "TRACE"},  {spdlog::level::debug, "DEBUG"}, {spdlog::level::info, "INFO"},
            {spdlog::level::warn, "WARNING"}, {spdlog::level::err, "ERROR"},   {spdlog::level::critical, "CRITICAL"},
            {spdlog::level::off, "OFF"},
        };

        const auto level_name = SpdlogLevelNames.find(msg.level)->second;
        const auto *buf_ptr = level_name.data();
        dest.append(buf_ptr, buf_ptr + level_name.size());
    }

    [[nodiscard]] std::unique_ptr<custom_flag_formatter> clone() const override
    {
        return spdlog::details::make_unique<LevelFormatter>();
    }
};

SpdLogAdapter::SpdLogAdapter(std::shared_ptr<spdlog::logger> logger) : logger_(std::move(logger)) {}

void SpdLogAdapter::log(Logger::Level level, const std::string &message) const
{
    if (isEnabledFor(level)) {
        logger_->log(static_cast<spdlog::level::level_enum>(level), message);
    }
}

void SpdLogAdapter::setLevel(Logger::Level level)
{
    logger_->set_level(static_cast<spdlog::level::level_enum>(level));
}

bool SpdLogAdapter::isEnabledFor(Logger::Level level) const
{
    return logger_->should_log(static_cast<spdlog::level::level_enum>(level));
}

std::string_view SpdLogAdapter::getName() const
{
    return logger_->name();
}

Logger &LoggerFactory::getLogger(const std::string &name)
{
    static std::mutex mutex;
    static std::unordered_map<std::string, SpdLogAdapter> loggers;

    std::scoped_lock<std::mutex> lock(mutex);
    auto it = loggers.find(name);
    if (it != loggers.end()) {
        return it->second;
    }

    auto formatter = std::make_unique<spdlog::pattern_formatter>();
    formatter->add_flag<LevelFormatter>('L').set_pattern("%^[%Y-%m-%d %H:%M:%S.%e %L] [%n] %v%$");

    auto console_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
    console_sink->set_formatter(std::move(formatter));
    console_sink->set_color(spdlog::level::info, console_sink->reset);

    auto console = std::make_shared<spdlog::logger>(name, console_sink);
    spdlog::register_logger(console);
    it = loggers.emplace(name, SpdLogAdapter(console)).first;
    return it->second;
}
