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
};
}  // namespace endstone
