//
// Created by Vincent on 07/08/2023.
//

#include <memory>

#include "common.h"
#include "endstone/logger.h"

#define BEDROCK_LOG(level)                                                                                             \
    if (level >= level_)                                                                                               \
    {                                                                                                                  \
        va_list args;                                                                                                  \
        va_start(args, format);                                                                                        \
        BedrockLog::log_va(BedrockLog::LogCategory::All,                                                               \
                           1,                                                                                          \
                           BedrockLog::LogRule::Default,                                                               \
                           BedrockLog::LogAreaID::Server,                                                              \
                           level,                                                                                      \
                           __FUNCTION__,                                                                               \
                           __LINE__,                                                                                   \
                           format,                                                                                     \
                           args);                                                                                      \
        va_end(args);                                                                                                  \
    }

void Logger::log(LogLevel level, char const *format, ...) const
{
    BEDROCK_LOG(level)
}

void Logger::verbose(const char *format, ...) const
{
    BEDROCK_LOG(LogLevel::Verbose)
}

void Logger::info(const char *format, ...) const
{
    BEDROCK_LOG(LogLevel::Info)
}

void Logger::warning(const char *format, ...) const
{
    BEDROCK_LOG(LogLevel::Warning)
}

void Logger::error(const char *format, ...) const
{
    BEDROCK_LOG(LogLevel::Error)
}

void Logger::setLevel(LogLevel level)
{
    level_ = level;
}

const std::string &Logger::getName() const
{
    return name_;
}

Logger &Logger::getLogger(const std::string &name)
{
    static std::map<std::string, Logger> loggers;
    static std::mutex mutex;

    std::scoped_lock<std::mutex> lock(mutex);
    auto it = loggers.find(name);
    if (it == loggers.end())
    {
        it = loggers.insert({name, Logger(name)}).first;
    }

    return it->second;
}
