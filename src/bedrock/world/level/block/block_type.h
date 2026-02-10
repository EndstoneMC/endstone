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

#include <gsl/span>

#include "bedrock/common_types.h"
#include "bedrock/core/container/cache.h"
#include "bedrock/core/math/color.h"
#include "bedrock/core/string/string_hash.h"
#include "bedrock/core/utility/optional_ref.h"
#include "bedrock/forward.h"
#include "bedrock/resources/base_game_version.h"
#include "bedrock/util/int_range.h"
#include "bedrock/util/random.h"
#include "bedrock/world/direction.h"
#include "bedrock/world/flip.h"
#include "bedrock/world/item/item_category.h"
#include "bedrock/world/level/block/actor/block_actor_type.h"
#include "bedrock/world/level/block/components/block_component_storage.h"
#include "bedrock/world/level/block/flammable.h"
#include "bedrock/world/level/block/states/block_state.h"
#include "bedrock/world/level/block/tint_method.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/clip_parameters.h"
#include "bedrock/world/level/material/material.h"
#include "bedrock/world/phys/aabb.h"
#include "bedrock/world/phys/hit_result.h"

class Actor;
class Block;
class BlockActor;
class BlockSource;
class Container;
class IBlockSource;
class IConstBlockSource;
class ItemStack;
class ItemInstance;
class Player;

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
    // PistonBlockGrabber = 0x40000,
    TopSolidBlocking = 0x80000,
    CubeShaped = 0x200000,
    Power_NO = 0x400000,
    Power_BlockDown = 0x800000,
    Immovable = 0x1000000,
    // BreakOnPush = 0x2000000,
    Piston = 0x4000000,
    InfiniBurn = 0x8000000,
    RequiresWorldBuilder = 0x10000000,
    CausesDamage = 0x20000000,
    BreaksWhenFallenOnByFallingBlock = 0x40000000,
    // OnlyPistonPush = 0x80000000,
    Liquid = 0x100000000,
    // CanBeBuiltOver = 0x200000000,
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

class BlockType {
public:
    struct NameInfo {
        HashedString raw_name;             // +0
        std::string namespace_name;        // +48
        HashedString full_name;            // +80
        HashedString pre_flattening_name;  // +128
    };
    static constexpr int UPDATE_NEIGHBORS = 1;
    static constexpr int UPDATE_CLIENTS = 2;
    static constexpr int UPDATE_INVISIBLE = 4;
    static constexpr int UPDATE_ITEM_DATA = 16;
    static constexpr int UPDATE_NONE = 4;
    static constexpr int UPDATE_ALL = 3;
    static constexpr int TILE_NUM_SHIFT = 12;
    static constexpr int NUM_LEGACY_BLOCK_TYPES = 512;

    BlockType(const std::string &, int, const Material &);

