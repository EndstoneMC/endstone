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

#include "bedrock/core/container/enum_set.h"
#include "bedrock/world/level/block/actor/block_actor.h"

class BlockSource;
class Container;
class CompoundTag;
class LevelChunk;
class Vec3;

class IVanillaMainBlockActorComponent {
public:
    virtual BlockActor &getBlockActor() = 0;
    [[nodiscard]] virtual BlockActorType getBlockActorType() const = 0;
    virtual Container *getContainer() = 0;
    [[nodiscard]] virtual const Container *getContainer() const = 0;
    virtual void eraseLootTable() = 0;
    [[nodiscard]] virtual std::string getName() const = 0;
    virtual std::string getImmersiveReaderText(BlockSource &) = 0;
    [[nodiscard]] virtual bool canRenderCustomName() const = 0;
    [[nodiscard]] virtual bool hasCustomName() const = 0;
    [[nodiscard]] virtual const Bedrock::Safety::RedactableString &getCustomName() const = 0;
    virtual void setCustomName(const Bedrock::Safety::RedactableString &) = 0;
    [[nodiscard]] virtual bool isCustomNameSaved() const = 0;
    [[nodiscard]] virtual Bedrock::Safety::RedactableString getDisplayName() const = 0;
    [[nodiscard]] virtual bool hasFilteredNameTag() const = 0;
    [[nodiscard]] virtual std::string getFilteredNameTag() const = 0;
    virtual void setFilteredNameTag(const std::string &) = 0;
    [[nodiscard]] virtual std::vector<std::string> getUgcStrings(const CompoundTag &) const = 0;
    [[nodiscard]] virtual std::vector<std::string> getFilteredUgcStrings(const CompoundTag &) const = 0;
    virtual void setUgcStrings(CompoundTag &, const std::vector<std::string> &) const = 0;
    virtual void setFilteredUgcStrings(CompoundTag &, const std::vector<std::string> &) const = 0;
    [[nodiscard]] virtual bool validateData(const CompoundTag &) const = 0;
    virtual void fixupOnLoad(LevelChunk &) = 0;
    virtual void setChanged() = 0;
    [[nodiscard]] virtual bool isChanged() const = 0;
    virtual void onChanged(BlockSource &) = 0;

protected:
    virtual void _setCanRenderCustomName(bool) = 0;
    virtual void _setCustomNameSaved(bool) = 0;
};

class IVanillaRenderBlockActorComponent {
public:
    virtual BlockActor &getBlockActor() = 0;
    [[nodiscard]] virtual const BlockActor &getBlockActor() const = 0;
    [[nodiscard]] virtual const BlockPos &getBlockActorPosition() const = 0;
    [[nodiscard]] virtual BlockActorType getBlockActorType() const = 0;
    [[nodiscard]] virtual BlockActorRendererId getRendererId() const = 0;
    [[nodiscard]] virtual bool isPermanentlyRendered() const = 0;
    [[nodiscard]] virtual bool isWithinRenderDistance(const Vec3 &) const = 0;
    [[nodiscard]] virtual bool hasAlphaLayer() const = 0;
    virtual float getShadowRadius(BlockSource &) const = 0;
    [[nodiscard]] virtual bool isInWorld() const = 0;
    virtual IVanillaRenderBlockActorComponent *getCrackEntity(BlockSource &, const BlockPos &) = 0;
    [[nodiscard]] virtual const AABB &getAABB() const = 0;
    virtual void setAABB(const AABB &) = 0;
    virtual ActorTerrainInterlockData &getEntityTerrainInterlockData() = 0;
    [[nodiscard]] virtual const ActorTerrainInterlockData &getEntityTerrainInterlockDataConst() const = 0;

protected:
    virtual void _resetAABB() = 0;
};

class IVanillaTickBlockActorComponent {
public:
    virtual BlockActor &getBlockActor() = 0;
    [[nodiscard]] virtual const BlockActor &getBlockActor() const = 0;
    [[nodiscard]] virtual const BlockPos &getBlockActorPosition() const = 0;
    [[nodiscard]] virtual BlockActorType getBlockActorType() const = 0;
    virtual void tick(BlockSource &) = 0;
    [[nodiscard]] virtual int getTickCount() const = 0;
};

