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

#include "bedrock/deps/json/value.h"
#include "bedrock/forward.h"
#include "bedrock/resources/base_game_version.h"
#include "bedrock/safety/redactable_string.h"
#include "bedrock/shared_ptr.h"
#include "bedrock/shared_types/legacy/level_sound_event.h"
#include "bedrock/world/actor/actor_definition_identifier.h"
#include "bedrock/world/actor/actor_location.h"
#include "bedrock/world/gamemode/interaction_result.h"
#include "bedrock/world/interactions/mining/mine_block_item_effect_type.h"
#include "bedrock/world/item/item_descriptor.h"
#include "bedrock/world/item/item_helper.h"
#include "bedrock/world/item/rarity.h"
#include "bedrock/world/item/registry/item_registry.h"
#include "bedrock/world/item/vanilla_item_tag.h"
#include "bedrock/world/level/block/block_type.h"

using BlockShape = std::int64_t;

class ItemStackBase;
class Level;
class Mob;

struct NoItemTintStrategy {};
struct ActorItemTintStrategy {
    const ActorDefinitionIdentifier &identifier;
};
struct ItemTintStrategy {
    using Variant = std::variant<NoItemTintStrategy, ActorItemTintStrategy>;
    Variant data;
};

class Item {
public:
    static const std::uint8_t MAX_STACK_SIZE = 64;
    static const int INVALID_ITEM_ID = -1;
    static const std::string TAG_DAMAGE;

    Item(const std::string &, short);

