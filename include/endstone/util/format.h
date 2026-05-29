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
#include <iterator>
#include <string>
#include <string_view>

namespace endstone::detail {

/**
 * Quotes and escapes a string the way the `{:?}` debug presentation does.
 *
 * `std::format` only gained the `?` (debug) specifier in C++23, so this provides equivalent behaviour under C++20.
 */
inline std::string quoted(std::string_view value)
{
    std::string out;
    out.reserve(value.size() + 2);
    out.push_back('"');
    for (const char c : value) {
        switch (c) {
        case '"':
            out += "\\\"";
            break;
        case '\\':
            out += "\\\\";
            break;
        case '\n':
            out += "\\n";
            break;
        case '\t':
            out += "\\t";
            break;
        case '\r':
            out += "\\r";
            break;
        default:
            out.push_back(c);
            break;
        }
    }
    out.push_back('"');
    return out;
}

/**
 * A lightweight, formattable view over a range that renders its elements separated by a delimiter.
 *
 * This is a drop-in replacement for `fmt::join`, which has no equivalent in `std::format` (C++20).
 * It allocates nothing of its own: elements are written straight to the output iterator.
 */
template <typename It, typename Sentinel>
struct JoinView {
    It begin;
    Sentinel end;
    std::string_view sep;
};

template <typename It, typename Sentinel>
JoinView<It, Sentinel> join(It begin, Sentinel end, std::string_view sep)
{
    return {begin, end, sep};
}

template <typename Range>
auto join(const Range &range, std::string_view sep)
{
    return join(std::begin(range), std::end(range), sep);
}

}  // namespace endstone::detail

template <typename It, typename Sentinel>
struct std::formatter<endstone::detail::JoinView<It, Sentinel>> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const endstone::detail::JoinView<It, Sentinel> &view, FormatContext &ctx) const
    {
        auto out = ctx.out();
        for (auto it = view.begin; it != view.end; ++it) {
            if (it != view.begin) {
                out = std::format_to(out, "{}", view.sep);
            }
            out = std::format_to(out, "{}", *it);
        }
        return out;
    }
};
