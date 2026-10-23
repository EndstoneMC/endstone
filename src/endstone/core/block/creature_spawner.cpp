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

#include <string>

#include "bedrock/nbt/compound_tag.h"
#include "bedrock/world/item/save_context.h"
#include "endstone/check.h"
#include "endstone/core/server.h"

namespace endstone::core {

EndstoneCreatureSpawner::EndstoneCreatureSpawner(const EndstoneBlock &block, ::MobSpawnerBlockActor &spawner,
                                                 bool use_snapshot)
    : EndstoneBlockStateBase<CreatureSpawner>(block, spawner, use_snapshot)
{
}

bool EndstoneCreatureSpawner::update(bool force, bool apply_physics)
{
    if (!EndstoneBlockStateBase::update(force, apply_physics)) {
        return false;
    }
    auto *live = tryGetLiveBlockActor();
    if (isSnapshot() && live != nullptr && getEntityIdentifier(getSpawner()).empty()) {
        static_cast<::MobSpawnerBlockActor &>(*live).getSpawner().setEntityId(ActorDefinitionIdentifier());
    }
    return true;
}

const ActorType *EndstoneCreatureSpawner::getSpawnedType() const
{
    return EndstoneServer::getInstance().getRegistry<ActorType>().get(getEntityIdentifier(getSpawner()));
}

void EndstoneCreatureSpawner::setSpawnedType(std::optional<ActorTypeId> type)
{
    if (type.has_value()) {
        Preconditions::checkArgument(ActorType::get(*type) != nullptr, "Unknown actor type: {}", *type);
    }
    auto &spawner = getSpawner();
    ::CompoundTag tag;
    Preconditions::checkState(spawner.save(tag, SaveContext::forClone()), "Unable to save the spawner.");
    tag.remove("SpawnPotentials");
    if (type.has_value()) {
        tag.putString("EntityIdentifier", std::string(*type));
        if (auto *spawn_data = tag.getCompound("SpawnData"); spawn_data != nullptr) {
            spawn_data->putString("TypeId", std::string(*type));
        }
    }
    else {
        tag.remove("SpawnData");
    }
    load(getBlockSource().getILevel(), spawner, tag);
    if (!type.has_value()) {
        spawner.getSpawner().setEntityId(ActorDefinitionIdentifier());
    }
    spawner.setChanged();
}

std::string EndstoneCreatureSpawner::getEntityIdentifier(const ::MobSpawnerBlockActor &spawner)
{
    ::CompoundTag tag;
    if (!spawner.save(tag, SaveContext::forClone())) {
        return {};
    }
    return tag.getString("EntityIdentifier");
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
