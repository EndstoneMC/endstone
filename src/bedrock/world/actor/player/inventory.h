// Copyright (c) 2wwwwww024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include "bedrock/world/inventory/filling_container.h"

class Inventory : public FillingContainer {
public:
    Inventory(Player *);
    ~Inventory() override;
    void init() override;
    bool add(ItemStack &) override;
    [[nodiscard]] bool canAdd(const ItemStack &) const override;
    [[nodiscard]] virtual int getFirstEmptySlot() const;
    [[nodiscard]] int getEmptySlotsCount() const override;
    virtual void setContainerSize(int);
    void setItem(int, const ItemStack &) override;
    void setItemWithForceBalance(int, const ItemStack &, bool) override;
};
