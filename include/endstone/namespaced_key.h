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

#include <algorithm>
#include <cctype>
#include <locale>
#include <optional>
#include <string>
#include <string_view>

namespace endstone {

/**
 * @brief Represents a string-based key which consists of two components - a namespace and a key.
 *
 * Namespaces may only contain lowercase alphanumeric characters, periods, underscores, and hyphens.
 *
 * Keys may only contain lowercase alphanumeric characters, periods, underscores, hyphens, and forward slashes.
 */
class NamespacedKey final {
public:
    /**
     * The namespace representing all inbuilt keys.
     */
    static constexpr std::string_view MINECRAFT = "minecraft";

    /**
     * @brief Factory method to create a NamespacedKey. Returns std::nullopt if namespace or key is invalid.
     *
     * @param namespace_ the namespace
     * @param key the key
     * @return optional containing the NamespacedKey, or std::nullopt on failure
     */
    static std::optional<NamespacedKey> create(std::string_view namespace_, std::string_view key)
    {
        std::string ns = normalizeString(namespace_);
        std::string k = normalizeString(key);

        if (!isValidNamespace(ns) || !isValidKey(k)) {
            return std::nullopt;
        }
        return NamespacedKey(std::move(ns), std::move(k));
    }

    /**
     * Parse a NamespacedKey from a string, with an optional default namespace.
     *
     * Examples:
     * - fromString("foo", "plugin")       -> "plugin:foo"
     * - fromString("foo:bar", std::nullopt) -> "foo:bar"
     * - fromString(":foo", "minecraft")   -> "minecraft:foo"
     * - fromString("foo", std::nullopt)     -> "minecraft:foo"
     * - fromString("Foo", ...)              -> std::nullopt
     *
     * @param input the string to parse
     * @param default_ns optional default namespace; if std::nullopt, uses Minecraft
     * @return optional containing the parsed key, or std::nullopt if invalid
     */
    static std::optional<NamespacedKey> fromString(std::string_view input,
                                                   std::optional<std::string_view> default_ns = std::nullopt)
    {
        if (input.empty()) {
            return std::nullopt;
        }
        size_t colon_count = std::count(input.begin(), input.end(), ':');
        if (colon_count > 1) {
            return std::nullopt;
        }

        std::string ns;
        std::string key;
        if (colon_count == 1) {
            auto pos = input.find(':');
            ns = std::string(input.substr(0, pos));
            key = std::string(input.substr(pos + 1));
        }
        else {
            key = std::string(input);
        }

        std::string_view final_ns;
        if (colon_count == 1) {
            final_ns = ns.empty() ? default_ns.value_or(MINECRAFT) : ns;
        }
        else {
            if (!isValidKey(key)) {
                return std::nullopt;
            }
            final_ns = default_ns.value_or(MINECRAFT);
        }

        return create(final_ns, key);
    }

    [[nodiscard]] const std::string &getNamespace() const noexcept
    {
        return namespace_;
    }

    [[nodiscard]] const std::string &getKey() const noexcept
    {
        return key_;
    }

    [[nodiscard]] std::string toString() const noexcept
    {
        return namespace_ + ':' + key_;
    }

    bool operator==(const NamespacedKey &other) const noexcept = default;
    bool operator!=(const NamespacedKey &other) const noexcept = default;

private:
    std::string namespace_;
    std::string key_;

    /**
     * Private constructor; use create() or fromString() instead.
     */
    NamespacedKey(std::string ns, std::string key) : namespace_(std::move(ns)), key_(std::move(key)) {}

    /**
     * Normalize a string view to lowercase, returning a new std::string.
     */
    static std::string normalizeString(std::string_view str)
    {
        std::string s(str);
        std::ranges::transform(s, s.begin(), ::tolower);
        return s;
    }

    static constexpr bool isValidNamespaceChar(char c) noexcept
    {
        return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '.' || c == '_' || c == '-';
    }

    static constexpr bool isValidKeyChar(char c) noexcept
    {
        return isValidNamespaceChar(c) || c == '/';
    }

    static bool isValidNamespace(const std::string &ns) noexcept
    {
        return !ns.empty() && std::ranges::all_of(ns, isValidNamespaceChar);
    }

    static bool isValidKey(const std::string &key) noexcept
    {
        return !key.empty() && std::ranges::all_of(key, isValidKeyChar);
    }
};

}  // namespace endstone

namespace std {
template <>
struct hash<endstone::NamespacedKey> {
    size_t operator()(const endstone::NamespacedKey &key) const noexcept
    {
        const size_t h1 = std::hash<std::string_view>{}(key.getNamespace());
        const size_t h2 = std::hash<std::string_view>{}(key.getKey());
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};
}  // namespace std
