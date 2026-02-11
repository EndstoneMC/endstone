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

#include "bedrock/world/actor/item/item_actor.h"
#include "endstone/actor/item.h"
#include "endstone/core/actor/actor.h"

namespace endstone::core {
class EndstoneItem : public EndstoneActorBase<Item, ::ItemActor> {
public:
    using EndstoneActorBase::EndstoneActorBase;
    [[nodiscard]] Item *asItem() const override;
    [[nodiscard]] ItemStack getItemStack() const override;
    void setItemStack(const ItemStack &stack) override;
    [[nodiscard]] int getPickupDelay() const override;
    void setPickupDelay(int delay) override;
    void setUnlimitedLifetime(bool unlimited) override;
    [[nodiscard]] bool isUnlimitedLifetime() const override;
    void setThrower(std::optional<std::int64_t> thrower) override;
    [[nodiscard]] std::optional<std::int64_t> getThrower() const override;
};
}  // namespace endstone::core
