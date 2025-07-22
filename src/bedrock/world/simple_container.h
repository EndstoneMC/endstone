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

#include "bedrock/world/container.h"

class SimpleContainer : public Container {
public:
    SimpleContainer(const std::string &, bool, int, ContainerType);
    [[nodiscard]] const ItemStack &getItem(int) const override;
    void setItem(int, const ItemStack &) override;
    [[nodiscard]] int getContainerSize() const override;
    [[nodiscard]] int getMaxStackSize() const override;
    void startOpen(Actor &) override;
    void stopOpen(Actor &) override;
    void serverInitItemStackIds(int, int, ItemStackNetIdChangedCallback) override;

protected:
    int size_;
    std::vector<ItemStack> items_;
};
