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

#include <cstdint>
#include <string>
#include <string_view>

#include "bedrock/world/actor/actor_damage_source.h"
#include "bedrock/world/item/item_instance.h"

class Enchant {
public:
    enum class Type : std::uint8_t {
        Protection = 0,
        FireProtection = 1,
        FeatherFalling = 2,
        BlastProtection = 3,
        ProjectileProtection = 4,
        Thorns = 5,
        Respiration = 6,
        DepthStrider = 7,
        AquaAffinity = 8,
        Sharpness = 9,
        Smite = 10,
        BaneOfArthropods = 11,
        Knockback = 12,
        FireAspect = 13,
        Looting = 14,
        Efficiency = 15,
        SilkTouch = 16,
        Unbreaking = 17,
        Fortune = 18,
        Power = 19,
        Punch = 20,
        Flame = 21,
        Infinity = 22,
        LuckOfTheSea = 23,
        Lure = 24,
        FrostWalker = 25,
        Mending = 26,
        CurseOfBinding = 27,
        CurseOfVanishing = 28,
        Impaling = 29,
        Riptide = 30,
        Loyalty = 31,
        Channeling = 32,
        Multishot = 33,
        Piercing = 34,
        QuickCharge = 35,
        SoulSpeed = 36,
        SwiftSneak = 37,
        WindBurst = 38,
        Density = 39,
        Breach = 40,
        NumEnchantments = 41,
        InvalidEnchantment = 42,
    };

    enum class Frequency : int {
        Common = 30,
        Uncommon = 10,
        Rare = 3,
        VeryRare = 1,
    };

    enum Slot : std::uint32_t {  // NOLINTBEGIN
        NONE = 0U,
        All = ~NONE,
        ARMOR_HEAD = 1U << 0,
        ARMOR_TORSO = 1U << 1,
        ARMOR_FEET = 1U << 2,
        ARMOR_LEGS = 1U << 3,
        SWORD = 1U << 4,
        BOW = 1U << 5,
        HOE = 1U << 6,
        SHEARS = 1U << 7,
        FLINTSTEEL = 1U << 8,
        AXE = 1U << 9,
        PICKAXE = 1U << 10,
        SHOVEL = 1U << 11,
        FISHING_ROD = 1U << 12,
        CARROT_STICK = 1U << 13,
        ELYTRA = 1U << 14,
        SPEAR = 1U << 15,
        CROSSBOW = 1U << 16,
        SHIELD = 1U << 17,
        COSMETIC_HEAD = 1U << 18,
        COMPASS = 1U << 19,
        MUSHROOM_STICK = 1U << 20,
        BRUSH = 1U << 21,
        HEAVY_WEAPON = 1U << 22,

        G_ARMOR = ARMOR_HEAD | ARMOR_TORSO | ARMOR_FEET | ARMOR_LEGS,
        G_TOOL = HOE | SHEARS | FLINTSTEEL | SHIELD,
        G_DIGGING = HOE | AXE | PICKAXE | SHOVEL,
    };  // NOLINTEND

    enum Activation : unsigned int {  // NOLINTBEGIN
        EQUIPPED = 0,
        HELD = 1,
        SELF = 2,
        _num_activations = 3,
        _invalid = 4,
    };  // NOLINTEND

    enum CompatibilityID : unsigned int {  // NOLINTBEGIN
        NON_CONFLICT = 0,
        DAMAGE = 1,
        GATHERING = 2,
        PROTECTION = 3,
        FROSTSTRIDER = 4,
        MENDFINITY = 5,
        LOYALRIPTIDE = 6,
    };  // NOLINTEND

    enum class VillagerTrading : int {
        NotAvailable = 0,
        Available = 1,
    };

    Enchant(Type, Frequency, std::string_view, std::string_view, VillagerTrading, int, int);
    Enchant(Type, Frequency, std::string_view, std::string_view, std::string_view, VillagerTrading, int, int);

    virtual ~Enchant();
    [[nodiscard]] virtual bool isCompatibleWith(Type) const = 0;
    [[nodiscard]] virtual int getMinCost(int) const = 0;
    [[nodiscard]] virtual int getMaxCost(int) const = 0;
    [[nodiscard]] virtual int getMinLevel() const = 0;
    [[nodiscard]] virtual int getMaxLevel() const = 0;
    [[nodiscard]] virtual int getDamageProtection(int, const ActorDamageSource &) const = 0;
    [[nodiscard]] virtual float getAfterBreachArmorFraction(int, float) const = 0;
    [[nodiscard]] virtual float getDamageBonus(int, const Actor &, const Actor &) const = 0;
    virtual void doPostAttack(Actor &, Actor &, int) const = 0;
    virtual void doPostItemHurtActor(Actor &, Actor &, int) const = 0;
    virtual void doPostHurt(ItemInstance &, Actor &, Actor &, int) const = 0;
    [[nodiscard]] virtual bool isMeleeDamageEnchant() const = 0;
    [[nodiscard]] virtual bool isProtectionEnchant() const = 0;
    [[nodiscard]] virtual bool isTreasureOnly() const = 0;
    [[nodiscard]] virtual bool isDiscoverable() const = 0;

private:
    [[nodiscard]] virtual bool _isValidEnchantmentTypeForCategory(Type) const = 0;

public:
    Frequency getFrequency() const;
    bool canEnchant(const ItemInstance &, bool) const;
    bool canEnchant(int slot, bool allow_non_vanilla) const;
    bool canPrimaryEnchant(const ItemInstance &) const;
    bool canPrimaryEnchant(int) const;
    bool canSecondaryEnchant(const ItemInstance &) const;
    bool canSecondaryEnchant(int) const;
    std::string getDescription() const;
    std::string getDescriptionId() const;
    const HashedString &getStringId() const;
    const HashedString &getScriptStringId() const;
    Type getEnchantType() const;
    bool isAvailableInVillagerTrading() const;
    bool isAvailable() const;
    void setDisabled();
    bool isDisabled() const;

protected:
    const Type enchant_type_;
    const Frequency frequency_;
    const bool is_available_in_villager_trading_;
    const int primary_slots_;
    const int secondary_slots_;
    const int compatibility_;

private:
    const std::string description_;
    const HashedString string_id_;
    const HashedString script_string_id_;
    bool is_disabled_;
};
BEDROCK_STATIC_ASSERT_SIZE(Enchant, 168, 144);
