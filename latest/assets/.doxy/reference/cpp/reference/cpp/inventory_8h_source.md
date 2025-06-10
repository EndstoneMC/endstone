

# File inventory.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**inventory**](dir_d1e84b530b14f41e8b6f5ec1b5dee76c.md) **>** [**inventory.h**](inventory_8h.md)

[Go to the documentation of this file](inventory_8h.md)


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
#include <vector>

#include "endstone/util/result.h"

namespace endstone {
class ItemStack;
class ItemType;
class Inventory {
public:
    virtual ~Inventory() = default;
    [[nodiscard]] virtual int getSize() const = 0;

    [[nodiscard]] virtual int getMaxStackSize() const = 0;

    [[nodiscard]] virtual std::unique_ptr<ItemStack> getItem(int index) const = 0;

    virtual void setItem(int index, const ItemStack *item) = 0;

    virtual std::unordered_map<int, ItemStack *> addItem(std::vector<ItemStack *> items) = 0;

    virtual std::unordered_map<int, ItemStack *> removeItem(std::vector<ItemStack *> items) = 0;

    template <typename... Args, typename = std::enable_if_t<(std::is_convertible_v<Args, ItemStack &> && ...)>>
    std::unordered_map<int, ItemStack *> addItem(Args &&...items)
    {
        return addItem(std::vector<ItemStack *>{&items...});
    }

    template <typename... Args, typename = std::enable_if_t<(std::is_convertible_v<Args, ItemStack &> && ...)>>
    std::unordered_map<int, ItemStack *> removeItem(Args &&...items)
    {
        return removeItem(std::vector<ItemStack *>{&items...});
    }

    [[nodiscard]] virtual std::vector<std::unique_ptr<ItemStack>> getContents() const = 0;

    virtual Result<void> setContents(std::vector<const ItemStack *> items) = 0;

    [[nodiscard]] virtual Result<bool> contains(const std::string &type) const = 0;

    [[nodiscard]] virtual bool contains(const ItemStack &item) const = 0;

    [[nodiscard]] virtual bool contains(const ItemStack &item, int amount) const = 0;

    [[nodiscard]] virtual Result<bool> containsAtLeast(const std::string &type, int amount) const = 0;

    [[nodiscard]] virtual bool containsAtLeast(const ItemStack &item, int amount) const = 0;

    [[nodiscard]] virtual Result<std::unordered_map<int, std::unique_ptr<ItemStack>>> all(
        const std::string &type) const = 0;

    [[nodiscard]] virtual std::unordered_map<int, std::unique_ptr<ItemStack>> all(const ItemStack &item) const = 0;

    [[nodiscard]] virtual Result<int> first(const std::string &type) const = 0;

    [[nodiscard]] virtual int first(const ItemStack &item) const = 0;

    [[nodiscard]] virtual int firstEmpty() const = 0;

    [[nodiscard]] virtual bool isEmpty() const = 0;

    virtual Result<void> remove(const std::string &type) = 0;

    virtual void remove(const ItemStack &item) = 0;

    virtual void clear(int index) = 0;

    virtual void clear() = 0;
};
}  // namespace endstone
```


