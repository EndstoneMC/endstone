

# File item\_meta.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**inventory**](dir_d1e84b530b14f41e8b6f5ec1b5dee76c.md) **>** [**meta**](dir_2d728641c8c30e7cdff7ab60efc98406.md) **>** [**item\_meta.h**](item__meta_8h.md)

[Go to the documentation of this file](item__meta_8h.md)


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

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace endstone {
class ItemMeta : public std::enable_shared_from_this<ItemMeta> {
public:
    enum class Type {
        Item = 0,
        Map = 1,
        Count = 2,
        None = Item,
    };

    ItemMeta() = default;
    explicit ItemMeta(const ItemMeta *meta)
    {
        if (meta == nullptr) {
            return;
        }
        *this = *meta;
    }

    virtual ~ItemMeta() = default;

    [[nodiscard]] virtual Type getType() const
    {
        return Type::Item;
    }

    [[nodiscard]] virtual bool isEmpty() const
    {
        // TODO(item): more checks here
        return !hasLore();
    }

    [[nodiscard]] virtual std::shared_ptr<ItemMeta> clone() const
    {
        return std::make_shared<ItemMeta>(*this);
    }

    [[nodiscard]] virtual bool hasLore() const
    {
        return lore_.has_value() && !lore_.value().empty();
    }

    [[nodiscard]] virtual std::optional<std::vector<std::string>> getLore() const
    {
        if (!hasLore()) {
            return std::nullopt;
        }
        return lore_;
    }

    virtual void setLore(std::optional<std::vector<std::string>> lore)
    {
        if (!lore.has_value() || lore.value().empty()) {
            lore_ = std::nullopt;
        }
        else {
            lore_ = std::move(lore);
        }
    }

private:
    std::optional<std::vector<std::string>> lore_;
};
}  // namespace endstone
```


