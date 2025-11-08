

# File namespaced\_key.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**namespaced\_key.h**](namespaced__key_8h.md)

[Go to the documentation of this file](namespaced__key_8h.md)


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

namespace endstone {
class NamespacedKey {
public:
    inline static const std::string Minecraft = "minecraft";

    [[nodiscard]] static std::string minecraft(const std::string &key)
    {
        if (key.starts_with(Minecraft) && key.find(':') == Minecraft.size()) {
            return key;
        }
        return Minecraft + ":" + key;
    }

    [[nodiscard]] static std::string getNamespace(const std::string &key)
    {
        const auto pos = key.rfind(':');
        if (pos == std::string::npos) {
            return Minecraft;
        }
        return key.substr(0, pos);
    }

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
```


