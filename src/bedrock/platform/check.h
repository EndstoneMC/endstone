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

#include <entt/entt.hpp>
#include <fmt/format.h>
#include <fmt/os.h>
#include <fmt/std.h>

#include "bedrock/bedrock.h"
#include "bedrock/platform/callstack.h"

namespace Bedrock {

template <typename ErrorType>
struct ErrorInfo {
    ErrorInfo(ErrorType error, CallStack call_stack, std::vector<ErrorInfo> branches)
        : error(error), call_stack(std::move(call_stack)), branches(branches)
    {
    }
    [[nodiscard]] std::string message() const;
    ErrorType error;
    CallStack call_stack;
    std::vector<ErrorInfo> branches;
};
BEDROCK_STATIC_ASSERT_SIZE(ErrorInfo<std::error_code>, 64, 64);

namespace Detail {

template <typename T>
struct ErrorInfoBuilder {
    std::optional<T> value;
};

template <typename T>
    requires(!std::is_same_v<T, std::errc>)
ErrorInfoBuilder<T> createError(T value)
{
    return ErrorInfoBuilder<T>(value);
}

inline ErrorInfoBuilder<std::error_code> createError(std::errc ec)
{
    return ErrorInfoBuilder<std::error_code>(std::make_error_code(ec));
}

template <typename Context>
CallStack::FrameWithContext createFrame(std::pair<CallStack::Frame, Context> &&pair)
{
    return CallStack::FrameWithContext(std::move(pair.first), std::move(pair.second));
}

template <typename ErrorType, typename GetFrame>
ErrorInfo<ErrorType> addFrameToError(ErrorInfoBuilder<ErrorType> &&error, const GetFrame &get_frame)
{
    return {error.value.value(), {get_frame()}, {}};
}

template <typename ErrorType, typename GetFrame>
ErrorInfo<ErrorType> &&addFrameToError(ErrorInfo<ErrorType> &&error_info, const GetFrame &get_frame)
{
    error_info.call_stack.frames.emplace_back(get_frame());
    return std::move(error_info);
}

inline std::nullopt_t createContext()
{
    return std::nullopt;
}

inline CallStack::Context createContext(std::string value)
{
    return {std::move(value)};
}

inline std::back_insert_iterator<std::string> formatCallStackContexts(std::back_insert_iterator<std::string> back_it,
                                                                      const CallStack &call_stack)
{
    auto out = back_it;
    for (const auto &frame : call_stack.frames) {
        if (const auto &context = frame.context; context.has_value()) {
            out = fmt::format_to(out, "\n{}", context.value().value);
        }
    }
    return out;
}

}  // namespace Detail

template <typename ErrorType>
std::string ErrorInfo<ErrorType>::message() const
{
    std::string result = fmt::format("Error: {}", error);
    auto out = Detail::formatCallStackContexts(std::back_inserter(result), call_stack);
    for (auto &branch : branches) {
        out = fmt::format_to(out, "\n{}", branch.message());
    }
    return result;
}
}  // namespace Bedrock

#define BEDROCK_NEW_ERROR(ERROR)                                                                            \
    Bedrock::Detail::addFrameToError(Bedrock::Detail::createError(ERROR), [&]() {                            \
        constexpr static Bedrock::CallStack::Frame frame{                                                   \
            .filename_hash = entt::hashed_string::value(__FILE__), .filename = __FILE__, .line = __LINE__}; \
        return Bedrock::Detail::createFrame(std::make_pair(frame, Bedrock::Detail::createContext()));       \
    })

#define BEDROCK_NEW_ERROR_MESSAGE(ERROR, MESSAGE)                                                              \
    Bedrock::Detail::addFrameToError(Bedrock::Detail::createError(ERROR), [&]() {                               \
        constexpr static Bedrock::CallStack::Frame frame{                                                      \
            .filename_hash = entt::hashed_string::value(__FILE__), .filename = __FILE__, .line = __LINE__};    \
        return Bedrock::Detail::createFrame(std::make_pair(frame, Bedrock::Detail::createContext(MESSAGE))); \
    })

#define BEDROCK_RETHROW(RESULT)                                                                             \
    Bedrock::Detail::addFrameToError(std::move(RESULT.discardError().error()), [&]() {                       \
        constexpr static Bedrock::CallStack::Frame frame{                                                   \
            .filename_hash = entt::hashed_string::value(__FILE__), .filename = __FILE__, .line = __LINE__}; \
        return Bedrock::Detail::createFrame(std::make_pair(frame, Bedrock::Detail::createContext()));       \
    })
