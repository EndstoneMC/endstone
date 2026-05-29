

# File item\_stack.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**inventory**](dir_d1e84b530b14f41e8b6f5ec1b5dee76c.md) **>** [**item\_stack.h**](item__stack_8h.md)

[Go to the documentation of this file](item__stack_8h.md)


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

#include "endstone/detail/endstone.h"
#include "endstone/inventory/item_factory.h"
#include "endstone/inventory/meta/item_meta.h"

namespace endstone {

namespace core {
class EndstoneItemStack;
}

class ItemStack {
public:
    ItemStack() = default;
    explicit ItemStack(std::string type, const int amount = 1) : type_(std::move(type)), amount_(amount) {}

    virtual ~ItemStack() = default;

protected:
    friend class core::EndstoneItemStack;
    virtual bool isEndstoneItemStack() const
    {
        return false;
    }

public:
    [[nodiscard]] virtual std::string getType() const
    {
        return type_;
    }

    virtual void setType(std::string type)
    {
        // TODO(item): clear item components when the type is changed
        type_ = std::move(type);
    }

    [[nodiscard]] virtual int getAmount() const
    {
        return amount_;
    }

    virtual void setAmount(int amount)
    {
        amount_ = amount;
    }

    virtual std::unique_ptr<ItemMeta> getItemMeta() const
    {
        return meta_ == nullptr ? Endstone::getServer().getItemFactory().getItemMeta(type_) : meta_->clone();
    }

    virtual bool hasItemMeta() const
    {
        return !Endstone::getServer().getItemFactory().equals(meta_.get(), nullptr);
    }

    virtual bool setItemMeta(ItemMeta *meta)
    {
        return setItemMeta0(meta, type_);
    }

private:
    bool setItemMeta0(ItemMeta *meta, const std::string_view type)
    {
        if (!meta) {
            meta_ = nullptr;
            return true;
        }

        const auto &item_factory = Endstone::getServer().getItemFactory();
        if (!item_factory.isApplicable(meta, type)) {
            return false;
        }

        meta_ = item_factory.asMetaFor(meta, type);
        return true;
    }

    std::string type_ = "minecraft:air";
    int amount_ = 0;
    std::unique_ptr<ItemMeta> meta_ = nullptr;
};

}  // namespace endstone

namespace fmt {
template <>
struct formatter<endstone::ItemStack> : formatter<string_view> {
    using Type = endstone::ItemStack;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "ItemStack({} x {})", val.getType(), val.getAmount());
    }
};
}  // namespace fmt
```


