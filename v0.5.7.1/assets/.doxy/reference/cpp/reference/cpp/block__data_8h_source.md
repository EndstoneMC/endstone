

# File block\_data.h

[**File List**](files.md) **>** [**block**](dir_faca67fc60a7463eb1bd30eabe023cf1.md) **>** [**block\_data.h**](block__data_8h.md)

[Go to the documentation of this file](block__data_8h.md)


```C++
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

#include <string>
#include <unordered_map>
#include <variant>

#include <fmt/format.h>

#include "endstone/endstone.h"

namespace endstone {

using BlockStates = std::unordered_map<std::string, std::variant<bool, std::string, int>>;

class BlockData : public std::enable_shared_from_this<BlockData> {
public:
    virtual ~BlockData() = default;

    [[nodiscard]] virtual std::string getType() const = 0;

    [[nodiscard]] virtual BlockStates getBlockStates() const = 0;
};

}  // namespace endstone

namespace fmt {

template <>
struct formatter<endstone::BlockStates::mapped_type> : formatter<string_view> {
    using Type = endstone::BlockStates::mapped_type;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return std::visit(endstone::overloaded{
                              [&ctx](const std::string &arg) { return format_to(ctx.out(), "{:?}", arg); },
                              [&ctx](auto &&arg) { return format_to(ctx.out(), "{}", arg); },
                          },
                          val);
    }
};

template <>
struct formatter<endstone::BlockStates::value_type> : formatter<string_view> {
    using Type = endstone::BlockStates::value_type;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return format_to(ctx.out(), "{:?}={}", val.first, val.second);
    }
};

template <>
struct formatter<endstone::BlockStates> : formatter<string_view> {
    using Type = endstone::BlockStates;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return format_to(ctx.out(), "[{}]", join(val.begin(), val.end(), ","));
    }
};

template <>
struct formatter<endstone::BlockData> : formatter<string_view> {
    using Type = endstone::BlockData;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return format_to(ctx.out(), "BlockData(type={}, block_states={})", val.getType(), val.getBlockStates());
    }
};
}  // namespace fmt
```


