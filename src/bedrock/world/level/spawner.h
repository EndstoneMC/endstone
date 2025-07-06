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

#include <functional>
#include <string>
#include <unordered_set>

#include <bedrock/world/actor/item/item_actor.h>
#include <gsl/gsl>

#include "bedrock/forward.h"

class Spawner {
public:
    virtual ~Spawner();
    virtual void initializeServerSide(::ResourcePackManager &, IWorldRegistriesProvider &) = 0;
    [[nodiscard]] virtual SpawnSettings const &getSpawnSettings() const = 0;
    virtual void setSpawnSettings(::SpawnSettings const &) = 0;
    [[nodiscard]] virtual ActorSpawnRuleGroup const *getSpawnRules() const = 0;
    [[nodiscard]] virtual ActorSpawnRuleGroup *getSpawnRulesMutable() const = 0;
    [[nodiscard]] virtual SpawnGroupRegistry const *getSpawnGroupRegistry() const = 0;
    [[nodiscard]] virtual br::spawn::EntityTypeCache *getEntityTypeCache() const = 0;
    virtual Mob *spawnMob(BlockSource &region, ActorDefinitionIdentifier const &id, Actor *spawner, Vec3 const &pos,
                          bool natural_spawn, bool surface, bool from_spawner);
    virtual ItemActor *spawnItem(BlockSource &region, ItemStack const &inst, Actor *spawner, Vec3 const &pos,
                                 int throw_time);
    virtual Actor *spawnProjectile(BlockSource &region, ActorDefinitionIdentifier const &id, Actor *spawner,
                                   Vec3 const &position, Vec3 const &direction);
    virtual void postProcessSpawnMobs(BlockSource &, int, int, Random &) = 0;
    virtual void tick(BlockSource &, LevelChunkVolumeData const &, const ChunkPos) = 0;
    virtual void tickMobCount() = 0;
    virtual void incrementSpawnableTickedMob() = 0;
    [[nodiscard]] virtual int getSpawnableTickedMobCountPrevious() const = 0;
    virtual std::unordered_set<::ActorUniqueID> spawnMobGroup(BlockSource &, std::string const &, Vec3 const &, bool,
                                                              bool, std::function<void(::Mob &)> &&) = 0;
    virtual br::spawn::SpawnPlacements &getSpawnPlacements() = 0;
    [[nodiscard]] virtual std::tuple<std::array<SpawnCategory::Type, 8>, uint64_t> filteredSpawningCategories(
        br::spawn::State const &, bool, bool, bool) const;
    virtual void spawnForChunk(BlockSource &, LevelChunkVolumeData const &, br::spawn::State &,
                               gsl::span<::SpawnCategory::Type>, IRandom &) const;
    virtual void spawnMobsForChunkGeneration(BlockSource &, Biome const &, ChunkPos, IRandom &) const;
};
