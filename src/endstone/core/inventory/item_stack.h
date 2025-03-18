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

#include "bedrock/world/item/item_stack.h"
#include "endstone/inventory/item_stack.h"

namespace endstone::core {

class EndstoneItemStack : public ItemStack {
public:
    explicit EndstoneItemStack(const ::ItemStack &item);

protected:
    const EndstoneItemStack *asEndstoneItemStack() const override;
    EndstoneItemStack *asEndstoneItemStack() override;

public:
    [[nodiscard]] std::string getType() const override;
    void setType(std::string type) override;
    [[nodiscard]] int getAmount() const override;
    void setAmount(int amount) override;
    std::shared_ptr<ItemMeta> getItemMeta() const override;

    static ::ItemStack toMinecraft(const std::shared_ptr<ItemStack> &item);
    static std::shared_ptr<EndstoneItemStack> fromMinecraft(const ::ItemStack &item);

private:
    void reset();

    ::ItemStack *handle_;
    std::unique_ptr<::ItemStack> owned_handle_;
};

}  // namespace endstone::core
