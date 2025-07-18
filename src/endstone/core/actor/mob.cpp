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

#include "endstone/core/actor/mob.h"

#include "endstone/core/permissions/permissible.h"
#include "endstone/core/server.h"

namespace endstone::core {

EndstoneMob::EndstoneMob(EndstoneServer &server, ::Mob &mob) : EndstoneActor(server, mob) {}

Mob *EndstoneMob::asMob() const
{
    return const_cast<EndstoneMob *>(this);
}

void EndstoneMob::setRotation(float yaw, float pitch)
{
    EndstoneActor::setRotation(yaw, pitch);
    getMob().setYBodyRotation(yaw);
}

bool EndstoneMob::isGliding() const
{
    return getMob().isGliding();
}

::Mob &EndstoneMob::getMob() const
{
    return getHandle<::Mob>();
}

std::shared_ptr<EndstoneMob> EndstoneMob::create(EndstoneServer &server, ::Mob &mob)
{
    return PermissibleFactory::create<EndstoneMob>(server, mob);
}

}  // namespace endstone::core
