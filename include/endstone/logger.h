//
// Created by Vincent on 02/08/2023.
//

#ifndef ENDSTONE_LOGGER_H
#define ENDSTONE_LOGGER_H

#include "common.h"

/**
 * @enum LogLevel
 * @brief Specifies the log level.
 */
enum class LogLevel {
    All = -1,
    Verbose = 1,
    Info = 2,
    Warning = 4,
    Error = 8,
};

class Logger {
public:
    virtual void setLevel(LogLevel level) = 0;
    virtual bool isEnabledFor(LogLevel level) const noexcept = 0;
    virtual std::string_view getName() const = 0;
    virtual void log(LogLevel level, const std::string &message) const = 0;

    template <typename... Args>
    void log(LogLevel level, const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(level, fmt::format(format, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void verbose(const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(LogLevel::Verbose, fmt::format(format, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void info(const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(LogLevel::Info, fmt::format(format, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void warning(const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(LogLevel::Warning, fmt::format(format, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void error(const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(LogLevel::Error, fmt::format(format, std::forward<Args>(args)...));
    }
};

#endif // ENDSTONE_LOGGER_H
