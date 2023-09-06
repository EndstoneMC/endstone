//
// Created by Vincent on 02/08/2023.
//

#ifndef ENDSTONE_LOGGER_H
#define ENDSTONE_LOGGER_H

#include "common.h"

class Logger {
public:
    /**
     * @enum LogLevel
     * @brief Specifies the log level.
     */
    enum Level {
        All = 0,
        Verbose = 1,
        Info = 2,
        Warning = 3,
        Error = 4,
    };

    virtual ~Logger() = default;
    virtual void setLevel(Level level) = 0;
    virtual bool isEnabledFor(Level level) const noexcept = 0;
    virtual std::string_view getName() const = 0;
    virtual void log(Level level, const std::string &message) const = 0;

    template <typename... Args>
    void log(Level level, const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(level, fmt::format(format, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void verbose(const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(Level::Verbose, fmt::format(format, std::forward<Args>(args)...));
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
};

#endif // ENDSTONE_LOGGER_H
