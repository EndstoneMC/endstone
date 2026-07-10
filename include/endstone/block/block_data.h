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

#include <cstdint>
#include <format>
#include <string>
#include <unordered_map>
#include <variant>

#include "endstone/util/format.h"
#include "endstone/variant.h"

namespace endstone {

class BlockType;

using BlockStates = std::unordered_map<std::string, std::variant<bool, std::string, int>>;

/**
 * Represents the data related to a live block.
 */
class BlockData {
public:
    virtual ~BlockData() = default;

    /**
     * Get the block type represented by this block data.
     *
     * @return the block type
     */
    [[nodiscard]] virtual const BlockType &getType() const = 0;

    /**
     * Get the translation key for this block.
     *
     * @return the translation key for this block
     */
    [[nodiscard]] virtual std::string getTranslationKey() const = 0;

    /**
     * Gets the block states, which when passed into a method such as Server::createBlockData(type, block_states)
     * will unambiguously recreate this instance.
     *
     * @return the block states for this block
     */
    [[nodiscard]] virtual BlockStates getBlockStates() const = 0;

    /**
     * Get the runtime ID for this block.
     *
     * @return the runtime id for this block
     */
    [[nodiscard]] virtual std::uint32_t getRuntimeId() const = 0;
};

}  // namespace endstone

template <>
struct std::formatter<endstone::BlockStates::mapped_type> : std::formatter<std::string_view> {
    using Type = endstone::BlockStates::mapped_type;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return std::visit(endstone::overloaded{
                              [&ctx](const std::string &arg) {
                                  return std::format_to(ctx.out(), "{}", endstone::detail::quoted(arg));
                              },
                              [&ctx](auto &&arg) { return std::format_to(ctx.out(), "{}", arg); },
                          },
                          val);
    }
};

template <>
struct std::formatter<endstone::BlockStates::value_type> : std::formatter<std::string_view> {
    using Type = endstone::BlockStates::value_type;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return std::format_to(ctx.out(), "{}={}", endstone::detail::quoted(val.first), val.second);
    }
};

template <>
struct std::formatter<endstone::BlockStates> : std::formatter<std::string_view> {
    using Type = endstone::BlockStates;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return std::format_to(ctx.out(), "[{}]", endstone::detail::join(val.begin(), val.end(), ","));
    }
};

template <>
struct std::formatter<endstone::BlockData> : std::formatter<std::string_view> {
    using Type = endstone::BlockData;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return std::format_to(ctx.out(), "BlockData(type={}, block_states={})", val.getType(), val.getBlockStates());
    }
};
