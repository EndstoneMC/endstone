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
#include <string_view>

namespace brstd {
struct source_location {
    static source_location current(std::source_location location = std::source_location::current())
    {
        return location;
    };
    source_location(std::source_location &&location)
        : source_location_(location), file_name_size_(std::strlen(source_location_.file_name())),
          function_name_size_(std::strlen(source_location_.function_name()))
    {
    }

    [[nodiscard]] const char *file_name() const
    {
        return source_location_.file_name();
    }

    [[nodiscard]] const char *function_name() const
    {
        return source_location_.function_name();
    }

    [[nodiscard]] std::uint_least32_t line() const
    {
        return source_location_.line();
    }

    [[nodiscard]] std::uint_least32_t column() const
    {
        return source_location_.column();
    }

    [[nodiscard]] std::string_view file_name_view() const
    {
        return {file_name(), file_name_size_};
    }

    [[nodiscard]] std::string_view function_name_view() const
    {
        return {function_name(), file_name_size_};
    }

private:
    std::source_location source_location_;
    uint32_t file_name_size_;
    uint32_t function_name_size_;
};
}  // namespace brstd
