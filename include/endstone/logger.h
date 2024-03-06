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

#include <string>
#include <utility>

#include <fmt/format.h>

namespace endstone {

/**
 * @brief Logger class which can format and output varies levels of logs.
 */
class Logger {
public:
    /**
     * @brief Specifies the log level.
     */
    enum Level : uint8_t {
        Trace = 0,
        Debug = 1,
        Info = 2,
        Warning = 3,
        Error = 4,
        Critical = 5,
        Off = 6,
    };

    Logger() = default;

    virtual ~Logger() = default;
    /**
     * @brief Set the logging level for this Logger instance.
     *
     * @param level The desired log level
     */
    virtual void setLevel(Level level) = 0;

    /**
     * @brief Check if the Logger instance is enabled for the given log Level.
     *
     * @param level The log level to check
     * @return true if the logger is enabled for the Level, false otherwise.
     */
    [[nodiscard]] virtual bool isEnabledFor(Level level) const = 0;

    /**
     * @brief Get the name of this Logger instance.
     *
     * @return The name of this Logger instance.
     */
    [[nodiscard]] virtual std::string_view getName() const = 0;

    /**
     * @brief Log a message at the given level.
     *
     * @param level The level at which the message should be logged.
     * @param message The message to log.
     */
    virtual void log(Level level, const std::string &message) const = 0;

    template <typename... Args>
    void log(Level level, const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(level, fmt::format(format, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void trace(const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(Level::Trace, fmt::format(format, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void debug(const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(Level::Debug, fmt::format(format, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void info(const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(Level::Info, fmt::format(format, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void warning(const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(Level::Warning, fmt::format(format, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void error(const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(Level::Error, fmt::format(format, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void critical(const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(Level::Critical, fmt::format(format, std::forward<Args>(args)...));
    }
};

}  // namespace endstone
