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

#include <cstdint>
#include <format>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace endstone {

/**
 * Logger class which can format and output varies levels of logs.
 */
class Logger {
public:
    /**
     * Specifies the log level.
     */
    enum Level : std::uint8_t {
        /**
         * The most fine-grained level, used for detailed tracing.
         */
        Trace = 0,
        /**
         * Detailed information useful for debugging.
         */
        Debug = 1,
        /**
         * Informational messages about normal operation.
         */
        Info = 2,
        /**
         * Indicates a potential problem that does not stop execution.
         */
        Warning = 3,
        /**
         * Indicates a failure that prevents an operation from completing.
         */
        Error = 4,
        /**
         * Indicates a severe failure that may abort the server.
         */
        Critical = 5,
        /**
         * Disables all logging when set as the logger's level.
         */
        Off = 6,
    };

    Logger() = default;

    virtual ~Logger() = default;
    /**
     * Set the logging level for this Logger instance.
     *
     * @param level The desired log level
     */
    virtual void setLevel(Level level) = 0;

    /**
     * Check if the Logger instance is enabled for the given log Level.
     *
     * @param level The log level to check
     * @return `true` if the logger is enabled for the Level, `false` otherwise.
     */
    [[nodiscard]] virtual bool isEnabledFor(Level level) const = 0;

    /**
     * Get the name of this Logger instance.
     *
     * @return The name of this Logger instance.
     */
    [[nodiscard]] virtual std::string_view getName() const = 0;

    /**
     * Log a message at the given level.
     *
     * @param level The level at which the message should be logged.
     * @param message The message to log.
     */
    virtual void log(Level level, std::string_view message) const = 0;

    /**
     * Log a formatted message at the given level.
     *
     * Formats the message using std::format and logs the result. If formatting throws,
     * the exception message is logged at the Error level instead.
     *
     * @param level The level at which the message should be logged.
     * @param format The format string.
     * @param args The arguments to substitute into the format string.
     */
    template <typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int> = 0>
    void log(Level level, const std::format_string<Args...> format, Args &&...args) const
    {
        try {
            log(level, std::format(format, std::forward<Args>(args)...));
        }
        catch (std::exception &e) {
            log(Error, e.what());
        }
    }

    /**
     * Log a message at the Trace level.
     *
     * @param message The message to log.
     */
    void trace(const std::string_view message) const { log(Trace, message); }

    /**
     * Log a formatted message at the Trace level.
     *
     * @param format The format string.
     * @param args The arguments to substitute into the format string.
     */
    template <typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int> = 0>
    void trace(const std::format_string<Args...> format, Args &&...args) const
    {
        log(Trace, format, std::forward<Args>(args)...);
    }

    /**
     * Log a message at the Debug level.
     *
     * @param message The message to log.
     */
    void debug(const std::string_view message) const { log(Debug, message); }

    /**
     * Log a formatted message at the Debug level.
     *
     * @param format The format string.
     * @param args The arguments to substitute into the format string.
     */
    template <typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int> = 0>
    void debug(const std::format_string<Args...> format, Args &&...args) const
    {
        log(Debug, format, std::forward<Args>(args)...);
    }

    /**
     * Log a message at the Info level.
     *
     * @param message The message to log.
     */
    void info(const std::string_view message) const { log(Info, message); }

    /**
     * Log a formatted message at the Info level.
     *
     * @param format The format string.
     * @param args The arguments to substitute into the format string.
     */
    template <typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int> = 0>
    void info(const std::format_string<Args...> format, Args &&...args) const
    {
        log(Info, format, std::forward<Args>(args)...);
    }

    /**
     * Log a message at the Warning level.
     *
     * @param message The message to log.
     */
    void warning(const std::string_view message) const { log(Warning, message); }

    /**
     * Log a formatted message at the Warning level.
     *
     * @param format The format string.
     * @param args The arguments to substitute into the format string.
     */
    template <typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int> = 0>
    void warning(const std::format_string<Args...> format, Args &&...args) const
    {
        log(Warning, format, std::forward<Args>(args)...);
    }

    /**
     * Log a message at the Error level.
     *
     * @param message The message to log.
     */
    void error(const std::string_view message) const { log(Error, message); }

    /**
     * Log a formatted message at the Error level.
     *
     * @param format The format string.
     * @param args The arguments to substitute into the format string.
     */
    template <typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int> = 0>
    void error(const std::format_string<Args...> format, Args &&...args) const
    {
        log(Error, format, std::forward<Args>(args)...);
    }

    /**
     * Log a message at the Critical level.
     *
     * @param message The message to log.
     */
    void critical(const std::string_view message) const { log(Critical, message); }

    /**
     * Log a formatted message at the Critical level.
     *
     * @param format The format string.
     * @param args The arguments to substitute into the format string.
     */
    template <typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int> = 0>
    void critical(const std::format_string<Args...> format, Args &&...args) const
    {
        log(Critical, format, std::forward<Args>(args)...);
    }
};

}  // namespace endstone
