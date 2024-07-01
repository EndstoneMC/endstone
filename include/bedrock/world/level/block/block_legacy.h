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

#include <shared_mutex>

#include "bedrock/common/game_version.h"
#include "bedrock/core/hashed_string.h"
#include "bedrock/core/math/color.h"
#include "bedrock/core/math/int_range.h"
#include "bedrock/entity/entity_context.h"
#include "bedrock/forward.h"
#include "bedrock/world/direction.h"
#include "bedrock/world/flip.h"
#include "bedrock/world/level/block/block_component_storage.h"
#include "bedrock/world/level/block/block_property.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/block_source_interface.h"
#include "bedrock/world/level/material/material.h"
#include "bedrock/world/phys/aabb.h"
#include "block_state_instance.h"

using Brightness = std::uint8_t;
using FacingID = std::uint8_t;
using NewBlockID = std::uint16_t;
using DataID = std::uint16_t;

class Actor;
class Block;
class Container;
class ItemStack;
class ItemInstance;
class Player;

class BlockLegacy : public BlockComponentStorage {
    struct NameInfo {
        HashedString raw_name;             // +0
        std::string namespace_name;        // +48
        HashedString full_name;            // +80
        HashedString pre_flattening_name;  // +128
    };

public:
    ~BlockLegacy() override = 0;
    [[nodiscard]] virtual std::shared_ptr<BlockActor> newBlockEntity(BlockPos const &, Block const &) const = 0;
    [[nodiscard]] virtual Block const *getNextBlockPermutation(Block const &) const = 0;
    [[nodiscard]] virtual bool hasTag(BlockSource &, BlockPos const &, Block const &, std::string const &) const = 0;
    [[nodiscard]] virtual HitResult clip(Block const &, BlockSource const &, BlockPos const &, Vec3 const &,
                                         Vec3 const &, ShapeType,
                                         optional_ref<GetCollisionShapeInterface const>) const = 0;
    [[nodiscard]] virtual AABB getCollisionShape(Block const &, IConstBlockSource const &, BlockPos const &,
                                                 optional_ref<GetCollisionShapeInterface const>) const = 0;
    [[nodiscard]] virtual bool getCollisionShapeForCamera(AABB &, Block const &, IConstBlockSource const &,
                                                          BlockPos const &) const = 0;
    [[nodiscard]] virtual bool addCollisionShapes(Block const &, IConstBlockSource const &, BlockPos const &,
                                                  AABB const *, std::vector<AABB> &,
                                                  optional_ref<GetCollisionShapeInterface const>) = 0;
    virtual void addAABBs(Block const &, IConstBlockSource const &, BlockPos const &, AABB const *,
                          std::vector<AABB> &) const = 0;
    [[nodiscard]] virtual AABB const &getOutline(Block const &, IConstBlockSource const &, BlockPos const &,
                                                 AABB &) const = 0;
    [[nodiscard]] virtual AABB const &getVisualShapeInWorld(Block const &, IConstBlockSource const &, BlockPos const &,
                                                            AABB &) const = 0;
    [[nodiscard]] virtual AABB const &getVisualShape(Block const &, AABB &) const = 0;
    [[nodiscard]] virtual AABB const &getUIShape(Block const &, AABB &) const = 0;
    [[nodiscard]] virtual bool getLiquidClipVolume(Block const &, BlockSource &, BlockPos const &, AABB &) const = 0;
    [[nodiscard]] virtual bool isObstructingChests(BlockSource &, BlockPos const &, Block const &) const = 0;
    [[nodiscard]] virtual Vec3 randomlyModifyPosition(BlockPos const &, int &) const = 0;
    [[nodiscard]] virtual Vec3 randomlyModifyPosition(BlockPos const &) const = 0;
    virtual void onProjectileHit(BlockSource &, BlockPos const &, Actor const &) const = 0;
    virtual void onLightningHit(BlockSource &, BlockPos const &) const = 0;
    [[nodiscard]] virtual bool liquidCanFlowIntoFromDirection(FacingID,
                                                              std::function<Block const &(BlockPos const &)> const &,
                                                              BlockPos const &) const = 0;
    [[nodiscard]] virtual bool hasVariableLighting() const = 0;
    [[nodiscard]] virtual bool isStrippable(Block const &) const = 0;
    [[nodiscard]] virtual Block const &getStrippedBlock(Block const &) const = 0;
    [[nodiscard]] virtual bool canProvideSupport(Block const &, FacingID, BlockSupportType type) const = 0;
    [[nodiscard]] virtual bool canProvideMultifaceSupport(Block const &, FacingID) const = 0;
    [[nodiscard]] virtual bool canConnect(Block const &, FacingID, Block const &) const = 0;
    [[nodiscard]] virtual bool isMovingBlock() const = 0;
    [[nodiscard]] virtual CopperBehavior const *tryGetCopperBehavior() const = 0;
    [[nodiscard]] virtual bool canDamperVibrations() const = 0;
    [[nodiscard]] virtual bool canOccludeVibrations() const = 0;
    [[nodiscard]] virtual bool isStemBlock() const = 0;
    [[nodiscard]] virtual bool isContainerBlock() const = 0;
    [[nodiscard]] virtual bool isCraftingBlock() const = 0;
    [[nodiscard]] virtual bool isWaterBlocking() const = 0;
    [[nodiscard]] virtual bool isFenceBlock() const = 0;
    [[nodiscard]] virtual bool isFenceGateBlock() const = 0;
    [[nodiscard]] virtual bool isThinFenceBlock() const = 0;
    [[nodiscard]] virtual bool isWallBlock() const = 0;
    [[nodiscard]] virtual bool isStairBlock() const = 0;
    [[nodiscard]] virtual bool isSlabBlock() const = 0;
    [[nodiscard]] virtual bool isDoubleSlabBlock() const = 0;
    [[nodiscard]] virtual bool isDoorBlock() const = 0;
    [[nodiscard]] virtual bool isRailBlock() const = 0;
    [[nodiscard]] virtual bool isButtonBlock() const = 0;
    [[nodiscard]] virtual bool isLeverBlock() const = 0;
    [[nodiscard]] virtual bool isCandleCakeBlock() const = 0;
    [[nodiscard]] virtual bool isMultifaceBlock() const = 0;
    [[nodiscard]] virtual bool isSignalSource() const = 0;
    [[nodiscard]] virtual bool canBeOriginalSurface() const = 0;
    [[nodiscard]] virtual bool isSilentWhenJumpingOff() const = 0;
    [[nodiscard]] virtual bool isValidAuxValue(int) const = 0;
    [[nodiscard]] virtual bool canFillAtPos(BlockSource &, BlockPos const &, Block const &) const = 0;
    [[nodiscard]] virtual Block const &sanitizeFillBlock(Block const &) const = 0;
    virtual void onFillBlock(BlockSource &, BlockPos const &, Block const &) const = 0;
    [[nodiscard]] virtual int getDirectSignal(BlockSource &, BlockPos const &, int dir) const = 0;
    [[nodiscard]] virtual bool canBeDestroyedByWaterSpread() const = 0;
    [[nodiscard]] virtual bool waterSpreadCausesSpawn() const = 0;
    [[nodiscard]] virtual bool canContainLiquid() const = 0;
    [[nodiscard]] virtual std::optional<HashedString> getRequiredMedium() const = 0;
    [[nodiscard]] virtual bool shouldConnectToRedstone(BlockSource &, BlockPos const &, Direction::Type) const = 0;
    virtual void handlePrecipitation(BlockSource &, BlockPos const &, float, float) const = 0;
    [[nodiscard]] virtual bool canBeUsedInCommands(BaseGameVersion const &) const = 0;
    [[nodiscard]] virtual bool checkIsPathable(Actor &, BlockPos const &, BlockPos const &) const = 0;
    [[nodiscard]] virtual bool shouldDispense(BlockSource &, Container &) const = 0;
    [[nodiscard]] virtual bool dispense(BlockSource &, Container &, int, Vec3 const &, FacingID) const = 0;
    virtual void transformOnFall(BlockSource &, BlockPos const &, Actor *, float) const = 0;
    virtual void onRedstoneUpdate(BlockSource &, BlockPos const &, int, bool) const = 0;
    virtual void onMove(BlockSource &, BlockPos const &, BlockPos const &) const = 0;
    [[nodiscard]] virtual bool detachesOnPistonMove(BlockSource &, BlockPos const &) const = 0;
    virtual void movedByPiston(BlockSource &, BlockPos const &) const = 0;
    virtual void onStructureBlockPlace(BlockSource &, BlockPos const &) const = 0;
    virtual void onStructureNeighborBlockPlace(BlockSource &, BlockPos const &) const = 0;
    virtual void setupRedstoneComponent(BlockSource &, BlockPos const &) const = 0;
    [[nodiscard]] virtual ::BlockProperty getRedstoneProperty(BlockSource &, BlockPos const &) const = 0;
    virtual void updateEntityAfterFallOn(BlockPos const &, UpdateEntityAfterFallOnInterface &) const = 0;
    [[nodiscard]] virtual bool isBounceBlock() const = 0;
    [[nodiscard]] virtual bool isPreservingMediumWhenPlaced(BlockLegacy const *) const = 0;
    [[nodiscard]] virtual bool isFilteredOut(BlockRenderLayer) const = 0;
    [[nodiscard]] virtual bool canRenderSelectionOverlay(BlockRenderLayer) const = 0;
    [[nodiscard]] virtual bool ignoreEntitiesOnPistonMove(Block const &) const = 0;
    [[nodiscard]] virtual bool onFertilized(BlockSource &, BlockPos const &, Actor *, FertilizerType) const = 0;
    [[nodiscard]] virtual bool mayConsumeFertilizer(BlockSource &) const = 0;
    [[nodiscard]] virtual bool canBeFertilized(BlockSource &, BlockPos const &, Block const &) const = 0;
    [[nodiscard]] virtual bool mayPick() const = 0;
    [[nodiscard]] virtual bool mayPick(BlockSource const &region, Block const &, bool liquid) const = 0;
    [[nodiscard]] virtual bool mayPlace(BlockSource &, BlockPos const &, FacingID) const = 0;
    [[nodiscard]] virtual bool mayPlace(BlockSource &, BlockPos const &) const = 0;
    [[nodiscard]] virtual bool mayPlaceOn(BlockSource &, BlockPos const &) const = 0;
    [[nodiscard]] virtual bool tryToPlace(BlockSource &, BlockPos const &, Block const &,
                                          ActorBlockSyncMessage const *) const = 0;
    [[nodiscard]] virtual bool tryToTill(BlockSource &, BlockPos const &, Actor &, ItemStack &) const = 0;
    [[nodiscard]] virtual bool breaksFallingBlocks(Block const &, BaseGameVersion) const = 0;
    virtual void destroy(BlockSource &, BlockPos const &, Block const &, Actor *) const = 0;
    [[nodiscard]] virtual bool getIgnoresDestroyPermissions(Actor &, BlockPos const &) const = 0;
    virtual void neighborChanged(BlockSource &, BlockPos const &, BlockPos const &) const = 0;
    [[nodiscard]] virtual bool getSecondPart(IConstBlockSource const &region, BlockPos const &,
                                             BlockPos &out) const = 0;
    [[nodiscard]] virtual Block const *playerWillDestroy(Player &player, BlockPos const &, Block const &) const = 0;
    [[nodiscard]] virtual ItemInstance asItemInstance(Block const &, BlockActor const *) const = 0;
    virtual void spawnAfterBreak(BlockSource &, Block const &, BlockPos const &,
                                 ResourceDropsContext const &) const = 0;
    [[nodiscard]] virtual Block const &getPlacementBlock(Actor const &by, BlockPos const &, FacingID,
                                                         Vec3 const &clickPos, int itemValue) const = 0;
    [[nodiscard]] virtual int calcVariant(BlockSource &, BlockPos const &, mce::Color const &baseColor) const = 0;
    [[nodiscard]] virtual bool isAttachedTo(BlockSource &, BlockPos const &, BlockPos &outAttachedTo) const = 0;
    [[nodiscard]] virtual bool attack(Player *player, BlockPos const &) const = 0;
    [[nodiscard]] virtual bool shouldTriggerEntityInside(BlockSource &, BlockPos const &, Actor &) const = 0;
    [[nodiscard]] virtual bool canBeBuiltOver(BlockSource &, BlockPos const &, BlockItem const &newItem) const = 0;
    [[nodiscard]] virtual bool canBeBuiltOver(BlockSource &, BlockPos const &) const = 0;
    virtual void triggerEvent(BlockSource &, BlockPos const &, int b0, int b1) const = 0;
    virtual void executeEvent(BlockSource &, BlockPos const &, Block const &, std::string const &eventName,
                              Actor &sourceEntity) const = 0;
    [[nodiscard]] virtual MobSpawnerData const *getMobToSpawn(SpawnConditions const &conditions,
                                                              BlockSource &) const = 0;
    [[nodiscard]] virtual bool shouldStopFalling(Actor &) const = 0;
    [[nodiscard]] virtual bool pushesUpFallingBlocks() const = 0;
    [[nodiscard]] virtual bool canHaveExtraData() const = 0;
    [[nodiscard]] virtual bool hasComparatorSignal() const = 0;
    [[nodiscard]] virtual int getComparatorSignal(BlockSource &, BlockPos const &, Block const &, FacingID) const = 0;
    [[nodiscard]] virtual bool canSlide(BlockSource &, BlockPos const &) const = 0;
    [[nodiscard]] virtual bool canInstatick() const = 0;
    [[nodiscard]] virtual bool canSpawnAt(BlockSource const &region, BlockPos const &) const = 0;
    virtual void notifySpawnedAt(BlockSource &, BlockPos const &) const = 0;
    [[nodiscard]] virtual bool causesFreezeEffect() const = 0;
    [[nodiscard]] virtual int getIconYOffset() const = 0;
    [[nodiscard]] virtual std::string buildDescriptionId(Block const &) const = 0;
    [[nodiscard]] virtual bool isAuxValueRelevantForPicking() const = 0;
    [[nodiscard]] virtual int getColor(Block const &) const = 0;
    [[nodiscard]] virtual int getColor(BlockSource &, BlockPos const &, Block const &) const = 0;
    [[nodiscard]] virtual int getColorAtPos(BlockSource &, BlockPos const &) const = 0;
    [[nodiscard]] virtual int getColorForParticle(BlockSource &, BlockPos const &, Block const &) const = 0;
    [[nodiscard]] virtual bool isSeasonTinted(Block const &, BlockSource &, BlockPos const &) const = 0;
    virtual void onGraphicsModeChanged(BlockGraphicsModeChangeContext const &) = 0;
    [[nodiscard]] virtual float getShadeBrightness(Block const &) const = 0;
    [[nodiscard]] virtual int telemetryVariant(BlockSource &, BlockPos const &) const = 0;
    [[nodiscard]] virtual int getVariant(Block const &) const = 0;
    [[nodiscard]] virtual bool canSpawnOn(Actor *) const = 0;
    [[nodiscard]] virtual Block const &getRenderBlock() const = 0;
    [[nodiscard]] virtual FacingID getMappedFace(FacingID, Block const &) const = 0;
    [[nodiscard]] virtual Flip getFaceFlip(FacingID, Block const &) const = 0;
    virtual void animateTickBedrockLegacy(BlockSource &, BlockPos const &, Random &) const = 0;
    virtual void animateTick(BlockSource &, BlockPos const &, Random &) const = 0;
    [[nodiscard]] virtual BlockLegacy &init() = 0;
    [[nodiscard]] virtual Brightness getLightEmission(Block const &) const = 0;
    [[nodiscard]] virtual Block const *tryLegacyUpgrade(DataID) const = 0;
    [[nodiscard]] virtual bool dealsContactDamage(Actor const &, Block const &, bool) const = 0;
    [[nodiscard]] virtual Block const *tryGetInfested(Block const &) const = 0;
    [[nodiscard]] virtual Block const *tryGetUninfested(Block const &) const = 0;
    virtual void _addHardCodedBlockComponents(Experiments const &) = 0;
    virtual void onRemove(BlockSource &, BlockPos const &) const = 0;
    virtual void onExploded(BlockSource &, BlockPos const &, Actor *) const = 0;
    virtual void onStandOn(EntityContext &, BlockPos const &) const = 0;
    virtual void onPlace(BlockSource &, BlockPos const &) const = 0;
    [[nodiscard]] virtual bool shouldTickOnSetBlock() const = 0;
    virtual void tick(BlockSource &, BlockPos const &, Random &) const = 0;
    virtual void randomTick(BlockSource &, BlockPos const &, Random &) const = 0;
    [[nodiscard]] virtual bool isInteractiveBlock() const = 0;
    [[nodiscard]] virtual bool use(Player &, BlockPos const &, FacingID, std::optional<Vec3>) const = 0;
    [[nodiscard]] virtual bool use(Player &, BlockPos const &, FacingID) const = 0;
    [[nodiscard]] virtual bool allowStateMismatchOnPlacement(Block const &, Block const &) const = 0;
    [[nodiscard]] virtual bool canSurvive(BlockSource &, BlockPos const &) const = 0;
    [[nodiscard]] virtual ::BlockRenderLayer getRenderLayer() const = 0;
    [[nodiscard]] virtual ::BlockRenderLayer getRenderLayer(Block const &, BlockSource &, BlockPos const &) const = 0;
    [[nodiscard]] virtual int getExtraRenderLayers() const = 0;
    [[nodiscard]] virtual Brightness getLight(Block const &) const = 0;
    [[nodiscard]] virtual Brightness getEmissiveBrightness(Block const &) const = 0;
    [[nodiscard]] virtual mce::Color getMapColor(BlockSource &, BlockPos const &, Block const &) const = 0;
    virtual void _onHitByActivatingAttack(BlockSource &, BlockPos const &, Actor *) const = 0;
    virtual void entityInside(BlockSource &, BlockPos const &, Actor &) const = 0;
    [[nodiscard]] virtual mce::Color getDustColor(Block const &) const = 0;
    virtual void _iterateCandles(Block const &, BlockPos const &, std::function<void(Vec3 const &, int)>) const = 0;
    [[nodiscard]] virtual ItemInstance const getBaseSeed() const = 0;
    virtual void onLand(BlockSource &, BlockPos const &) const = 0;
    [[nodiscard]] virtual bool isFreeToFall(BlockSource &, BlockPos const &) const = 0;
    virtual void startFalling(BlockSource &, BlockPos const &, Block const &, bool) const = 0;
    [[nodiscard]] virtual int getInputSignal(BlockSource &, BlockPos const &) const = 0;
    [[nodiscard]] virtual bool isAlternateInput(Block const &) const = 0;
    [[nodiscard]] virtual int getAlternateSignal(BlockSource &, BlockPos const &) const = 0;
    [[nodiscard]] virtual int getOutputSignal(Block const &) const = 0;
    [[nodiscard]] virtual int getTurnOffDelay(Block const &) const = 0;
    [[nodiscard]] virtual int getTurnOnDelay(Block const &) const = 0;
    [[nodiscard]] virtual Block const *getOnBlock(Block const *) const = 0;
    [[nodiscard]] virtual Block const *getOffBlock(Block const *) const = 0;