    virtual ~Item() = 0;
    virtual bool initServer(Json::Value const &, SemVersion const &, PackLoadContext &, JsonBetaState) = 0;
    virtual void tearDown() = 0;
    virtual Item &setDescriptionId(std::string const &) = 0;
    virtual std::string const &getDescriptionId() const = 0;
    virtual int getMaxUseDuration(ItemStack const *) const = 0;
    virtual WeakPtr<const BlockType> const &getBlockTypeForRendering() const = 0;
    virtual bool isMusicDisk() const = 0;
    virtual void executeEvent(ItemStackBase &, std::string const &, RenderParams &) const = 0;
    virtual bool isComponentBased() const = 0;
    virtual bool isHumanoidArmor() const = 0;
    virtual bool isBlockPlanterItem() const = 0;
    virtual bool isBucket() const = 0;
    virtual bool isCandle() const = 0;
    virtual bool isDamageable() const = 0;
    virtual bool isDyeable() const = 0;
    virtual bool isDye() const = 0;
    virtual ItemColor getItemColor() const = 0;
    virtual bool isFertilizer() const = 0;
    virtual bool isFood() const = 0;
    virtual bool isThrowable() const = 0;
    virtual bool isUseable() const = 0;
    virtual bool isTrimAllowed() const = 0;
    virtual bool isBodyArmor() const = 0;
    virtual ItemComponent *getComponent(HashedString const &) const = 0;
    virtual IFoodItemComponent *getFood() const = 0;
    virtual Item &setMaxDamage(int) = 0;
    virtual Item &setMaxUseDuration(int) = 0;
    virtual std::unique_ptr<CompoundTag> buildNetworkTag() const = 0;
    virtual void initializeFromNetwork(CompoundTag const &) = 0;
    virtual std::vector<std::string> validateFromNetwork(CompoundTag const &) = 0;
    virtual BlockShape getBlockShape() const = 0;
    virtual bool canBeDepleted() const = 0;
    virtual bool canDestroySpecial(Block const &) const = 0;
    virtual int getLevelDataForAuxValue(int) const = 0;
    virtual bool isStackedByData() const = 0;
    virtual std::int16_t getMaxDamage() const = 0;
    virtual int getAttackDamage() const = 0;
    virtual float getAttackDamageBonus(Actor const &) const = 0;
    virtual bool isHandEquipped() const = 0;
    virtual bool isGlint(ItemStackBase const &) const = 0;
    virtual bool isPattern() const = 0;
    virtual int getPatternIndex() const = 0;
    virtual Rarity getBaseRarity() const;
    virtual Rarity getRarity(ItemStackBase const &stack) const;
    virtual std::uint32_t getSwingDuration() const;
    virtual bool showsDurabilityInCreative() const = 0;
    virtual bool isWearableThroughLootTable(CompoundTag const *) const = 0;
    virtual bool canDestroyInCreative() const = 0;
    virtual bool isDestructive(int) const = 0;
    virtual bool isLiquidClipItem() const = 0;
    virtual bool shouldInteractionWithBlockBypassLiquid(Block const &) const = 0;
    virtual bool requiresInteract() const = 0;
    virtual std::string getHoverTextColor(ItemStackBase const &stack) const;
    virtual void appendFormattedHovertext(ItemStackBase const &, Level &, std::string &, bool) const = 0;
    virtual bool isValidRepairItem(ItemStackBase const &, ItemStackBase const &, BaseGameVersion const &) const = 0;
    virtual int getEnchantSlot() const = 0;
    virtual int getEnchantValue() const = 0;
    virtual int getArmorValue() const = 0;
    virtual int getToughnessValue() const = 0;
    virtual float getKnockbackResistanceValue() const;
    virtual std::optional<SharedTypes::Legacy::LevelSoundEvent> getAttackMissSound() const;
    virtual std::optional<SharedTypes::Legacy::LevelSoundEvent> getAttackHitSound() const;
    virtual std::optional<SharedTypes::Legacy::LevelSoundEvent> getAttackCriticalHitSound() const;
    virtual LevelSoundEvent getBreakSound() const = 0;
    virtual LevelSoundEvent getEquipSound() const;
    virtual bool isComplex() const = 0;
    virtual bool isValidAuxValue(int) const = 0;
    virtual int getDamageChance(int) const = 0;
    virtual float getViewDamping() const = 0;
    virtual bool uniqueAuxValues() const = 0;
    virtual bool isActorPlacerItem() const = 0;
    virtual ItemTintStrategy getTintStrategy() const;
    virtual mce::Color getColor(CompoundTag const *, ItemDescriptor const &) const = 0;
    virtual bool hasCustomColor(ItemStackBase const &) const = 0;
    virtual bool hasCustomColor(CompoundTag const *) const = 0;
    virtual void clearColor(ItemStackBase &) const = 0;
    virtual void setColor(ItemStackBase &, mce::Color const &) const = 0;
    // virtual mce::Color getBaseColor(ItemStack const &) const = 0;
    // virtual mce::Color getSecondaryColor(ItemStack const &) const = 0;
    virtual ActorDefinitionIdentifier getActorIdentifier(ItemStack const &) const = 0;
    virtual int buildIdAux(std::int16_t, CompoundTag const *) const = 0;
    virtual bool canUseOnSimTick() const = 0;
    virtual ItemStack &use(ItemStack &, Player &) const = 0;
    virtual bool canUseAsAttack() const = 0;
    virtual ItemStack &useAsAttack(ItemStack &itemStack, Player &) const = 0;
    virtual Actor *createProjectileActor(BlockSource &, ItemStack const &, Vec3 const &, Vec3 const &) const = 0;
    virtual bool dispense(BlockSource &, Container &, int slot, Vec3 const &, FacingID face) const = 0;
    virtual ItemUseMethod useTimeDepleted(ItemStack &, Level *, Player *) const = 0;
    virtual void releaseUsing(ItemStack &, Player *, int) const = 0;
    virtual float getDestroySpeed(ItemStackBase const &, Block const &) const = 0;
    virtual void hurtActor(ItemStack &, Actor &, Mob &) const = 0;
    virtual void hitActor(ItemStack &, Actor &, Mob &) const = 0;
    virtual void hitBlock(ItemStack &, Block const &, BlockPos const &, Mob &) const = 0;
    virtual std::string buildDescriptionName(ItemStackBase const &) const = 0;
    virtual Bedrock::Safety::RedactableString const buildRedactedDescriptionName(ItemStackBase const &) const = 0;
    virtual std::string buildDescriptionId(ItemDescriptor const &, CompoundTag const *) const = 0;
    virtual Bedrock::Safety::RedactableString getRedactedCustomName(const ItemStackBase &stack) const = 0;
    virtual bool hasCustomHoverName(const ItemStackBase &stack) const = 0;
    virtual std::string buildEffectDescriptionName(ItemStackBase const &) const = 0;
    ENDSTONE_HOOK virtual void readUserData(ItemStackBase &stack, IDataInput &input,
                                            ReadOnlyBinaryStream &underlying_stream) const;
    virtual void writeUserData(ItemStackBase const &, IDataOutput &) const = 0;
    virtual std::uint8_t getMaxStackSize(ItemDescriptor const &) const = 0;
    virtual bool inventoryTick(ItemStack &, Level &, Actor &, int, bool) const = 0;
    virtual void refreshedInContainer(ItemStackBase const &, Level &) const = 0;
    virtual HashedString const &getCooldownCategory() const = 0;
    virtual int getCooldownDuration() const = 0;
    virtual ItemCooldownType getCooldownType() const;
    virtual void fixupCommon(ItemStackBase &) const = 0;
    virtual void fixupCommon(ItemStackBase &, Level &) const = 0;
    virtual InHandUpdateType getInHandUpdateType(Player const &, ItemStack const &, ItemStack const &, bool,
                                                 bool) const = 0;
    virtual bool validFishInteraction(int) const = 0;
    virtual void enchantProjectile(ItemStackBase const &, Actor &) const = 0;
    virtual ActorLocation getEquipLocation() const = 0;
    virtual bool shouldEmitInUseGameEvents() const = 0;
    virtual bool useInterruptedByAttacking() const = 0;
    virtual bool hasSameRelevantUserData(ItemStackBase const &, ItemStackBase const &) const = 0;
    virtual void initClient(Json::Value const &, SemVersion const &, JsonBetaState, PackLoadContext &) = 0;
    virtual Item &setIconInfo(std::string const &, int) = 0;
    virtual ResolvedItemIconInfo getIconInfo(ItemStackBase const &, int, bool) const = 0;
    virtual std::string getInteractText(Player const &) const = 0;
    virtual int getAnimationFrameFor(Mob *, bool, ItemStack const *, bool) const = 0;
    virtual bool isEmissive(int) const = 0;
    virtual Brightness getLightEmission(int) const = 0;
    virtual bool canBeCharged() const = 0;
    virtual void playSoundIncrementally(ItemStack const &, Mob &) const = 0;
    virtual float getFurnaceXPmultiplier(ItemStackBase const *) const = 0;
    virtual bool calculatePlacePos(ItemStackBase &, Actor &, FacingID &, BlockPos &) const = 0;

private:
    virtual bool _checkUseOnPermissions(Actor &, ItemStackBase &, FacingID const &, BlockPos const &) const = 0;
    virtual bool _calculatePlacePos(ItemStackBase &, Actor &, FacingID &, BlockPos &) const = 0;
    virtual bool _shouldAutoCalculatePlacePos() const = 0;
    virtual InteractionResult _useOn(ItemStack &, Actor &, BlockPos pos, FacingID face, Vec3 const &) const = 0;

public:
    bool operator==(const Item &other) const;
    bool operator!=(const Item &other) const;
    [[nodiscard]] std::int16_t getId() const;
    [[nodiscard]] const std::string &getFullItemName() const;
    [[nodiscard]] const HashedString &getFullNameHash() const;
    [[nodiscard]] std::string getSerializedName() const;
    [[nodiscard]] const BaseGameVersion &getRequiredBaseGameVersion() const;
    [[nodiscard]] const WeakPtr<BlockType> &getBlockType() const;
    [[nodiscard]] bool hasTag(const ItemTag &tag) const;
    [[nodiscard]] const std::vector<ItemTag> &getTags() const;
    Item &setMinRequiredBaseGameVersion(const BaseGameVersion &base_game_version);
    ItemDescriptor buildDescriptor(std::int16_t, const CompoundTag *) const;
    [[nodiscard]] float getFurnaceBurnIntervalMultipler() const;
    [[nodiscard]] const std::string &getCreativeGroup() const;
    [[nodiscard]] CreativeItemCategory getCreativeCategory() const;
    [[nodiscard]] std::int16_t getDamageValue(const CompoundTag *tag) const;
    [[nodiscard]] bool hasDamageValue(const CompoundTag *tag) const;
    void removeDamageValue(ItemStackBase &item) const;
    void setDamageValue(ItemStackBase &item, std::int16_t damage) const;

public:
    ItemVersion item_parse_version;

protected:
    std::string texture_atlas_file_;
    int frame_count_;
    bool animates_in_toolbar_;
    bool is_mirrored_art_;
    std::uint8_t use_anim_;
    std::string hover_text_color_format_;
    int icon_frame_;
    int atlas_frame_;
    int atlas_total_frame_;
    std::string icon_name_;
    std::string atlas_name_;

public:
protected:
    std::uint8_t max_stack_size_;
    std::int16_t id_;
    std::string description_id_;
    HashedString raw_name_id_;
    std::string namespace_;
    HashedString full_name_;
    std::int16_t max_damage_;
    bool is_glint_ : 1;
    bool hand_equipped_ : 1;
    bool is_stacked_by_data_ : 1;
    bool requires_world_builder_ : 1;
    bool explodable_ : 1;
    bool fire_resistat_ : 1;
    bool should_despawn_ : 1;
    bool allow_offhand_ : 1;
    bool ignores_permission_ : 1;
    int max_use_duration_;
    BaseGameVersion min_required_base_game_version_;
    WeakPtr<BlockType> block_type_;
    CreativeItemCategory creative_category_;
    Item *crafting_remaining_item_;
    std::string creative_group_;  // +400
    float furnace_burn_interval_modifier_;
    float furnace_xp_multiplier_;
    ItemCommandVisibility is_hidden_in_command_;
    Rarity base_rarity_;
    Interactions::Mining::MineBlockItemEffectType mine_block_type_;
    std::unique_ptr<class FoodItemComponentLegacy> food_component_legacy_;
    std::unique_ptr<class SeedItemComponentLegacy> seed_component_legacy_;
    std::unique_ptr<class CameraItemComponentLegacy> camera_component_legacy_;
    std::vector<std::function<void()>> on_reset_bai_callbacks_;
    std::vector<ItemTag> tags_;
};
