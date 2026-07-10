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

#include <format>
#include <stdexcept>
#include <utility>

namespace endstone {
/**
 * Static helpers for validating method arguments and object state.
 */
class Preconditions {
public:
    /**
     * Ensures the truth of an expression involving one or more parameters to the calling method.
     *
     * @param condition The expression that must hold.
     * @param format A format string describing the failure, used when the condition is `false`.
     * @param args Arguments to substitute into the format string.
     * @throws std::invalid_argument if condition is `false`.
     */
    template <typename... Args>
    static void checkArgument(bool condition, std::format_string<Args...> format, Args &&...args)
    {
        if (!condition) {
            auto message = std::format(format, std::forward<Args>(args)...);
            throw std::invalid_argument(message);
        }
    }

    /**
     * Ensures the truth of an expression involving the state of the calling instance.
     *
     * @param condition The expression that must hold.
     * @param format A format string describing the failure, used when the condition is `false`.
     * @param args Arguments to substitute into the format string.
     * @throws std::runtime_error if condition is `false`.
     */
    template <typename... Args>
    static void checkState(bool condition, std::format_string<Args...> format, Args &&...args)
    {
        if (!condition) {
            auto message = std::format(format, std::forward<Args>(args)...);
            throw std::runtime_error(message);
        }
    }
};
}  // namespace endstone
