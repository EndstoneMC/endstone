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

#include "bedrock/safety/redactable_string.h"
#include "bedrock/world/actor/actor_terrain_interlock_data.h"
#include "bedrock/world/item/save_context.h"
#include "bedrock/world/level/block/block.h"

class ILevel;

enum class BlockActorType : int {
    Undefined = 0,
    Furnace = 1,
    Chest = 2,
    NetherReactor = 3,
    Sign = 4,
    MobSpawner = 5,
    Skull = 6,
    FlowerPot = 7,
    BrewingStand = 8,
    EnchantingTable = 9,
    DaylightDetector = 10,
    Music = 11,
    Comparator = 12,
    Dispenser = 13,
    Dropper = 14,
    Hopper = 15,
    Cauldron = 16,
    ItemFrame = 17,
    PistonArm = 18,
    MovingBlock = 19,
    Chalkboard = 20,
    Beacon = 21,
    EndPortal = 22,
    EnderChest = 23,
    EndGateway = 24,
    ShulkerBox = 25,
    CommandBlock = 26,
    Bed = 27,
    Banner = 28,
    StructureBlock = 32,
    Jukebox = 33,
    ChemistryTable = 34,
    Conduit = 35,
    JigsawBlock = 36,
    Lectern = 37,
    BlastFurnace = 38,
    Smoker = 39,
    Bell = 40,
    Campfire = 41,
    BarrelBlock = 42,
    Beehive = 43,
    Lodestone = 44,
    SculkSensor = 45,
    SporeBlossom = 46,
    GlowItemFrame = 47,
    SculkCatalyst = 48,
    SculkShrieker = 49,
    HangingSign = 50,
    ChiseledBookshelf = 51,
    BrushableBlock = 52,
    DecoratedPot = 53,
    CalibratedSculkSensor = 54,
    Crafter = 55,
    TrialSpawner = 56,
    Vault = 57,
    CreakingHeart = 58,
    _count = 59,
};

class BlockActor {
public:
    BlockActor(BlockActorType, const BlockPos &, const std::string &);
    virtual ~BlockActor() = default;
    virtual void load(ILevel &, const CompoundTag &, DataLoadHelper &) = 0;
    virtual bool save(CompoundTag &, const SaveContext &) const = 0;
    virtual bool saveItemInstanceData(CompoundTag &, const SaveContext &) const = 0;
    virtual void saveBlockData(CompoundTag &, BlockSource &) const = 0;
    virtual void loadBlockData(const CompoundTag &, BlockSource &, DataLoadHelper &) = 0;
    virtual void onCustomTagLoadDone(BlockSource &) = 0;
    virtual bool isPermanentlyRendered() const = 0;
    virtual bool isWithinRenderDistance(const Vec3 &) const = 0;
    virtual void tick(BlockSource &) = 0;

    void setChanged()
    {
        changed_ = true;
    }

    [[nodiscard]] bool isChanged() const
    {
        return changed_;
    }

    [[nodiscard]] const BlockActorType &getType() const
    {
        return type_;
    }

    int tick_count;

protected:
    const Block *block_;
    float destroy_timer_;
    Vec3 destroy_direction_;
    float destroy_progress_;
    BlockPos position_;
    AABB bb_;
    const BlockActorType type_;
    BlockActorRendererId renderer_id_;
    Bedrock::Safety::RedactableString custom_name_;
    std::string filtered_custom_name_;
    int repair_cost_;
    bool client_side_only_;
    bool is_movable_;
    bool save_custom_name_;
    bool can_render_custom_name_;
    const float sign_shadow_radius_;
    ActorTerrainInterlockData terrain_interlock_data_;

private:
    bool changed_;
};
BEDROCK_STATIC_ASSERT_SIZE(BlockActor, 240, 216);
