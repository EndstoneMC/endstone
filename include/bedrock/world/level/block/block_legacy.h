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

#include <optional>

#include "bedrock/common_types.h"
#include "bedrock/core/container/cache.h"
#include "bedrock/core/math/color.h"
#include "bedrock/core/string/string_hash.h"
#include "bedrock/core/utility/optional_ref.h"
#include "bedrock/entity/gamerefs_entity/entity_context.h"
#include "bedrock/forward.h"
#include "bedrock/resources/base_game_version.h"
#include "bedrock/util/int_range.h"
#include "bedrock/world/direction.h"
#include "bedrock/world/flip.h"
#include "bedrock/world/level/block/actor/block_actor.h"
#include "bedrock/world/level/block/block_color_logic.h"
#include "bedrock/world/level/block/block_state_instance.h"
#include "bedrock/world/level/block/components/block_component_storage.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/phys/aabb.h"

class Actor;
class Block;
class BlockSource;
class Container;
class IBlockSource;
class IConstBlockSource;
class ItemStack;
class ItemInstance;
class Player;
class Material;
class MaterialType;

enum class BlockProperty : std::uint64_t {
    None = 0x0,
    Stair = 0x1,
    HalfSlab = 0x2,
    Hopper = 0x4,
    TopSnow = 0x8,
    FenceGate = 0x10,
    Leaves = 0x20,
    ThinConnects2D = 0x40,
    Connects2D = 0x80,
    Carpet = 0x100,
    Button = 0x200,
    Door = 0x400,
    Portal = 0x800,
    CanFall = 0x1000,
    Snow = 0x2000,
    Trap = 0x4000,
    Sign = 0x8000,
    Walkable = 0x10000,
    PressurePlate = 0x20000,
    PistonBlockGrabber = 0x40000,
    TopSolidBlocking = 0x80000,
    CubeShaped = 0x200000,
    Power_NO = 0x400000,
    Power_BlockDown = 0x800000,
    Immovable = 0x1000000,
    BreakOnPush = 0x2000000,
    Piston = 0x4000000,
    InfiniBurn = 0x8000000,
    RequiresWorldBuilder = 0x10000000,
    CausesDamage = 0x20000000,
    BreaksWhenFallenOnByFallingBlock = 0x40000000,
    OnlyPistonPush = 0x80000000,
    Liquid = 0x100000000,
    CanBeBuiltOver = 0x200000000,
    SnowRecoverable = 0x400000000,
    Scaffolding = 0x800000000,
    CanSupportCenterHangingBlock = 0x1000000000,
    BreaksWhenHitByArrow_DEPRECATED = 0x2000000000,
    Unwalkable = 0x4000000000,
    Hollow = 0x10000000000,
    OperatorBlock = 0x20000000000,
    SupportedByFlowerPot = 0x40000000000,
    PreventsJumping = 0x80000000000,
    ContainsHoney = 0x100000000000,
    Slime = 0x200000000000,
    SculkReplaceable_DEPRECATED = 0x400000000000,
    Climbable = 0x800000000000,
    CanHaltWhenClimbing = 0x1000000000000,
    _entt_enum_as_bitmask
};

class BlockLegacy {
    struct NameInfo {
        HashedString raw_name;             // +0
        std::string namespace_name;        // +48
        HashedString full_name;            // +80
        HashedString pre_flattening_name;  // +128
    };

public:
    virtual ~BlockLegacy() = 0;
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
    [[nodiscard]] virtual bool isDoorBlock() const = 0;
    [[nodiscard]] virtual bool isRailBlock() const = 0;
    [[nodiscard]] virtual bool isButtonBlock() const = 0;
    [[nodiscard]] virtual bool isLeverBlock() const = 0;
    [[nodiscard]] virtual bool isCandleCakeBlock() const = 0;
    [[nodiscard]] virtual bool isMultifaceBlock() const = 0;
    [[nodiscard]] virtual bool isSignalSource() const = 0;
    [[nodiscard]] virtual bool isConsumerComponent() const = 0;
    [[nodiscard]] virtual bool canBeOriginalSurface() const = 0;
    [[nodiscard]] virtual bool isSilentWhenJumpingOff() const = 0;
    [[nodiscard]] virtual bool isValidAuxValue(int) const = 0;
    [[nodiscard]] virtual bool canFillAtPos(BlockSource &, BlockPos const &, Block const &) const = 0;
    [[nodiscard]] virtual Block const &sanitizeFillBlock(Block const &) const = 0;
    virtual void onFillBlock(BlockSource &, BlockPos const &, Block const &) const = 0;
    [[nodiscard]] virtual int getDirectSignal(BlockSource &, BlockPos const &, int dir) const = 0;
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
    [[nodiscard]] virtual std::string buildDescriptionId(Block const &) const = 0;
    [[nodiscard]] virtual bool isAuxValueRelevantForPicking() const = 0;
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

