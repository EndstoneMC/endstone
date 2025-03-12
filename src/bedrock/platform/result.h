// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <system_error>

#include <bedrock/core/debug/bedrock_event_logger.h>
#include <nonstd/expected.hpp>

#include "bedrock/bedrock.h"
#include "bedrock/platform/check.h"

namespace Bedrock {

class ResultLogger {
    template <typename U, typename ErrorU>
    friend class Result;

    static void log(std::optional<LogLevel> log_level, std::optional<LogAreaID> log_area, const std::string &error,
                    const CallStack &call_stack)
    {
        va_list args = nullptr;
        const auto message = fmt::format("Error: {}\nCall stack:{}", error, call_stack);
        log_va(BedrockLog::LogArea, {1}, BedrockLog::DefaultRules, log_area.value_or(LogAreaID::All),
               log_level.value_or(Error), __FUNCTION__, __LINE__, message.c_str(), args);
    }
};

template <typename T, typename E = std::error_code>
using LoggedResult = nonstd::expected<T, ErrorInfo<E>>;

template <typename T, typename E = std::error_code>
class Result : nonstd::expected<T, ErrorInfo<E>> {
public:
    using nonstd::expected<T, ErrorInfo<E>>::expected;

    template <typename ErrorTypeU>
    Result(ErrorInfo<ErrorTypeU> error_info) : nonstd::expected<T, ErrorInfo<E>>(nonstd::make_unexpected(error_info))
    {
    }

    LoggedResult<T, E> &&logError(std::optional<LogLevel> log_level = std::nullopt,
                                  std::optional<LogAreaID> log_area = std::nullopt)
    {
        if (!nonstd::expected<T, ErrorInfo<E>>::has_value()) {
            ErrorInfo<E> &error_info = nonstd::expected<T, ErrorInfo<E>>::error();
            ResultLogger::log(log_level, log_area, fmt::format("{}", error_info.error), error_info.call_stack);
        }
        return std::move(*this);
    }

    bool ignoreError()
    {
        return nonstd::expected<T, ErrorInfo<E>>::has_value();
    }

    nonstd::expected<T, ErrorInfo<E>> &&discardError()
    {
        return std::move(*this);
    }

    [[nodiscard]] const nonstd::expected<T, ErrorInfo<E>> &asExpected() const
    {
        return *this;
    }
};
BEDROCK_STATIC_ASSERT_SIZE(Result<unsigned char>, 72, 72);

}  // namespace Bedrock