    [[nodiscard]] bool canDropWithAnyTool() const
    {
        return can_drop_with_any_tool_;
    }

    [[nodiscard]] float getThickness() const
    {
        return thickness_;
    }

    [[nodiscard]] const Material &getMaterial() const
    {
        return *material_;
    }

    [[nodiscard]] const std::vector<HashedString> &getTags() const
    {
        return tags_;
    }

    [[nodiscard]] const std::string &getDescriptionId() const
    {
        return description_id_;
    }

    [[nodiscard]] const std::string &getFullNameId() const
    {
        return name_info_.full_name.getString();
    }

    [[nodiscard]] const std::string &getRawNameId() const
    {
        return name_info_.raw_name.getString();
    }

    [[nodiscard]] const std::string &getNamespace() const
    {
        return name_info_.namespace_name;
    }

    [[nodiscard]] const Block *getDefaultState() const
    {
        return default_state_;
    }

    void forEachBlockPermutation(std::function<bool(Block const &)> callback) const
    {
        for (const auto &block_permutation : block_permutations_) {
            if (block_permutation) {
                callback(*block_permutation);
            }
        }
    }

private:
    std::string description_id_;                                      // +40
    NameInfo name_info_;                                              // +72  (+64)
    BlockProperty properties_;                                        // +248 (+208)
    bool fancy_;                                                      // +256 (+216)
    BlockRenderLayer render_layer_;                                   // +260 (+220)
    bool render_layer_can_render_as_opaque_;                          // +264 (+224)
    BlockActorType block_entity_type_;                                // +268 (+228)
    bool animated_texture_;                                           // +272 (+232)
    float brightness_gamma_;                                          // +276 (+236)
    float thickness_;                                                 // +280 (+240)
    bool can_slide_;                                                  // +284 (+244)
    bool can_react_to_neighbors_during_instatick_;                    // +285 (+245)
    bool is_interaction_;                                             // +286 (+246)
    float gravity_;                                                   // +288 (+248)
    Material *material_;                                              // +296 (+256)
    bool heavy_;                                                      // +304 (+264)
    float particle_quantity_scalar_;                                  // +308 (+268)
    CreativeItemCategory creative_item_category_;                     // +312 (+272)
    std::string creative_group_;                                      // +320 (+280)
    bool is_hidden_in_commands_;                                      // +352 (+304)
    bool allow_runes_;                                                // +353 (+305)
    bool can_be_broken_from_falling_;                                 // +354 (+306)
    bool can_be_original_surface_;                                    // +355 (+307)
    bool solid_;                                                      // +356 (+308)
    bool pushes_out_items_;                                           // +357 (+309)
    bool ignore_block_for_inside_cube_renderer_;                      // +358 (+310)
    bool is_trapdoor_;                                                // +359 (+311)
    bool is_door_;                                                    // +360 (+312)
    bool is_opaque_full_block_;                                       // +361 (+313)
    float translucency_;                                              // +364 (+316)
    bool should_random_tick_;                                         // +368 (+320)
    bool should_random_tick_extra_layer_;                             // +369 (+321)
    bool is_mob_piece_;                                               // +370 (+322)
    bool can_be_extra_block_;                                         // +371 (+323)
    bool can_propagate_brightness_;                                   // +372 (+324)
    Brightness light_block_;                                          // +373 (+325)
    Brightness light_emission_;                                       // +374 (+326)
    FlameOdds flame_odds_;                                            // +376 (+328)
    BurnOdds burn_odds_;                                              // +380 (+332)
    LavaFlammable lava_flammable_;                                    // +384 (+336)
    float destroy_speed_;                                             // +388 (+340)
    float explosion_resistence_;                                      // +392 (+344)
    mce::Color map_color_;                                            // +396 (+348)
    float friction_;                                                  // +412 (+364)
    BlockTintType block_tint_type_;                                   // +416 (+368)
    bool unknown_;                                                    // +420 (+372)
    NewBlockID id_;                                                   // +422 (+374)
    BaseGameVersion min_required_game_version_;                       // +424 (+376)
    bool is_vanilla_;                                                 // +544 (+472)
    std::vector<HashedString> tags_;                                  // +552 (+480)
    std::unordered_map<std::string, void *> event_handlers_;          // +576 (+504) void* = DefinitionEvent
    bool enable_data_driven_vanilla_blocks_and_items_;                // +640 (+544)
    AABB visual_shape_;                                               // +644 (+548)
    std::int64_t unknown3_;                                           // +672 (+576)
    std::map<std::uint64_t, BlockStateInstance> states_;              // +680 (+584) void* = BlockStateInstance
    std::unordered_map<HashedString, std::uint64_t> state_name_map_;  // +696 (+608)
    BlockState *creative_enum_state_;                                 // +760 (+648)
    std::vector<std::unique_ptr<Block>> block_permutations_;          // +768 (+656)
    Block *default_state_;                                            // +792 (+680)
    std::vector<void *> unknown4_;                                    // +800 (+688)
#ifdef __linux__                                                      //
    std::shared_timed_mutex mutex_;                                   //      (+712)
#else                                                                 //
    std::int64_t unknown5_;  // +824
#endif                                                                //
    std::unordered_map<void *, void *> unknown6_;                     // +832 (+856)
    std::unique_ptr<void *> block_state_group_;                       // +896        void* = BlockStateGroup
    std::unique_ptr<void *> resource_drops_strategy_;                 // +904        void* = IResourceDropsStrategy
    IntRange experience_drop_;                                        // +912
    bool can_drop_with_any_tool_;                                     // +920
    std::vector<void *> unknown9_;                                    // +928
    std::vector<void *> unknown10_;                                   // +952
    // ...
};
