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

#include "endstone/inventory/item_stack.h"
#include "endstone/object.h"

namespace endstone {
/**
 * Represents a potential item match within a recipe. All choices within a recipe must be satisfied for it to be
 * craftable.
 */
class RecipeIngredient : public Object {
public:
    ~RecipeIngredient() override = default;

    [[nodiscard]] virtual bool test(const ItemStack &item) const = 0;

    /**
     * Get how many items this ingredient consumes.
     *
     * Bedrock records a count on each ingredient where Java repeats the ingredient instead.
     *
     * @return the number of items consumed
     */
    [[nodiscard]] virtual int getCount() const = 0;
};
}  // namespace endstone
