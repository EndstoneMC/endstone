//
// Created by Vincent on 22/08/2023.
//

#include "logger_factory.h"

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

std::shared_ptr<Logger> LoggerFactory::getLogger(const std::string &name)
{
    static std::mutex mutex;
    std::scoped_lock<std::mutex> lock(mutex);

    static std::map<std::string, std::shared_ptr<BedrockLoggerAdapter>> loggers;
    auto it = loggers.find(name);
    if (it == loggers.end()) {
        it = loggers.insert({name, std::make_shared<BedrockLoggerAdapter>(name)}).first;
    }

    return it->second;
}
