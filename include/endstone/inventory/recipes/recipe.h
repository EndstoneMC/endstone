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
namespace endstone {
class Recipe {
public:
    explicit Recipe(std::string recipe_id) : recipe_id_(std::move(recipe_id)) {}
    virtual ~Recipe() = default;
    virtual std::vector<std::shared_ptr<ItemStack>> &getResult() = 0;
    std::string &getId()
    {
        return recipe_id_;
    }
    virtual bool isShaped() = 0;
    virtual bool isShapeless() = 0;
    virtual bool isFurnace() = 0;

private:
    std::string recipe_id_;
};
}  // namespace endstone
