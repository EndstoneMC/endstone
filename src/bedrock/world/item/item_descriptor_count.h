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

#include <cstdint>

#include "bedrock/world/item/item_descriptor.h"

class ItemDescriptorCount : public ItemDescriptor {
public:
    ItemDescriptorCount() = default;
    explicit ItemDescriptorCount(const ItemDescriptor &item_descriptor, uint16_t count)
        : ItemDescriptor(item_descriptor), stack_size_(count)
    {
    }

    [[nodiscard]] std::uint16_t getStackSize() const
    {
        return stack_size_;
    }

    void setStackSize(std::uint16_t stack_size)
    {
        stack_size_ = stack_size;
    }

    explicit operator bool() const
    {
        if (getItem() == nullptr) {
            return false;
        }
        return getItem() && !isNull() && stack_size_ > 0;
    }

private:
    std::uint16_t stack_size_;
};
