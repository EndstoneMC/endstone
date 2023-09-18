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

#include "endstone/logger_factory.h"

#include <mutex>
#include <unordered_map>
#include <utility>

#include "bedrock/bedrock_log.h"

AbstractLogger::AbstractLogger(std::string name) : level_(Level::Info), name_(std::move(name)) {}

void AbstractLogger::setLevel(Level level)
{
    level_ = level;
}

bool AbstractLogger::isEnabledFor(Level level) const noexcept
{
    return level >= level_;
}

std::string_view AbstractLogger::getName() const
{
    return name_;
}

void BedrockLoggerAdapter::log(Level level, const std::string &message) const
{
    if (isEnabledFor(level)) {
        uint32_t log_level;

        switch (level) {
        case Level::Verbose:
            log_level = 1;
            break;
        case Level::Info:
            log_level = 2;
            break;
        case Level::Warning:
            log_level = 4;
            break;
        case Level::Error:
            log_level = 8;
            break;
        default:
            log_level = 0;
        }

        BedrockLog::log_va(BedrockLog::LogCategory::All, 1, BedrockLog::LogRule::Default, LogAreaID::Server, log_level,
                           __FUNCTION__, __LINE__, message.c_str(), {});
    }
}

Logger &LoggerFactory::getLogger(const std::string &name)
{
    static std::mutex mutex;
    std::scoped_lock<std::mutex> lock(mutex);

    static std::unordered_map<std::string, BedrockLoggerAdapter> loggers;
    auto it = loggers.find(name);
    if (it == loggers.end()) {
        it = loggers.insert({name, BedrockLoggerAdapter(name)}).first;
    }

    return it->second;
}
