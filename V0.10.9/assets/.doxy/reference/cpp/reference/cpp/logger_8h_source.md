

# File logger.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**logger.h**](logger_8h.md)

[Go to the documentation of this file](logger_8h.md)


```C++
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

class Logger {
public:
    enum Level : std::uint8_t {
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
    virtual void setLevel(Level level) = 0;

    [[nodiscard]] virtual bool isEnabledFor(Level level) const = 0;

    [[nodiscard]] virtual std::string_view getName() const = 0;

    virtual void log(Level level, std::string_view message) const = 0;

    template <typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int> = 0>
    void log(Level level, const fmt::format_string<Args...> format, Args &&...args) const
    {
        try {
            log(level, fmt::format(format, std::forward<Args>(args)...));
        }
        catch (std::exception &e) {
            log(Error, e.what());
        }
    }

    void trace(const std::string_view message) const
    {
        log(Trace, message);
    }

    template <typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int> = 0>
    void trace(const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(Trace, format, std::forward<Args>(args)...);
    }

    void debug(const std::string_view message) const
    {
        log(Debug, message);
    }

    template <typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int> = 0>
    void debug(const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(Debug, format, std::forward<Args>(args)...);
    }

    void info(const std::string_view message) const
    {
        log(Info, message);
    }

    template <typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int> = 0>
    void info(const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(Info, format, std::forward<Args>(args)...);
    }

    void warning(const std::string_view message) const
    {
        log(Warning, message);
    }

    template <typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int> = 0>
    void warning(const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(Warning, format, std::forward<Args>(args)...);
    }

    void error(const std::string_view message) const
    {
        log(Error, message);
    }

    template <typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int> = 0>
    void error(const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(Error, format, std::forward<Args>(args)...);
    }

    void critical(const std::string_view message) const
    {
        log(Critical, message);
    }

    template <typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int> = 0>
    void critical(const fmt::format_string<Args...> format, Args &&...args) const
    {
        log(Critical, format, std::forward<Args>(args)...);
    }
};

}  // namespace endstone
```


