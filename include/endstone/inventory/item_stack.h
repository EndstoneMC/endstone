#include <utility>

#include <fmt/format.h>

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

namespace endstone {

/**
 * @brief Represents a stack of items.
 */
class ItemStack : public std::enable_shared_from_this<ItemStack> {
public:
    ItemStack() = default;
    explicit ItemStack(std::string type, int amount = 1) : type_(std::move(type)), amount_(amount) {}

    virtual ~ItemStack() = default;

    /**
     * @brief Gets the type of this item
     *
     * @return Type of the items in this stack
     */
    [[nodiscard]] virtual std::string getType() const
    {
        return type_;
    }

    /**
     * @brief Sets the type of this item
     *
     * @param type New type to set the items in this stack to
     */
    virtual void setType(std::string type)
    {
        // TODO(item): clear item components when the type is changed
        type_ = std::move(type);
    }

    /**
     * @brief Gets the amount of items in this stack
     *
     * @return Amount of items in this stack
     */
    [[nodiscard]] virtual int getAmount() const
    {
        return amount_;
    }

    /**
     * @brief Sets the amount of items in this stack
     *
     * @param amount New amount of items in this stack
     */
    virtual void setAmount(int amount)
    {
        amount_ = amount;
    }

private:
    std::string type_ = "minecraft:air";
    int amount_ = 0;
};

}  // namespace endstone

namespace fmt {
template <>
struct formatter<endstone::ItemStack> : formatter<string_view> {
    using Type = endstone::ItemStack;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return format_to(ctx.out(), "ItemStack({} x {})", val.getType(), val.getAmount());
    }
};
}  // namespace fmt
