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

#include <memory>
#include <string>
#include <vector>

#include "bedrock/forward.h"
#include "bedrock/safety/redactable_string.h"
#include "bedrock/shared_types/height.h"
#include "bedrock/world/actor/actor_terrain_interlock_data.h"
#include "bedrock/world/item/save_context.h"
#include "bedrock/world/level/block/actor/block_actor_type.h"
#include "bedrock/world/level/block/block.h"

class ILevel;
class CompoundTag;
class BlockActorDataPacket;
class IVanillaMainBlockActorComponent;
class IVanillaRenderBlockActorComponent;
class IVanillaTickBlockActorComponent;
class LevelChunk;
class Player;

class BlockActor {
public:
    BlockActor(BlockActorType type, const BlockPos &pos);
    virtual ~BlockActor() = default;
    virtual void load(ILevel &, const CompoundTag &, DataLoadHelper &) = 0;
    virtual bool save(CompoundTag &, const SaveContext &) const = 0;
    virtual bool saveItemInstanceData(CompoundTag &) const = 0;
    virtual void saveBlockData(CompoundTag &, BlockSource &) const = 0;
    virtual void loadBlockData(const CompoundTag &, BlockSource &, DataLoadHelper &) = 0;
    virtual void onCustomTagLoadDone(BlockSource &) = 0;
    static std::shared_ptr<BlockActor> loadStatic(const BlockType &, const BlockPos &, ILevel &, const CompoundTag &,
                                                  DataLoadHelper &);
    virtual void loadStatic() = 0;
    virtual void onPlace(BlockSource &) = 0;
    virtual void onMove() = 0;
    virtual void onRemoved(BlockSource &) = 0;
    virtual void onChunkLoaded(LevelChunk &) = 0;
    virtual void onChunkUnloaded(LevelChunk &) = 0;
    virtual void onSubChunkLoaded(LevelChunk &, SubChunkHeight, bool) = 0;
    virtual void onNeighborChanged(BlockSource &, const BlockPos &) = 0;
    virtual void triggerEvent(int, int) = 0;
    [[nodiscard]] virtual bool isTypeOrDerived(BlockActorType) const = 0;
    [[nodiscard]] BlockActorType getType() const { return type_; }
    virtual void getDebugText(std::vector<std::string> &, const BlockPos &, const BlockSource *) const = 0;

protected:
    BlockPos position_;          // +8
    const BlockActorType type_;  // +20
    virtual std::unique_ptr<BlockActorDataPacket> _getUpdatePacket(BlockSource &) = 0;
    virtual void _onUpdatePacket(const CompoundTag &, BlockSource &) = 0;
    [[nodiscard]] virtual bool _playerCanUpdate(const Player &) const = 0;
    [[nodiscard]] virtual const IVanillaRenderBlockActorComponent *_getRenderComponent() const = 0;
    virtual IVanillaRenderBlockActorComponent *_getRenderComponent() = 0;
    [[nodiscard]] virtual const IVanillaTickBlockActorComponent *_getTickComponent() const = 0;
    virtual IVanillaTickBlockActorComponent *_getTickComponent() = 0;
    [[nodiscard]] virtual const IVanillaMainBlockActorComponent *_getMainComponent() const = 0;
    virtual IVanillaMainBlockActorComponent *_getMainComponent() = 0;
    std::unique_ptr<DynamicPropertiesBlockActorComponent> dynamic_properties_;          // +24
    std::unique_ptr<RandomizableContainerBlockActorComponent> randomizable_container_;  // +32
    std::unique_ptr<ContainerBlockActorComponent> container_;                           // +40
};
BEDROCK_STATIC_ASSERT_SIZE(BlockActor, 48, 48);
