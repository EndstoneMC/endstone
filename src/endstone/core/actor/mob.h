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

#include "endstone/actor/mob.h"
#include "endstone/core/actor/actor.h"

class Mob;

namespace endstone::core {
class EndstoneMob : public EndstoneActor, public Mob {
public:
    explicit EndstoneMob(EndstoneServer &server, ::Mob &mob);

    [[nodiscard]] Mob *asMob() const override;
    void setRotation(float yaw, float pitch) override;
    [[nodiscard]] bool isGliding() const override;

    ::Mob &getMob() const;

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
    [[nodiscard]] Item *asItem() const override
    {
        return EndstoneActor::asItem();
    }
    void remove() override
    {
        EndstoneActor::remove();
    }
};
}  // namespace endstone::core

#define ENDSTONE_FORWARD_IMPL_MOB(IMPL)               \
    void setRotation(float yaw, float pitch) override \
    {                                                 \
        IMPL::setRotation(yaw, pitch);                \
    }                                                 \
    [[nodiscard]] bool isGliding() const override     \
    {                                                 \
        return IMPL::isGliding();                     \
    }