    virtual ~BlockType() = default;
    [[nodiscard]] virtual std::shared_ptr<BlockActor> newBlockEntity(BlockPos const &, Block const &) const = 0;
    [[nodiscard]] virtual Block const *getNextBlockPermutation(Block const &) const = 0;
    bool hasTag(const HashedString &) const;
    bool hasTag(const HashType64 &) const;
    [[nodiscard]] virtual HitResult clip(Block const &, BlockSource const &, BlockPos const &, Vec3 const &,
                                         Vec3 const &, ShapeType,
                                         optional_ref<GetCollisionShapeInterface const>) const = 0;
    [[nodiscard]] virtual AABB getCollisionShape(Block const &, IConstBlockSource const &, BlockPos const &,
                                                 optional_ref<GetCollisionShapeInterface const>) const = 0;
    [[nodiscard]] virtual bool getCollisionShapeForCamera(AABB &, Block const &, IConstBlockSource const &,
                                                          BlockPos const &) const = 0;
    [[nodiscard]] virtual bool addCollisionShapes(Block const &, IConstBlockSource const &, BlockPos const &,
                                                  AABB const *, std::vector<AABB> &,
                                                  optional_ref<GetCollisionShapeInterface const>) const = 0;
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
    // [[nodiscard]] virtual bool canConnect(Block const &, FacingID, Block const &) const = 0;
    [[nodiscard]] virtual bool isMovingBlock() const = 0;
    [[nodiscard]] virtual CopperBehavior const *tryGetCopperBehavior() const = 0;
    [[nodiscard]] virtual bool isStemBlock() const = 0;
    [[nodiscard]] virtual bool isContainerBlock() const = 0;
    [[nodiscard]] virtual bool isCraftingBlock() const = 0;
    [[nodiscard]] virtual bool isLavaBlocking() const = 0;
    [[nodiscard]] virtual bool isFenceBlock() const = 0;
    [[nodiscard]] virtual bool isFenceGateBlock() const = 0;
    [[nodiscard]] virtual bool isThinFenceBlock() const = 0;
    [[nodiscard]] virtual bool isWallBlock() const = 0;
    [[nodiscard]] virtual bool isStairBlock() const = 0;
    [[nodiscard]] virtual bool isSlabBlock() const = 0;
    [[nodiscard]] virtual bool isDoorBlock() const = 0;
    [[nodiscard]] virtual bool isChestBlock() const = 0;
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
    // virtual void onRedstoneUpdate(BlockSource &, BlockPos const &, int, bool) const = 0;
    virtual void onMove(BlockSource &, BlockPos const &, BlockPos const &) const = 0;
    [[nodiscard]] virtual bool detachesOnPistonMove(BlockSource &, BlockPos const &) const = 0;
    virtual void movedByPiston(BlockSource &, BlockPos const &) const = 0;
    virtual void onStructureBlockPlace(BlockSource &, BlockPos const &) const = 0;
    virtual void setupRedstoneComponent(BlockSource &, BlockPos const &) const = 0;
    virtual void updateEntityAfterFallOn(BlockPos const &, UpdateEntityAfterFallOnInterface &) const = 0;
    [[nodiscard]] virtual bool isBounceBlock() const = 0;
    [[nodiscard]] virtual bool isPreservingMediumWhenPlaced(BlockType const *) const = 0;
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
    [[nodiscard]] virtual Block const &getPlacementBlock(Actor const &, BlockPos const &, FacingID, Vec3 const &,
                                                         int) const = 0;
    [[nodiscard]] virtual int calcVariant(BlockSource &, BlockPos const &, mce::Color const &) const = 0;
    [[nodiscard]] virtual bool isAttachedTo(BlockSource &, BlockPos const &, BlockPos &) const = 0;
    [[nodiscard]] virtual bool attack(Player *player, BlockPos const &) const = 0;
    [[nodiscard]] virtual bool shouldTriggerEntityInside(BlockSource &, BlockPos const &, Actor &) const = 0;
    [[nodiscard]] virtual bool canBeBuiltOver(const Block &, BlockSource &, BlockPos const &,
                                              BlockType const &) const = 0;
    [[nodiscard]] virtual bool canBeBuiltOver(const Block &, BlockSource &, BlockPos const &) const = 0;
    virtual void triggerEvent(BlockSource &, BlockPos const &, int b0, int b1) const = 0;
    // virtual void executeEvent(BlockSource &, BlockPos const &, Block const &, std::string const &, Actor &) const = 0;
    [[nodiscard]] virtual bool hasTag(BlockSource &, BlockPos const &, Block const &, std::string const &) const = 0;
    [[nodiscard]] virtual MobSpawnerData const *getMobToSpawn(SpawnConditions const &, BlockSource &) const = 0;
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
    [[nodiscard]] virtual BlockType &init() = 0;
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
    // virtual void tick(BlockSource &, BlockPos const &, Random &) const = 0;
    // virtual void randomTick(BlockSource &, BlockPos const &, Random &) const = 0;
    [[nodiscard]] virtual bool isInteractiveBlock() const = 0;
    // [[nodiscard]] virtual bool use(Player &, BlockPos const &, FacingID, std::optional<Vec3>) const = 0;
    // [[nodiscard]] virtual bool use(Player &, BlockPos const &, FacingID) const = 0;
    [[nodiscard]] virtual bool allowStateMismatchOnPlacement(Block const &, Block const &) const = 0;
    [[nodiscard]] virtual bool canSurvive(BlockSource &, BlockPos const &) const = 0;
    [[nodiscard]] virtual BlockRenderLayer getRenderLayer(Block const &, BlockSource &, BlockPos const &) const = 0;
    [[nodiscard]] virtual int getExtraRenderLayers() const = 0;
    [[nodiscard]] virtual const HashedString &getCullingLayer() const = 0;
    [[nodiscard]] virtual Brightness getLight(Block const &) const = 0;
    [[nodiscard]] virtual Brightness getEmissiveBrightness(Block const &) const = 0;
    [[nodiscard]] virtual mce::Color getMapColor(BlockSource &, BlockPos const &, Block const &) const = 0;
    virtual void _onHitByActivatingAttack(BlockSource &, BlockPos const &, Actor *) const = 0;
    virtual void entityInside(BlockSource &, BlockPos const &, Actor &) const = 0;

    [[nodiscard]] bool hasProperty(BlockProperty property) const;
    [[nodiscard]] const Block *tryGetStateFromLegacyData(DataID) const;
    [[nodiscard]] bool hasState(const BlockState &block_state) const;
    [[nodiscard]] bool hasState(const HashedString &name) const;
    template <typename T>
    T getState(const BlockState &block_state, DataID data) const
    {
        return getState<T>(block_state.getID(), data);
    }

    template <typename T>
    T getState(const size_t &id, DataID data) const
    {
        if (const auto it = states_.find(id); it != states_.end()) {
            return it->second.get<T>(data);
        }
        return static_cast<T>(_tryLookupAlteredStateCollection(id, data).value_or(0));
    }

