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

#include "endstone/core/logger_factory.h"

#include <mutex>
#include <string>
#include <unordered_map>

#include <ll/api/mod/NativeMod.h>

namespace endstone::core {

static ll::io::LogLevel transformLevel(Logger::Level level)
{
    switch (level) {
    case Logger::Trace:
        return ll::io::LogLevel::Trace;
    case Logger::Debug:
        return ll::io::LogLevel::Debug;
    case Logger::Info:
        return ll::io::LogLevel::Info;
    case Logger::Warning:
        return ll::io::LogLevel::Warn;
    case Logger::Error:
        return ll::io::LogLevel::Error;
    case Logger::Critical:
        return ll::io::LogLevel::Fatal;
    case Logger::Off:
        return ll::io::LogLevel::Off;
    default:
        LL_UNREACHABLE;
    }
}

static Logger::Level transformLevel(ll::io::LogLevel level)
{
    switch (level) {
    case ll::io::LogLevel::Trace:
        return Logger::Trace;
    case ll::io::LogLevel::Debug:
        return Logger::Debug;
    case ll::io::LogLevel::Info:
        return Logger::Info;
    case ll::io::LogLevel::Warn:
        return Logger::Warning;
    case ll::io::LogLevel::Error:
        return Logger::Error;
    case ll::io::LogLevel::Fatal:
        return Logger::Critical;
    case ll::io::LogLevel::Off:
        return Logger::Off;
    default:
        LL_UNREACHABLE;
    }
}

class LogAdapter : public Logger {
public:
    explicit LogAdapter(std::shared_ptr<ll::io::Logger> logger) : logger_(std::move(logger)) {}
    void setLevel(Level level) override
    {
        logger_->setLevel(transformLevel(level));
    }
    [[nodiscard]] bool isEnabledFor(Level level) const override
    {
        return logger_->shouldLog(transformLevel(level));
    }
    [[nodiscard]] std::string_view getName() const override
    {
        return logger_->getTitle();
    }
    void log(Level level, std::string_view message) const override
    {
        logger_->log(transformLevel(level), message);
    }

private:
    std::shared_ptr<ll::io::Logger> logger_;
};

Logger &LoggerFactory::getLogger(const std::string &)
{
    // we don't want to name it
    static LogAdapter logger(ll::mod::NativeMod::current()->getLogger().shared_from_this());

    return logger;
}

}  // namespace endstone::core
