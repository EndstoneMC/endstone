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
public:
    using EndstoneActorBase<Interface, Handle>::EndstoneActorBase;
    [[nodiscard]] Mob *asMob() const override
    {
        return const_cast<EndstoneMobBase *>(this);
    }

    void setRotation(float yaw, float pitch) override
    {
        EndstoneActor::setRotation(yaw, pitch);
        getHandle().setYBodyRotation(yaw);
    }

    [[nodiscard]] bool isGliding() const override
    {
        return getHandle().isGliding();
    }
};

class EndstoneMob : public EndstoneMobBase<Mob, ::Mob> {
public:
    using EndstoneMobBase::EndstoneMobBase;
};
}  // namespace endstone::core
