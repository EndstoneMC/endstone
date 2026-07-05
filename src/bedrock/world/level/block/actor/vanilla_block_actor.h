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

#include <string>
#include <vector>

#include "bedrock/world/level/block/actor/block_actor.h"

class BlockSource;
class Container;
class CompoundTag;
class LevelChunk;
class Vec3;

class IVanillaMainBlockActorComponent {
public:
    virtual BlockActorType getBlockActorType() const = 0;
    virtual Container *getContainer() = 0;
    virtual const Container *getContainer() const = 0;
    virtual void eraseLootTable() = 0;
    virtual std::string getName() const = 0;
    virtual std::string getImmersiveReaderText(BlockSource &) = 0;
    virtual bool canRenderCustomName() const = 0;
    virtual bool hasCustomName() const = 0;
    virtual const Bedrock::Safety::RedactableString &getCustomName() const = 0;
    virtual void setCustomName(const Bedrock::Safety::RedactableString &) = 0;
    virtual bool isCustomNameSaved() const = 0;
    virtual Bedrock::Safety::RedactableString getDisplayName() const = 0;
    virtual bool hasFilteredNameTag() const = 0;
    virtual std::string getFilteredNameTag() const = 0;
    virtual void setFilteredNameTag(const std::string &) = 0;
    virtual std::vector<std::string> getUgcStrings(const CompoundTag &) const = 0;
    virtual std::vector<std::string> getFilteredUgcStrings(const CompoundTag &) const = 0;
    virtual void setUgcStrings(CompoundTag &, const std::vector<std::string> &) const = 0;
    virtual void setFilteredUgcStrings(CompoundTag &, const std::vector<std::string> &) const = 0;
    virtual bool validateData(const CompoundTag &) const = 0;
    virtual void fixupOnLoad(LevelChunk &) = 0;
    virtual void setChanged() = 0;
    virtual bool isChanged() const = 0;
    virtual void onChanged(BlockSource &) = 0;

protected:
    virtual void _setCanRenderCustomName(bool) = 0;
    virtual void _setCustomNameSaved(bool) = 0;
};

class IVanillaRenderBlockActorComponent {
public:
    virtual BlockActor &getBlockActor() = 0;
    virtual const BlockActor &getBlockActor() const = 0;
    virtual const BlockPos &getBlockActorPosition() const = 0;
    virtual BlockActorType getBlockActorType() const = 0;
    virtual BlockActorRendererId getRendererId() const = 0;
    virtual bool isPermanentlyRendered() const = 0;
    virtual bool isWithinRenderDistance(const Vec3 &) const = 0;
    virtual bool hasAlphaLayer() const = 0;
    virtual float getShadowRadius(BlockSource &) const = 0;
    virtual bool isInWorld() const = 0;
    virtual IVanillaRenderBlockActorComponent *getCrackEntity(BlockSource &, const BlockPos &) = 0;
    virtual const AABB &getAABB() const = 0;
    virtual void setAABB(const AABB &) = 0;
    virtual ActorTerrainInterlockData &getEntityTerrainInterlockData() = 0;
    virtual const ActorTerrainInterlockData &getEntityTerrainInterlockDataConst() const = 0;

protected:
    virtual void _resetAABB() = 0;
};

class IVanillaTickBlockActorComponent {
public:
    virtual BlockActor &getBlockActor() = 0;
    virtual const BlockActor &getBlockActor() const = 0;
    virtual const BlockPos &getBlockActorPosition() const = 0;
    virtual BlockActorType getBlockActorType() const = 0;
    virtual void tick(BlockSource &) = 0;
    virtual int getTickCount() const = 0;
};

class VanillaBlockActor : public BlockActor,
                          public IVanillaMainBlockActorComponent,
                          public IVanillaRenderBlockActorComponent,
                          public IVanillaTickBlockActorComponent {
public:
    virtual void tick(BlockSource &) = 0;

    void setChanged() { properties_.insert(BlockActor::Property::Changed); }
    [[nodiscard]] bool isChanged() const { return properties_.contains(BlockActor::Property::Changed); }

protected:
    int tick_count_;                                    // +56
    BlockActor::Properties properties_;                 // +60
    AABB bb_;                                           // +64
    BlockActorRendererId renderer_id_;                  // +88
    ActorTerrainInterlockData terrain_interlock_data_;  // +96
    Bedrock::Safety::RedactableString custom_name_;     // +120
    std::string filtered_custom_name_;                  // +176
};
