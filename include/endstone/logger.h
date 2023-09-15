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
    [[nodiscard]] virtual bool isEnabledFor(Level level) const noexcept = 0;
    [[nodiscard]] virtual std::string_view getName() const = 0;
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
