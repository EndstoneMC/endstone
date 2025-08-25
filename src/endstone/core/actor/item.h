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
class EndstoneItem : public EndstoneActor, public Item {
public:
    explicit EndstoneItem(EndstoneServer &server, ::ItemActor &item);

    [[nodiscard]] Item *asItem() const override;
    [[nodiscard]] std::unique_ptr<ItemStack> getItemStack() const override;
    void setItemStack(const ItemStack &stack) override;
    [[nodiscard]] int getPickupDelay() const override;
    void setPickupDelay(int delay) override;
    void setUnlimitedLifetime(bool unlimited) override;
    [[nodiscard]] bool isUnlimitedLifetime() const override;
    void setThrower(std::optional<std::int64_t> thrower) override;
    [[nodiscard]] std::optional<std::int64_t> getThrower() const override;

    [[nodiscard]] ::ItemActor &getItem() const;

    // forward
    ENDSTONE_FORWARD_IMPL_PERMISSIBLE(EndstoneActor);
    ENDSTONE_FORWARD_IMPL_ACTOR(EndstoneActor);
    [[nodiscard]] PermissionLevel getPermissionLevel() const override
    {
        return EndstoneActor::getPermissionLevel();
    }
    void sendMessage(const Message &message) const override
    {
        EndstoneActor::sendMessage(message);
    }
    void sendErrorMessage(const Message &message) const override
    {
        EndstoneActor::sendErrorMessage(message);
    }
    [[nodiscard]] std::string getName() const override
    {
        return EndstoneActor::getName();
    }
    [[nodiscard]] Mob *asMob() const override
    {
        return EndstoneActor::asMob();
    }
    void setRotation(float yaw, float pitch) override
    {
        EndstoneActor::setRotation(yaw, pitch);
    }
    void remove() override
    {
        EndstoneActor::remove();
    }
};
}  // namespace endstone::core
