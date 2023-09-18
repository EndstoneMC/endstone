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

#pragma once

#include <memory>
#include <string>

#include "endstone/logger.h"

class AbstractLogger : public Logger {
public:
    explicit AbstractLogger(std::string name);

    void setLevel(Level level) override;
    [[nodiscard]] bool isEnabledFor(Level level) const noexcept override;
    [[nodiscard]] std::string_view getName() const override;

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
    static Logger &getLogger(const std::string &name);
};
