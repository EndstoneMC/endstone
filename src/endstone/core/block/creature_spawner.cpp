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

#include "endstone/core/block/creature_spawner.h"

#include "endstone/check.h"
#include "endstone/core/server.h"

namespace endstone::core {

EndstoneCreatureSpawner::EndstoneCreatureSpawner(const EndstoneBlock &block, ::MobSpawnerBlockActor &spawner)
    : EndstoneBlockStateBase<CreatureSpawner>(block, spawner)
{
}

const ActorType &EndstoneCreatureSpawner::getSpawnedType() const
{
    const auto &id = getSpawner().getSpawner().getEntityId();
    return EndstoneServer::getInstance().getRegistry<ActorType>().getOrThrow(id.getCanonicalName());
}

void EndstoneCreatureSpawner::setSpawnedType(ActorTypeId type)
{
    Preconditions::checkArgument(ActorType::get(type) != nullptr, "Unknown actor type: {}", type);
    getSpawner().getSpawner().setEntityId(ActorDefinitionIdentifier(std::string(type)));
    getSpawner().setChanged();
}

int EndstoneCreatureSpawner::getDelay() const
{
    return getSpawner().getSpawner().getSpawnDelay();
}

void EndstoneCreatureSpawner::setDelay(int delay)
{
    getSpawner().getSpawner().setSpawnDelay(delay);
    getSpawner().setChanged();
}

int EndstoneCreatureSpawner::getMinSpawnDelay() const
{
    return getSpawner().getSpawner().getMinSpawnDelay();
}

void EndstoneCreatureSpawner::setMinSpawnDelay(int delay)
{
    getSpawner().getSpawner().setMinSpawnDelay(delay);
    getSpawner().setChanged();
}

int EndstoneCreatureSpawner::getMaxSpawnDelay() const
{
    return getSpawner().getSpawner().getMaxSpawnDelay();
}

void EndstoneCreatureSpawner::setMaxSpawnDelay(int delay)
{
    getSpawner().getSpawner().setMaxSpawnDelay(delay);
    getSpawner().setChanged();
}

int EndstoneCreatureSpawner::getSpawnCount() const
{
    return getSpawner().getSpawner().getSpawnCount();
}

void EndstoneCreatureSpawner::setSpawnCount(int count)
{
    getSpawner().getSpawner().setSpawnCount(count);
    getSpawner().setChanged();
}

int EndstoneCreatureSpawner::getMaxNearbyEntities() const
{
    return getSpawner().getSpawner().getMaxNearbyEntities();
}

void EndstoneCreatureSpawner::setMaxNearbyEntities(int count)
{
    getSpawner().getSpawner().setMaxNearbyEntities(count);
    getSpawner().setChanged();
}

int EndstoneCreatureSpawner::getRequiredPlayerRange() const
{
    return getSpawner().getSpawner().getRequiredPlayerRange();
}

void EndstoneCreatureSpawner::setRequiredPlayerRange(int range)
{
    getSpawner().getSpawner().setRequiredPlayerRange(range);
    getSpawner().setChanged();
}

int EndstoneCreatureSpawner::getSpawnRange() const
{
    return getSpawner().getSpawner().getSpawnRange();
}

void EndstoneCreatureSpawner::setSpawnRange(int range)
{
    getSpawner().getSpawner().setSpawnRange(range);
    getSpawner().setChanged();
}

}  // namespace endstone::core