    template <typename T>
    const Block *trySetState(const BlockState &block_state, T val, DataID data) const
    {
        if (const auto it = states_.find(block_state.getID()); it != states_.end()) {
            return it->second.trySet<T>(data, val, block_permutations_);
        }
        if (auto *result = _trySetStateFromAlteredStateCollection(block_state.getID(), static_cast<int>(val), data)) {
            return result;
        }
        if (return_default_block_on_unidentified_block_state_) {
            return &getDefaultState();
        }
        return nullptr;
    }

    [[nodiscard]] bool requiresCorrectToolForDrops() const;
    [[nodiscard]] bool isSolid() const;
    [[nodiscard]] float getThickness() const;
    [[nodiscard]] float getTranslucency() const;
    [[nodiscard]] const std::vector<HashedString> &getTags() const;
    [[nodiscard]] const Material &getMaterial() const;
    [[nodiscard]] const std::string &getDescriptionId() const;
    [[nodiscard]] const std::string &getRawNameId() const;
    [[nodiscard]] const std::string &getNamespace() const;
    [[nodiscard]] const HashedString &getName() const;
    bool anyOf(std::initializer_list<std::reference_wrapper<const HashedString>>) const;
    bool anyOf(const gsl::span<const std::reference_wrapper<const HashedString>> &block_type_ids) const;
    [[nodiscard]] const Block &getDefaultState() const;
    [[nodiscard]] const BaseGameVersion &getRequiredBaseGameVersion() const;
    [[nodiscard]] std::int16_t getBlockItemId() const;
    [[nodiscard]] TintMethod getTintMethod() const;
    void forEachBlockPermutation(std::function<bool(Block const &)> callback) const;

private:
    std::optional<int> _tryLookupAlteredStateCollection(size_t, DataID) const;
    const Block *_trySetStateFromAlteredStateCollection(size_t, int, DataID) const;

public:
    std::string description_id;  // +8

private:
    BlockComponentStorage components_;  // +40
    NameInfo name_info_;                // +144
    BlockProperty properties_;
    bool fancy_;
    BlockRenderLayer render_layer_;
    bool render_layer_can_render_as_opaque_;
    BlockActorType block_entity_type_;
    float thickness_;
    bool can_slide_;
    bool can_react_to_neighbors_during_instatick_;
    // bool is_interaction_;
    float gravity_;
    const Material &material_;
    bool falling_;
    float particle_quantity_scalar_;
    CreativeItemCategory creative_item_category_;
    std::string creative_group_;
    bool is_hidden_in_commands_;
    bool allows_runes_;
    bool can_be_broken_from_falling_;
    bool can_be_original_surface_;
    bool solid_;
    bool pushes_out_items_;
    bool ignore_block_for_inside_cube_renderer_;
    bool is_trapdoor_;
    bool is_door_;
    bool is_opaque_full_block_;
    float translucency_;
    bool should_random_tick_extra_layer_;
    bool is_mob_piece_;
    bool can_be_extra_block_;
    bool can_propagate_brightness_;

protected:
    Brightness light_block_;
    Brightness light_emission_;
    Color map_color_; // +400
    float friction_;
    NoteBlockInstrument note_block_instrument_;
    TintMethod tint_method_;
    bool return_default_block_on_unidentified_block_state_;

private:
    NewBlockID id_;  // +426
    BaseGameVersion min_required_game_version_;
    bool is_vanilla_;
    std::vector<HashedString> tags_;
    bool data_driven_vanilla_blocks_and_items_enabled_;
    AABB visual_shape_;
    std::int32_t bits_used_;
    std::int32_t total_bits_used_;
    std::map<std::uint64_t, BlockStateInstance> states_;  // +624
    std::unordered_map<HashedString, std::uint64_t> state_name_map_;
    std::size_t creative_enum_state_;
    std::vector<std::unique_ptr<Block>> block_permutations_;  // +712
    Block *default_state_;
    std::vector<std::unique_ptr<void *>> get_placement_block_callbacks_;
    Core::Cache<std::uint16_t, const Block *> legacy_data_lookup_table_;
    std::unique_ptr<void *> block_state_group_;
    std::unique_ptr<void *> resource_drops_strategy_;
    IntRange experience_drop_;
    bool requires_correct_tool_for_drops_;
    BlockComponentStorage net_ease_component_storage_;

public:
    struct AlteredStateCollection {
        [[nodiscard]] const BlockState &getBlockState() const { return block_state_; }
        [[nodiscard]] virtual std::optional<int> getState(const BlockType &, int) const = 0;
        [[nodiscard]] virtual const Block *setState(const BlockType &, int, int) const = 0;

    protected:
        AlteredStateCollection(const BlockState &);
        virtual ~AlteredStateCollection() = default;

    private:
        std::reference_wrapper<const BlockState> block_state_;
    };

private:
    std::vector<std::shared_ptr<AlteredStateCollection>> altered_state_collections_;
};
