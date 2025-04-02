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
/**
 * @brief Do not use this API in plugins.
 */
class ItemType {
public:
    virtual ~ItemType() = default;

    /**
     * @brief Gets the maximum amount of this item type that can be held in a stack
     *
     * @return Maximum stack size for this item type
     */
    virtual int getMaxStackSize() const = 0;

    /**
     * @brief Gets the maximum durability of this item type
     *
     * @return Maximum durability for this item type
     */
    virtual int getMaxDurability() const = 0;
};

}  // namespace endstone
