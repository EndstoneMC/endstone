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

#include "bedrock/common/game_version.h"
#include "bedrock/deps/jsoncpp/value.h"
#include "bedrock/forward.h"
#include "bedrock/world/actor/actor_location.h"
#include "bedrock/world/item/item_descriptor.h"
#include "bedrock/world/item/item_tag.h"
#include "bedrock/world/level/block/block_legacy.h"

using BlockShape = std::int64_t;
using InteractionResult = std::int64_t;

class ItemStackBase;
class Level;
class Mob;

class Item {
public:
    virtual ~Item() = 0;
    virtual bool initServer(Json::Value const &, SemVersion const &, bool, Experiments const &) = 0;
    virtual void tearDown() = 0;
    virtual Item &setDescriptionId(std::string const &) = 0;
    virtual std::string const &getDescriptionId() const = 0;
    virtual int getMaxUseDuration(ItemStack const *) const = 0;
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
    virtual float getAttackDamageBonus(Actor const &, float) const = 0;
    virtual bool isHandEquipped() const = 0;
    virtual bool isGlint(ItemStackBase const &) const = 0;
    virtual bool isPattern() const = 0;
    virtual int getPatternIndex() const = 0;
    virtual bool showsDurabilityInCreative() const = 0;
    virtual bool isWearableThroughLootTable(CompoundTag const *) const = 0;
    virtual bool canDestroyInCreative() const = 0;
    virtual bool isDestructive(int) const = 0;
    virtual bool isLiquidClipItem() const = 0;
    virtual bool shouldInteractionWithBlockBypassLiquid(Block const &) const = 0;
    virtual bool requiresInteract() const = 0;
    virtual void appendFormattedHovertext(ItemStackBase const &, Level &, std::string &, bool) const = 0;
    virtual bool isValidRepairItem(ItemStackBase const &, ItemStackBase const &, BaseGameVersion const &) const = 0;
    virtual int getEnchantSlot() const = 0;
    virtual int getEnchantValue() const = 0;
    virtual int getArmorValue() const = 0;
    virtual int getToughnessValue() const = 0;
    virtual Puv::Legacy::LevelSoundEvent getBreakSound() const = 0;
    virtual bool isComplex() const = 0;
    virtual bool isValidAuxValue(int) const = 0;
    virtual int getDamageChance(int) const = 0;
    virtual float getViewDamping() const = 0;
    virtual bool uniqueAuxValues() const = 0;
    virtual bool isActorPlacerItem() const = 0;
    virtual bool isMultiColorTinted(ItemStack const &) const = 0;
    virtual mce::Color getColor(CompoundTag const *, ItemDescriptor const &) const = 0;
    virtual bool hasCustomColor(ItemStackBase const &) const = 0;
    virtual bool hasCustomColor(CompoundTag const *) const = 0;
    virtual void clearColor(ItemStackBase &) const = 0;
    virtual void setColor(ItemStackBase &, mce::Color const &) const = 0;
    virtual mce::Color getBaseColor(ItemStack const &) const = 0;
    virtual mce::Color getSecondaryColor(ItemStack const &) const = 0;
    virtual ActorDefinitionIdentifier getActorIdentifier(ItemStack const &) const = 0;
    virtual int buildIdAux(std::int16_t, CompoundTag const *) const = 0;
    virtual bool canUseOnSimTick() const = 0;
    virtual ItemStack &use(ItemStack &, Player &) const = 0;
    virtual Actor *createProjectileActor(BlockSource &, ItemStack const &, Vec3 const &, Vec3 const &) const = 0;
    virtual bool dispense(BlockSource &, Container &, int slot, Vec3 const &, FacingID face) const = 0;
    virtual ItemUseMethod useTimeDepleted(ItemStack &, Level *, Player *) const = 0;
    virtual void releaseUsing(ItemStack &, Player *, int) const = 0;
    virtual float getDestroySpeed(ItemStackBase const &, Block const &) const = 0;
    virtual void hurtActor(ItemStack &, Actor &, Mob &) const = 0;
    virtual void hitActor(ItemStack &, Actor &, Mob &) const = 0;
    virtual void hitBlock(ItemStack &, Block const &, BlockPos const &, Mob &) const = 0;
    virtual bool mineBlock(ItemStack &, Block const &, int x, int y, int z, Actor *) const = 0;
    virtual std::string buildDescriptionName(ItemStackBase const &) const = 0;
    virtual std::string buildDescriptionId(ItemDescriptor const &, CompoundTag const *) const = 0;
    virtual std::string buildEffectDescriptionName(ItemStackBase const &) const = 0;
    virtual void readUserData(ItemStackBase &, IDataInput &, ReadOnlyBinaryStream &) const = 0;
    virtual void writeUserData(ItemStackBase const &, IDataOutput &) const = 0;
    virtual std::uint8_t getMaxStackSize(ItemDescriptor const &) const = 0;
    virtual bool inventoryTick(ItemStack &, Level &, Actor &, int, bool) const = 0;
    virtual void refreshedInContainer(ItemStackBase const &, Level &) const = 0;
    virtual HashedString const &getCooldownType() const = 0;
    virtual int getCooldownTime() const = 0;
    virtual void fixupCommon(ItemStackBase &) const = 0;
    virtual void fixupCommon(ItemStackBase &, Level &) const = 0;
    virtual InHandUpdateType getInHandUpdateType(Player const &, ItemStack const &, ItemStack const &, bool,
                                                 bool) const = 0;
    virtual bool validFishInteraction(int) const = 0;
    virtual void enchantProjectile(ItemStackBase const &, Actor &) const = 0;
    virtual ActorLocation getEquipLocation() const = 0;
    virtual Puv::Legacy::LevelSoundEvent getEquipSound() const = 0;
    virtual bool shouldSendInteractionGameEvents() const = 0;
    virtual bool useInterruptedByAttacking() const = 0;
    virtual bool hasSameRelevantUserData(ItemStackBase const &, ItemStackBase const &) const = 0;
    virtual void initClient(Json::Value const &, SemVersion const &, bool, Experiments const &) = 0;
    virtual Item &setIconInfo(std::string const &, int) = 0;
    virtual ResolvedItemIconInfo getIconInfo(ItemStackBase const &, int, bool) const = 0;
    virtual std::string getInteractText(Player const &) const = 0;
    virtual int getAnimationFrameFor(Mob *, bool, ItemStack const *, bool) const = 0;
    virtual bool isEmissive(int) const = 0;
    virtual Brightness getLightEmission(int) const = 0;
    virtual int getIconYOffset() const = 0;
    virtual bool canBeCharged() const = 0;
    virtual void playSoundIncrementally(ItemStack const &, Mob &) const = 0;
    virtual float getFurnaceXPmultiplier(ItemStackBase const &) const = 0;
    virtual std::string getAuxValuesDescription() const = 0;
    virtual bool calculatePlacePos(ItemStackBase &, Actor &, FacingID &, BlockPos &) const = 0;
    virtual bool _checkUseOnPermissions(Actor &, ItemStackBase &, FacingID const &, BlockPos const &) const = 0;
    virtual bool _calculatePlacePos(ItemStackBase &, Actor &, FacingID &, BlockPos &) const = 0;
    virtual bool _shouldAutoCalculatePlacePos() const = 0;
    virtual InteractionResult _useOn(ItemStack &, Actor &, BlockPos pos, FacingID face, Vec3 const &) const = 0;

