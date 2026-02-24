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

#include <functional>
#include <string>
#include <string_view>

#include <fmt/format.h>

namespace endstone {
/**
 * @brief Represents a identifier consisting of two components: a namespace and a key.
 */
template <typename T>
class Identifier {
public:
    /**
     * @brief The namespace representing all inbuilt keys.
     */
    static constexpr std::string_view Minecraft = "minecraft";

    constexpr Identifier(const char *s) noexcept : Identifier(std::string_view{s}) {}

    constexpr Identifier(const std::string &s) noexcept : Identifier(std::string_view{s}) {}

    constexpr Identifier(const std::string_view &identifier) noexcept
    {
        const auto pos = identifier.rfind(':');
        if (pos == std::string::npos) {
            namespace_ = Minecraft;
            key_ = identifier;
        }
        else {
            namespace_ = identifier.substr(0, pos);
            key_ = identifier.substr(pos + 1);
        }
    }

    constexpr Identifier(const std::string_view &namespace_, const std::string_view &key) noexcept
        : namespace_(namespace_), key_(key)
    {
    }

    [[nodiscard]] constexpr std::string_view getNamespace() const noexcept { return namespace_; }

    [[nodiscard]] constexpr std::string_view getKey() const noexcept { return key_; }

    constexpr bool operator==(const Identifier &other) const noexcept
    {
        return namespace_ == other.namespace_ && key_ == other.key_;
    }

    constexpr bool operator!=(const Identifier &other) const noexcept { return !(*this == other); }

    operator std::string() const
    {
        if (key_.empty()) {
            return "";
        }
        return fmt::format("{}:{}", namespace_, key_);
    }

    static constexpr Identifier minecraft(const std::string_view key) noexcept { return {Minecraft, key}; }

private:
    std::string_view namespace_;
    std::string_view key_;
};
}  // namespace endstone

template <typename T>
struct std::hash<endstone::Identifier<T>> {
    std::size_t operator()(const endstone::Identifier<T> &id) const noexcept
    {
        auto seed = std::hash<std::string_view>{}(id.getNamespace());
        seed ^= std::hash<std::string_view>{}(id.getKey()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

template <typename T>
struct fmt::formatter<endstone::Identifier<T>> : formatter<string_view> {
    template <typename FormatContext>
    auto format(const endstone::Identifier<T> &id, FormatContext &ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "{}:{}", id.getNamespace(), id.getKey());
    }
};
