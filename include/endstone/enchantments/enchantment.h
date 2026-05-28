// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include "endstone/registry.h"

namespace endstone {
class ItemStack;
using EnchantmentId = Identifier<class Enchantment>;
class Enchantment : public Registry<Enchantment>::Type {
public:
    /**
     * Provides protection against environmental damage.
     */
    static constexpr auto Protection = EnchantmentId::minecraft("protection");

    /**
     * Provides protection against fire damage.
     */
    static constexpr auto FireProtection = EnchantmentId::minecraft("fire_protection");

    /**
     * Provides protection against fall damage.
     */
    static constexpr auto FeatherFalling = EnchantmentId::minecraft("feather_falling");

    /**
     * Provides protection against explosive damage.
     */
    static constexpr auto BlastProtection = EnchantmentId::minecraft("blast_protection");

    /**
     * Provides protection against projectile damage.
     */
    static constexpr auto ProjectileProtection = EnchantmentId::minecraft("projectile_protection");

    /**
     * Damages the attacker.
     */
    static constexpr auto Thorns = EnchantmentId::minecraft("thorns");

    /**
     * Decreases the rate of air loss whilst underwater.
     */
    static constexpr auto Respiration = EnchantmentId::minecraft("respiration");

    /**
     * Increases walking speed while in water.
     */
    static constexpr auto DepthStrider = EnchantmentId::minecraft("depth_strider");

    /**
     * Increases the speed at which a player may mine underwater.
     */
    static constexpr auto AquaAffinity = EnchantmentId::minecraft("aqua_affinity");

    /**
     * Increases damage against all targets.
     */
    static constexpr auto Sharpness = EnchantmentId::minecraft("sharpness");

    /**
     * Increases damage against undead targets.
     */
    static constexpr auto Smite = EnchantmentId::minecraft("smite");

    /**
     * Increases damage against arthropod targets.
     */
    static constexpr auto BaneOfArthropods = EnchantmentId::minecraft("bane_of_arthropods");

    /**
     * All damage to other targets will knock them back when hit.
     */
    static constexpr auto Knockback = EnchantmentId::minecraft("knockback");

    /**
     * When attacking a target, has a chance to set them on fire.
     */
    static constexpr auto FireAspect = EnchantmentId::minecraft("fire_aspect");

    /**
     * Provides a chance of gaining extra loot when killing monsters.
     */
    static constexpr auto Looting = EnchantmentId::minecraft("looting");

    /**
     * Increases the rate at which you mine/dig.
     */
    static constexpr auto Efficiency = EnchantmentId::minecraft("efficiency");

    /**
     * Allows blocks to drop themselves instead of fragments (for example, stone instead of cobblestone).
     */
    static constexpr auto SilkTouch = EnchantmentId::minecraft("silk_touch");

    /**
     * Decreases the rate at which a tool looses durability.
     */
    static constexpr auto Unbreaking = EnchantmentId::minecraft("unbreaking");

    /**
     * Provides a chance of gaining extra loot when destroying blocks.
     */
    static constexpr auto Fortune = EnchantmentId::minecraft("fortune");

    /**
     * Provides extra damage when shooting arrows from bows.
     */
    static constexpr auto Power = EnchantmentId::minecraft("power");

    /**
     * Provides a knockback when an entity is hit by an arrow from a bow.
     */
    static constexpr auto Punch = EnchantmentId::minecraft("punch");

    /**
     * Sets entities on fire when hit by arrows shot from a bow.
     */
    static constexpr auto Flame = EnchantmentId::minecraft("flame");

    /**
     * Provides infinite arrows when shooting a bow.
     */
    static constexpr auto Infinity = EnchantmentId::minecraft("infinity");

    /**
     * Decreases odds of catching worthless junk.
     */
    static constexpr auto LuckOfTheSea = EnchantmentId::minecraft("luck_of_the_sea");

    /**
     * Increases rate of fish biting your hook.
     */
    static constexpr auto Lure = EnchantmentId::minecraft("lure");

    /**
     * Freezes any still water adjacent to ice / frost which player is walking on.
     */
    static constexpr auto FrostWalker = EnchantmentId::minecraft("frost_walker");

    /**
     * Allows mending the item using experience orbs.
     */
    static constexpr auto Mending = EnchantmentId::minecraft("mending");

    /**
     * Item cannot be removed.
     */
    static constexpr auto CurseOfBinding = EnchantmentId::minecraft("binding");

    /**
     * Item disappears instead of dropping.
     */
    static constexpr auto CurseOfVanishing = EnchantmentId::minecraft("vanishing");

    /**
     * Deals more damage to mobs that live in the ocean.
     */
    static constexpr auto Impaling = EnchantmentId::minecraft("impaling");

    /**
     * When it is rainy, launches the player in the direction their trident is thrown.
     */
    static constexpr auto Riptide = EnchantmentId::minecraft("riptide");

    /**
     * Causes a thrown trident to return to the player who threw it.
     */
    static constexpr auto Loyalty = EnchantmentId::minecraft("loyalty");

    /**
     * Strikes lightning when a mob is hit with a trident if conditions are stormy.
     */
    static constexpr auto Channeling = EnchantmentId::minecraft("channeling");

    /**
     * Shoot multiple arrows from crossbows.
     */
    static constexpr auto Multishot = EnchantmentId::minecraft("multishot");

    /**
     * Crossbow projectiles pierce entities.
     */
    static constexpr auto Piercing = EnchantmentId::minecraft("piercing");

    /**
     * Charges crossbows quickly.
     */
    static constexpr auto QuickCharge = EnchantmentId::minecraft("quick_charge");

    /**
     * Walk quicker on soul blocks.
     */
    static constexpr auto SoulSpeed = EnchantmentId::minecraft("soul_speed");

    /**
     * Walk quicker while sneaking.
     */
    static constexpr auto SwiftSneak = EnchantmentId::minecraft("swift_sneak");

    /**
     * Emits wind burst upon hitting enemy.
     */
    static constexpr auto WindBurst = EnchantmentId::minecraft("wind_burst");

    /**
     * Increases fall damage of maces.
     */
    static constexpr auto Density = EnchantmentId::minecraft("density");

    /**
     * Reduces armor effectiveness against maces.
     */
    static constexpr auto Breach = EnchantmentId::minecraft("breach");

    /**
     * Applied to spears, causes jab attacks to propel the wielder forward horizontally.
     */
    static constexpr auto Lunge = EnchantmentId::minecraft("lunge");

    /**
     * Gets the maximum level that this Enchantment may become.
     *
     * @return Maximum level of the Enchantment
     */
    [[nodiscard]] virtual int getMaxLevel() const = 0;

    /**
     * Gets the level that this Enchantment should start at (also known as minimum level).
     *
     * @return Starting level of the Enchantment
     */
    [[nodiscard]] virtual int getStartLevel() const = 0;

    /**
     * Check if this enchantment conflicts with another enchantment.
     *
     * @param other The enchantment to check against
     * @return True if there is a conflict.
     */
    [[nodiscard]] virtual bool conflictsWith(const Enchantment &other) const = 0;

    /**
     * Checks if this Enchantment may be applied to the given ItemStack.
     *
     * This does not check if it conflicts with any enchantments already applied to the item.
     *
     * @param item Item to test
     * @return True if the enchantment may be applied, otherwise False
     */
    [[nodiscard]] virtual bool canEnchantItem(const ItemStack &item) const = 0;
};
}  // namespace endstone
