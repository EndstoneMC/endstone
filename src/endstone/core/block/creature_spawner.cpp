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

EndstoneCreatureSpawner::EndstoneCreatureSpawner(const EndstoneBlock &block, ::MobSpawnerBlockActor &spawner,
                                                 bool use_snapshot)
    : EndstoneBlockStateBase<CreatureSpawner>(block, spawner, use_snapshot),
      spawner_(static_cast<::MobSpawnerBlockActor &>(*getBlockActor()))
{
}

const ActorType &EndstoneCreatureSpawner::getSpawnedType() const
{
    const auto &id = spawner_.getSpawner().getEntityId();
    return EndstoneServer::getInstance().getRegistry<ActorType>().getOrThrow(id.getFullName());
}

void EndstoneCreatureSpawner::setSpawnedType(ActorTypeId type)
{
    Preconditions::checkArgument(ActorType::get(type) != nullptr, "Unknown actor type: {}", type);
    spawner_.getSpawner().setEntityId(ActorDefinitionIdentifier(std::string(type)));
    spawner_.setChanged();
}

int EndstoneCreatureSpawner::getDelay() const
{
    return spawner_.getSpawner().getSpawnDelay();
}

void EndstoneCreatureSpawner::setDelay(int delay)
{
    spawner_.getSpawner().setSpawnDelay(delay);
    spawner_.setChanged();
}

int EndstoneCreatureSpawner::getMinSpawnDelay() const
{
    return spawner_.getSpawner().getMinSpawnDelay();
}

void EndstoneCreatureSpawner::setMinSpawnDelay(int delay)
{
    spawner_.getSpawner().setMinSpawnDelay(delay);
    spawner_.setChanged();
}

int EndstoneCreatureSpawner::getMaxSpawnDelay() const
{
    return spawner_.getSpawner().getMaxSpawnDelay();
}

void EndstoneCreatureSpawner::setMaxSpawnDelay(int delay)
{
    spawner_.getSpawner().setMaxSpawnDelay(delay);
    spawner_.setChanged();
}

int EndstoneCreatureSpawner::getSpawnCount() const
{
    return spawner_.getSpawner().getSpawnCount();
}

void EndstoneCreatureSpawner::setSpawnCount(int count)
{
    spawner_.getSpawner().setSpawnCount(count);
    spawner_.setChanged();
}

int EndstoneCreatureSpawner::getMaxNearbyEntities() const
{
    return spawner_.getSpawner().getMaxNearbyEntities();
}

void EndstoneCreatureSpawner::setMaxNearbyEntities(int count)
{
    spawner_.getSpawner().setMaxNearbyEntities(count);
    spawner_.setChanged();
}

int EndstoneCreatureSpawner::getRequiredPlayerRange() const
{
    return spawner_.getSpawner().getRequiredPlayerRange();
}

void EndstoneCreatureSpawner::setRequiredPlayerRange(int range)
{
    spawner_.getSpawner().setRequiredPlayerRange(range);
    spawner_.setChanged();
}

int EndstoneCreatureSpawner::getSpawnRange() const
{
    return spawner_.getSpawner().getSpawnRange();
}

void EndstoneCreatureSpawner::setSpawnRange(int range)
{
    spawner_.getSpawner().setSpawnRange(range);
    spawner_.setChanged();
}

}  // namespace endstone::core
