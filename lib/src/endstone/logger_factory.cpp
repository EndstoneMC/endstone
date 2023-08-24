//
// Created by Vincent on 22/08/2023.
//

#include "logger_factory.h"

#include "bedrock/bedrock_log.h"

AbstractLogger::AbstractLogger(std::string name) : level_(LogLevel::Info), name_(std::move(name)) {}

void AbstractLogger::setLevel(LogLevel level)
{
    level_ = level;
}

bool AbstractLogger::isEnabledFor(LogLevel level) const noexcept
{
    return level >= level_;
}

std::string_view AbstractLogger::getName() const
{
    return name_;
}

class BedrockLoggerAdapter : public AbstractLogger {

public:
    using AbstractLogger::AbstractLogger;

    void log(LogLevel level, const std::string &message) const override
    {
        if (isEnabledFor(level)) {
            BedrockLog::log_va(BedrockLog::LogCategory::All, 1, BedrockLog::LogRule::Default,
                               BedrockLog::LogAreaID::Server, level, __FUNCTION__, __LINE__, message.c_str(), {});
        }
    }
};

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
