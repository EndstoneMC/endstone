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

namespace endstone {
class NamespacedKey {
public:
    /**
     * @brief The namespace representing all inbuilt keys.
     */
    inline static const std::string Minecraft = "minecraft";

    /**
     * @brief Get a key in the Minecraft namespace.
     *
     * @param key the key to use
     * @return new key in the Minecraft namespace
     */
    [[nodiscard]] static std::string minecraft(const std::string &key)
    {
        if (key.starts_with(Minecraft) && key.find(':') == Minecraft.size()) {
            return key;
        }
        return Minecraft + ":" + key;
    }

    /**
     * @brief Get the namespace (the part before the last colon).
     *
     * Examples:
     * - "minecraft:stone" -> "minecraft"
     * - "custom:block" -> "custom"
     * - "minecraft:potion_type:water" -> "minecraft:potion_type"
     * - "stone" -> "minecraft" (default)
     *
     * @param key the namespaced key string
     * @return substring before the last colon, or "minecraft" if none
     */
    [[nodiscard]] static std::string getNamespace(const std::string &key)
    {
        const auto pos = key.rfind(':');
        if (pos == std::string::npos) {
            return Minecraft;
        }
        return key.substr(0, pos);
    }

    /**
     * @brief Get the key name (the part after the last colon).
     *
     * Examples:
     * - "minecraft:stone" -> "stone"
     * - "custom:block" -> "block"
     * - "minecraft:potion_type:water" -> "water"
     * - "stone" -> "stone"
     *
     * @param key the namespaced key string
     * @return substring after the last colon, or the whole string if no colon
     */
    [[nodiscard]] static std::string getKey(const std::string &key)
    {
        const auto pos = key.rfind(':');
        if (pos == std::string::npos) {
            return key;
        }
        return key.substr(pos + 1);
    }
};
}  // namespace endstone