    [[nodiscard]] std::int16_t getId() const
    {
        return id_;
    }

    [[nodiscard]] const std::string &getFullItemName() const
    {
        return full_name_.getString();
    }

    [[nodiscard]] const std::vector<ItemTag> &getTags() const
    {
        return tags_;
    }

    [[nodiscard]] const WeakPtr<BlockLegacy> &getLegacyBlock() const
    {
        return legacy_block_;
    }

private:
    std::string texture_atlas_file_;                  // +8
    int frame_count_;                                 // +40
    std::uint16_t use_anim_;                          // +44
    bool animates_in_toolbar_;                        // +46
    std::string hover_text_color_format_;             // +48
    std::uint64_t unknown_;                           // +80
    int atlas_total_frame_;                           // +88
    std::string icon_name_;                           // +96
    std::string atlas_name_;                          // +128
    std::int8_t max_stack_size_;                      // +160
    std::int16_t id_;                                 // +162
    std::string description_id_;                      // +168
    HashedString raw_name_id_;                        // +200
    std::string namespace_;                           // +248
    HashedString full_name_;                          // +280
    std::int16_t max_damage_;                         // +328
    std::int16_t unknown2_;                           // +330
    int max_use_duration_;                            // +332
    BaseGameVersion min_required_base_game_version_;  // +336
    WeakPtr<BlockLegacy> legacy_block_;               // +456
    CreativeItemCategory creative_category_;          // +464
    Item *crafting_remaining_item_;                   // +472
    std::string creative_group_;                      // +480
    std::uint64_t unknown3_;                          // +512
    int unknown4_;                                    // +520
    int unknown5_;                                    // +524
    std::vector<void *> unknown6_;                    // +528
    std::vector<void *> unknown7_;                    // +552
    std::vector<ItemTag> tags_;                       // +576
};
