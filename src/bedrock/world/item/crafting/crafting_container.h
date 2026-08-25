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

#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/world/container.h"
#include "bedrock/world/item/item_stack.h"

class CraftingContainer : public Container {
public:
    [[nodiscard]] const ItemStack &getItem(int slot) const override = 0;

    [[nodiscard]] int getWidth() const { return width_; }

private:
    std::vector<ItemStack> items_;  // +408
    int width_;                     // +432
};
BEDROCK_STATIC_ASSERT_SIZE(CraftingContainer, 440, 440);
