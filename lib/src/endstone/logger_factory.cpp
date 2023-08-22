//
// Created by Vincent on 22/08/2023.
//

#include "logger_factory.h"
#include "bedrock/bedrock_log.h"

class BedrockLoggerAdapter : public Logger
{
  public:
    explicit BedrockLoggerAdapter(std::string name) : name_(std::move(name))
    {
    }

    void setLevel(LogLevel level) override
    {
        level_ = level;
    }

    bool isEnabledFor(LogLevel level) const noexcept override
    {
        return level >= level_;
    }

    std::string_view getName() const override
    {
        return name_;
    }

    void log(LogLevel level, const std::string &message) const override
    {
        if (isEnabledFor(level))
        {
            BedrockLog::log_va(BedrockLog::LogCategory::All,
                               1,
                               BedrockLog::LogRule::Default,
                               BedrockLog::LogAreaID::Server,
                               level,
                               __FUNCTION__,
                               __LINE__,
                               message.c_str(),
                               {});
        }
    }

  private:
    LogLevel level_;
    std::string name_;
};

std::shared_ptr<Logger> LoggerFactory::getLogger(const std::string &name)
{
    return std::make_shared<BedrockLoggerAdapter>(name);
}