class VanillaBlockActor : public BlockActor,
                          public IVanillaMainBlockActorComponent,
                          public IVanillaRenderBlockActorComponent,
                          public IVanillaTickBlockActorComponent {
    enum class Property : uint8_t {
        Changed = 0,
        Movable = 1,
        ClientSideOnly = 2,
        SaveCustomName = 3,
        CanRenderCustomName = 4,
        _count = 5,
    };

protected:
    using Properties = Bedrock::EnumSet<Property, Property::_count>;

public:
    void loadStatic() override;
    void load(ILevel &, const CompoundTag &, DataLoadHelper &) override;
    bool save(CompoundTag &, const SaveContext &) const override;
    BlockActor &getBlockActor() override;
    [[nodiscard]] const BlockActor &getBlockActor() const override;
    [[nodiscard]] const BlockPos &getBlockActorPosition() const override;
    [[nodiscard]] BlockActorType getBlockActorType() const override;
    [[nodiscard]] BlockActorRendererId getRendererId() const override;
    [[nodiscard]] bool isPermanentlyRendered() const override;
    [[nodiscard]] bool isWithinRenderDistance(const Vec3 &) const override;
    [[nodiscard]] bool hasAlphaLayer() const override;
    float getShadowRadius(BlockSource &) const override;
    [[nodiscard]] bool isInWorld() const override;
    IVanillaRenderBlockActorComponent *getCrackEntity(BlockSource &, const BlockPos &) override;
    [[nodiscard]] const AABB &getAABB() const override;
    void setAABB(const AABB &) override;
    ActorTerrainInterlockData &getEntityTerrainInterlockData() override;
    [[nodiscard]] const ActorTerrainInterlockData &getEntityTerrainInterlockDataConst() const override;
    void tick(BlockSource &) override;
    [[nodiscard]] int getTickCount() const override;
    Container *getContainer() override;
    [[nodiscard]] const Container *getContainer() const override;
    void eraseLootTable() override;
    [[nodiscard]] std::string getName() const override;
    std::string getImmersiveReaderText(BlockSource &) override;
    [[nodiscard]] bool canRenderCustomName() const override;
    [[nodiscard]] bool hasCustomName() const override;
    [[nodiscard]] const Bedrock::Safety::RedactableString &getCustomName() const override;
    void setCustomName(const Bedrock::Safety::RedactableString &) override;
    [[nodiscard]] bool isCustomNameSaved() const override;
    [[nodiscard]] Bedrock::Safety::RedactableString getDisplayName() const override;
    [[nodiscard]] bool hasFilteredNameTag() const override;
    [[nodiscard]] std::string getFilteredNameTag() const override;
    void setFilteredNameTag(const std::string &) override;
    [[nodiscard]] std::vector<std::string> getUgcStrings(const CompoundTag &) const override;
    [[nodiscard]] std::vector<std::string> getFilteredUgcStrings(const CompoundTag &) const override;
    void setUgcStrings(CompoundTag &, const std::vector<std::string> &) const override;
    void setFilteredUgcStrings(CompoundTag &, const std::vector<std::string> &) const override;
    [[nodiscard]] bool validateData(const CompoundTag &) const override;
    void fixupOnLoad(LevelChunk &) override;
    void setChanged() override { properties_.insert(Property::Changed); }
    [[nodiscard]] bool isChanged() const override { return properties_.contains(Property::Changed); }
    void onChanged(BlockSource &) override;

protected:
    void _resetAABB() override;
    void _setCanRenderCustomName(bool) override;
    void _setCustomNameSaved(bool) override;
    [[nodiscard]] const IVanillaRenderBlockActorComponent *_getRenderComponent() const override;
    IVanillaRenderBlockActorComponent *_getRenderComponent() override;
    [[nodiscard]] const IVanillaTickBlockActorComponent *_getTickComponent() const override;
    IVanillaTickBlockActorComponent *_getTickComponent() override;
    [[nodiscard]] const IVanillaMainBlockActorComponent *_getMainComponent() const override;
    IVanillaMainBlockActorComponent *_getMainComponent() override;

    int tick_count_;                                    // +72
    Properties properties_;                             // +76
    AABB bb_;                                           // +80
    BlockActorRendererId renderer_id_;                  // +104
    ActorTerrainInterlockData terrain_interlock_data_;  // +112
    Bedrock::Safety::RedactableString custom_name_;     // +136
    std::string filtered_custom_name_;                  // +192
};
