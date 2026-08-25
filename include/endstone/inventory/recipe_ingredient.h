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
#include <utility>

#include "endstone/inventory/item_stack.h"
#include "endstone/inventory/item_type.h"
#include "endstone/object.h"

namespace endstone {

namespace core {
class EndstoneIngredient;
}

/**
 * Represents a potential item match within a recipe. All choices within a recipe must be satisfied for it to be
 * craftable.
 */
class RecipeIngredient : public Object {
public:
    RecipeIngredient(const RecipeIngredient &other) : impl_(other.impl_->clone()) {}
    RecipeIngredient(RecipeIngredient &&other) noexcept = default;
    RecipeIngredient &operator=(const RecipeIngredient &other)
    {
        if (this != &other) {
            impl_ = other.impl_->clone();
        }
        return *this;
    }
    RecipeIngredient &operator=(RecipeIngredient &&other) noexcept = default;
    ~RecipeIngredient() override = default;

    [[nodiscard]] ClassInfo getClassInfo() const override { return impl_->getClassInfo(); }
    [[nodiscard]] bool isInstanceOf(ClassInfo target) const override { return impl_->isInstanceOf(target); }

    [[nodiscard]] bool test(const ItemStack &item) const { return impl_->test(item); }

    /**
     * Get how many items this ingredient consumes.
     *
     * Bedrock records a count on each ingredient where Java repeats the ingredient instead.
     *
     * @return the number of items consumed
     */
    [[nodiscard]] int getCount() const { return impl_->getCount(); }

    template <typename T>
        requires std::is_base_of_v<RecipeIngredient, T>
    [[nodiscard]] std::optional<T> as() const
    {
        if (!is<T>()) {
            return std::nullopt;
        }
        return T(impl_->clone());
    }

protected:
    class Impl {
    public:
        virtual ~Impl() = default;
        [[nodiscard]] virtual std::unique_ptr<Impl> clone() const = 0;
        [[nodiscard]] virtual ClassInfo getClassInfo() const = 0;
        [[nodiscard]] virtual bool isInstanceOf(ClassInfo target) const = 0;
        [[nodiscard]] virtual bool test(const ItemStack &item) const = 0;
        [[nodiscard]] virtual int getCount() const = 0;
        [[nodiscard]] virtual ItemStack getItemStack() const { return ItemType::get(ItemType::Air)->createItemStack(0); }
        [[nodiscard]] virtual const ItemType &getItemType() const { return *ItemType::get(ItemType::Air); }
        [[nodiscard]] virtual const std::string &getItemTag() const
        {
            static const std::string empty;
            return empty;
        }
        [[nodiscard]] virtual const std::string &getExpression() const
        {
            static const std::string empty;
            return empty;
        }
        [[nodiscard]] virtual const std::string &getAlias() const
        {
            static const std::string empty;
            return empty;
        }
    };

    template <typename Interface>
    class TypedImpl : public Impl {
    public:
        [[nodiscard]] ClassInfo getClassInfo() const override { return typeid(Interface); }
        [[nodiscard]] bool isInstanceOf(ClassInfo target) const override
        {
            return target == typeid(Interface) || target == typeid(RecipeIngredient);
        }
    };

    explicit RecipeIngredient(std::unique_ptr<Impl> impl) : impl_(std::move(impl)) {}

    friend class core::EndstoneIngredient;
    std::unique_ptr<Impl> impl_;
};

}  // namespace endstone
