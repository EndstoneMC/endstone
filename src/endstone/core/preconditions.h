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

#include <source_location>
#include <stdexcept>
#include <string_view>

#include <fmt/format.h>

namespace endstone {
class Preconditions {
public:
    template <typename... Args>
    static void checkArgument(bool condition, fmt::format_string<Args...> format, Args &&...args,
                              const std::source_location loc = std::source_location::current())
    {
        if (!condition) {
            auto message = fmt::format(format, std::forward<Args>(args)...);
            auto full = fmt::format("{} [{}:{}]", message, loc.file_name(), loc.line());
            throw std::invalid_argument(full);
        }
    }
};
}  // namespace endstone
