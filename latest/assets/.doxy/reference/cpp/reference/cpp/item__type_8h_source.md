

# File item\_type.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**inventory**](dir_d1e84b530b14f41e8b6f5ec1b5dee76c.md) **>** [**item\_type.h**](item__type_8h.md)

[Go to the documentation of this file](item__type_8h.md)


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

#include "endstone/detail/endstone.h"
#include "endstone/namespaced_key.h"
#include "endstone/registry.h"

namespace endstone {
class ItemType {
public:
    virtual ~ItemType() = default;
    [[nodiscard]] virtual std::string_view getId() const = 0;

    [[nodiscard]] virtual NamespacedKey getKey() const = 0;

    [[nodiscard]] virtual std::string getTranslationKey() const = 0;

    [[nodiscard]] virtual int getMaxStackSize() const = 0;

    [[nodiscard]] virtual int getMaxDurability() const = 0;

    static const ItemType *get(std::string_view name)
    {
        auto key = NamespacedKey::fromString(name);
        if (!key) {
            return nullptr;
        }
        return Endstone::getServer().getItemRegistry().get(key.value());
    }

    bool operator==(const std::string_view other) const
    {
        return getId() == other;
    }

    bool operator!=(const std::string_view other) const
    {
        return !(*this == other);
    }

    bool operator==(const ItemType &other) const
    {
        return getId() == other.getId();
    }

    bool operator!=(const ItemType &other) const
    {
        return !(*this == other);
    }
};
}  // namespace endstone

template <>
struct fmt::formatter<endstone::ItemType> : formatter<string_view> {
    using Type = endstone::ItemType;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "{}", val.getId());
    }
};  // namespace fmt
```


