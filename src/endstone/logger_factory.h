//
// Created by Vincent on 22/08/2023.
//

#pragma once

#include "endstone/logger.h"

class AbstractLogger : public Logger {
public:
    explicit AbstractLogger(std::string name);

    void setLevel(Level level) override;
    bool isEnabledFor(Level level) const noexcept override;
    std::string_view getName() const override;

    void log(Level level, const std::string &message) const override = 0;

private:
    Level level_;
    std::string name_;
};

class BedrockLoggerAdapter : public AbstractLogger {

public:
    using AbstractLogger::AbstractLogger;

    void log(Level level, const std::string &message) const override;
};

class LoggerFactory {
public:
    static std::shared_ptr<Logger> getLogger(const std::string &name);
};

