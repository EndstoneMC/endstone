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

#include <fmt/format.h>
#include <magic_enum/magic_enum.hpp>

#include "bedrock/diagnostics/log_area.h"
#include "bedrock/diagnostics/log_level.h"

namespace fmt {
template <>
struct formatter<LogAreaID> : formatter<string_view> {
    template <typename FormatContext>
    auto format(const LogAreaID &val, FormatContext &ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "{}", std::string(magic_enum::enum_name(val)));
    }
};

template <>
struct formatter<Bedrock::LogLevel> : formatter<string_view> {
    template <typename FormatContext>
    auto format(const Bedrock::LogLevel &val, FormatContext &ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "{}", std::string(magic_enum::enum_name(val)));
    }
};
}  // namespace fmt
