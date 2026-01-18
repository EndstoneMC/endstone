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

#include "bedrock/world/actor/mob.h"
#include "endstone/actor/mob.h"
#include "endstone/core/actor/actor.h"

namespace endstone::core {
template <typename Interface, typename Handle>
    requires std::is_base_of_v<Mob, Interface> && std::is_base_of_v<::Mob, Handle>
class EndstoneMobBase : public EndstoneActorBase<Interface, Handle> {
    using Base = EndstoneActorBase<Interface, Handle>;

public:
    using Base::Base;
    [[nodiscard]] Mob *asMob() const override
    {
        return const_cast<EndstoneMobBase *>(this);
    }

    void setRotation(float yaw, float pitch) override
    {
        Base::setRotation(yaw, pitch);
        Base::getHandle().setYBodyRotation(yaw);
    }

    bool teleport(const Location &location) override
    {
        if (getHealth() == 0) {
            return false;
        }
        return Base::teleport(location);
    }

    [[nodiscard]] bool isGliding() const override
    {
        return Base::getHandle().isGliding();
    }

    [[nodiscard]] int getHealth() const override
    {
        return Base::getHandle().getHealth();
    }

    void setHealth(int health) const override
    {
        Preconditions::checkArgument(health >= 0 && health <= getMaxHealth(),  //
                                     "Health value ({}) must be between 0 and {}.", health, getMaxHealth());
        auto mutable_attr = Base::getHandle().getMutableAttribute("minecraft:health");
        mutable_attr->setCurrentValue(static_cast<float>(health));
    }

    [[nodiscard]] int getMaxHealth() const override
    {
        return Base::getHandle().getMaxHealth();
    }

    void setMaxHealth(int health) const override
    {
        Preconditions::checkArgument(health > 0, "Max health amount ({}) must be greater than 0", health);
        auto mutable_attr = Base::getHandle().getMutableAttribute("minecraft:health");
        mutable_attr->setMaxValue(static_cast<float>(health));
    }
};

class EndstoneMob : public EndstoneMobBase<Mob, ::Mob> {
public:
    using EndstoneMobBase::EndstoneMobBase;
};
}  // namespace endstone::core