    [[nodiscard]] bool requiresCorrectToolForDrops() const;
    [[nodiscard]] bool isSolid() const;
    [[nodiscard]] float getThickness() const;
    [[nodiscard]] float getTranslucency() const;
    [[nodiscard]] const std::vector<HashedString> &getTags() const;
    [[nodiscard]] const std::string &getDescriptionId() const;
    [[nodiscard]] const std::string &getFullNameId() const;
    [[nodiscard]] const std::string &getRawNameId() const;
    [[nodiscard]] const std::string &getNamespace() const;
    [[nodiscard]] const Block *getDefaultState() const;
    void forEachBlockPermutation(std::function<bool(Block const &)> callback) const;

    std::string description_id_;  // +8

private:
    BlockComponentStorage components_;                                    // +40
    NameInfo name_info_;                                                  // +144
    BlockProperty properties_;                                            //
    bool fancy_;                                                          //
    BlockRenderLayer render_layer_;                                       //
    bool render_layer_can_render_as_opaque_;                              //
    BlockActorType block_entity_type_;                                    //
    bool animated_texture_;                                               //
    float brightness_gamma_;                                              //
    float thickness_;                                                     //
    bool can_slide_;                                                      //
    bool can_react_to_neighbors_during_instatick_;                        //
    bool is_interaction_;                                                 //
    float gravity_;                                                       //
    Material *material_;                                                  // +360
    bool falling_;                                                        //
    float particle_quantity_scalar_;                                      //
    CreativeItemCategory creative_item_category_;                         //
    std::string creative_group_;                                          //
    bool is_hidden_in_commands_;                                          //
    bool allows_runes_;                                                   //
    bool can_be_broken_from_falling_;                                     //
    bool can_be_original_surface_;                                        //
    bool solid_;                                                          //
    bool pushes_out_items_;                                               //
    bool ignore_block_for_inside_cube_renderer_;                          //
    bool is_trapdoor_;                                                    //
    bool is_door_;                                                        //
    bool is_opaque_full_block_;                                           //
    float translucency_;                                                  //
    bool should_random_tick_;                                             //
    bool should_random_tick_extra_layer_;                                 //
    bool is_mob_piece_;                                                   //
    bool can_be_extra_block_;                                             //
    bool can_propagate_brightness_;                                       //
    Brightness light_block_;                                              //
    Brightness light_emission_;                                           //
    FlameOdds flame_odds_;                                                //
    BurnOdds burn_odds_;                                                  //
    LavaFlammable lava_flammable_;                                        //
    mce::Color map_color_;                                                //
    float friction_;                                                      //
    BlockTintType block_tint_type_;                                       //
    bool return_default_block_on_unidentified_block_state_;               //
    BlockColorLogic color_logic_;                                         //
    NewBlockID id_;                                                       //
    BaseGameVersion min_required_game_version_;                           //
    bool is_vanilla_;                                                     //
    std::vector<HashedString> tags_;                                      //
    std::unordered_map<std::string, void *> event_handlers_;              //
    bool data_driven_vanilla_blocks_and_items_enabled_;                   //
    AABB visual_shape_;                                                   //
    std::int32_t bits_used_;                                              //
    std::int32_t total_bits_used_;                                        //
    std::map<std::uint64_t, BlockStateInstance> states_;                  //
    std::unordered_map<HashedString, std::uint64_t> state_name_map_;      //
    std::size_t creative_enum_state_;                                     //
    std::vector<std::unique_ptr<Block>> block_permutations_;              //
    Block *default_state_;                                                //
    std::vector<std::unique_ptr<void *>> get_placement_block_callbacks_;  //
    Core::Cache<std::uint16_t, const Block *> legacy_data_lookup_table_;  //
    std::unique_ptr<void *> block_state_group_;                           //
    std::unique_ptr<void *> resource_drops_strategy_;                     //
    IntRange experience_drop_;                                            //
    bool requires_correct_tool_for_drops;                                 //
                                                                          // ...
};
