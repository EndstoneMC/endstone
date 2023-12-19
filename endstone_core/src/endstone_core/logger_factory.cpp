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

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

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

    auto console = spdlog::stdout_color_mt(name);
    it = loggers.emplace(name, SpdLogAdapter(console)).first;
    return it->second;
}
