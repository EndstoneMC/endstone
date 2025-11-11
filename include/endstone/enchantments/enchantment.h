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

#include "endstone/detail.h"
#include "endstone/identifier.h"
#include "endstone/registry.h"
#include "endstone/server.h"

namespace endstone {
class ItemStack;

class Enchantment {
public:
    ENDSTONE_REGISTRY_TYPE(Enchantment)
    using EnchantmentId = Identifier<Enchantment>;
    /**
     * @brief Provides protection against environmental damage
     */
    static constexpr auto Protection = EnchantmentId::minecraft("protection");

    /**
     * @brief Provides protection against fire damage
     */
    static constexpr auto FireProtection = EnchantmentId::minecraft("fire_protection");

    /**
     * @brief Provides protection against fall damage
     */
    static constexpr auto FeatherFalling = EnchantmentId::minecraft("feather_falling");

    /**
     * @brief Provides protection against explosive damage
     */
    static constexpr auto BlastProtection = EnchantmentId::minecraft("blast_protection");

    /**
     * @brief Provides protection against projectile damage
     */
    static constexpr auto ProjectileProtection = EnchantmentId::minecraft("projectile_protection");

    /**
     * @brief Damages the attacker
     */
    static constexpr auto Thorns = EnchantmentId::minecraft("thorns");

    /**
     * @brief Decreases the rate of air loss whilst underwater
     */
    static constexpr auto Respiration = EnchantmentId::minecraft("respiration");

    /**
     * @brief Increases walking speed while in water
     */
    static constexpr auto DepthStrider = EnchantmentId::minecraft("depth_strider");

    /**
     * @brief Increases the speed at which a player may mine underwater
     */
    static constexpr auto AquaAffinity = EnchantmentId::minecraft("aqua_affinity");

    /**
     * @brief Increases damage against all targets
     */
    static constexpr auto Sharpness = EnchantmentId::minecraft("sharpness");

    /**
     * @brief Increases damage against undead targets
     */
    static constexpr auto Smite = EnchantmentId::minecraft("smite");

    /**
     * @brief Increases damage against arthropod targets
     */
    static constexpr auto BaneOfArthropods = EnchantmentId::minecraft("bane_of_arthropods");

    /**
     * @brief All damage to other targets will knock them back when hit
     */
    static constexpr auto Knockback = EnchantmentId::minecraft("knockback");

    /**
     * @brief When attacking a target, has a chance to set them on fire
     */
    static constexpr auto FireAspect = EnchantmentId::minecraft("fire_aspect");

    /**
     * @brief Provides a chance of gaining extra loot when killing monsters
     */
    static constexpr auto Looting = EnchantmentId::minecraft("looting");

    /**
     * @brief Increases the rate at which you mine/dig
     */
    static constexpr auto Efficiency = EnchantmentId::minecraft("efficiency");

    /**
     * @brief Allows blocks to drop themselves instead of fragments (for example, stone instead of cobblestone)
     */
    static constexpr auto SilkTouch = EnchantmentId::minecraft("silk_touch");

    /**
     * @brief Decreases the rate at which a tool looses durability
     */
    static constexpr auto Unbreaking = EnchantmentId::minecraft("unbreaking");

    /**
     * @brief Provides a chance of gaining extra loot when destroying blocks
     */
    static constexpr auto Fortune = EnchantmentId::minecraft("fortune");

    /**
     * @brief Provides extra damage when shooting arrows from bows
     */
    static constexpr auto Power = EnchantmentId::minecraft("power");

    /**
     * @brief Provides a knockback when an entity is hit by an arrow from a bow
     */
    static constexpr auto Punch = EnchantmentId::minecraft("punch");

    /**
     * @brief Sets entities on fire when hit by arrows shot from a bow
     */
    static constexpr auto Flame = EnchantmentId::minecraft("flame");

    /**
     * @brief Provides infinite arrows when shooting a bow
     */
    static constexpr auto Infinity = EnchantmentId::minecraft("infinity");

    /**
     * @brief Decreases odds of catching worthless junk
     */
    static constexpr auto LuckOfTheSea = EnchantmentId::minecraft("luck_of_the_sea");

    /**
     * @brief Increases rate of fish biting your hook
     */
    static constexpr auto Lure = EnchantmentId::minecraft("lure");

    /**
     * @brief Freezes any still water adjacent to ice / frost which player is walking on
     */
    static constexpr auto FrostWalker = EnchantmentId::minecraft("frost_walker");

    /**
     * @brief Allows mending the item using experience orbs
     */
    static constexpr auto Mending = EnchantmentId::minecraft("mending");

    /**
     * @brief Item cannot be removed
     */
    static constexpr auto CurseOfBinding = EnchantmentId::minecraft("binding");

    /**
     * @brief Item disappears instead of dropping
     */
    static constexpr auto CurseOfVanishing = EnchantmentId::minecraft("vanishing");

    /**
     * @brief Deals more damage to mobs that live in the ocean
     */
    static constexpr auto Impaling = EnchantmentId::minecraft("impaling");

    /**
     * @brief When it is rainy, launches the player in the direction their trident is thrown
     */
    static constexpr auto Riptide = EnchantmentId::minecraft("riptide");

    /**
     * @brief Causes a thrown trident to return to the player who threw it
     */
    static constexpr auto Loyalty = EnchantmentId::minecraft("loyalty");

    /**
     * @brief Strikes lightning when a mob is hit with a trident if conditions are stormy
     */
    static constexpr auto Channeling = EnchantmentId::minecraft("channeling");

    /**
     * @brief Shoot multiple arrows from crossbows
     */
    static constexpr auto Multishot = EnchantmentId::minecraft("multishot");

    /**
     * @brief Crossbow projectiles pierce entities
     */
    static constexpr auto Piercing = EnchantmentId::minecraft("piercing");

    /**
     * @brief Charges crossbows quickly
     */
    static constexpr auto QuickCharge = EnchantmentId::minecraft("quick_charge");

    /**
     * @brief Walk quicker on soul blocks
     */
    static constexpr auto SoulSpeed = EnchantmentId::minecraft("soul_speed");

    /**
     * @brief Walk quicker while sneaking
     */
    static constexpr auto SwiftSneak = EnchantmentId::minecraft("swift_sneak");

    /**
     * @brief Emits wind burst upon hitting enemy
     */
    static constexpr auto WindBurst = EnchantmentId::minecraft("wind_burst");

    /**
     * @brief Increases fall damage of maces
     */
    static constexpr auto Density = EnchantmentId::minecraft("density");

    /**
     * @brief Reduces armor effectiveness against maces
     */
    static constexpr auto Breach = EnchantmentId::minecraft("breach");

    virtual ~Enchantment() = default;

    /**
     * @brief Return the identifier for this enchantment.
     *
     * @return this enchantment's id
     */
    [[nodiscard]] virtual EnchantmentId getId() const = 0;

    /**
     * @brief Get the translation key, suitable for use in a translation component.
     *
     * @return the translation key
     */
    [[nodiscard]] virtual std::string getTranslationKey() const = 0;

    /**
     * @brief Gets the maximum level that this Enchantment may become.
     *
     * @return Maximum level of the Enchantment
     */
    [[nodiscard]] virtual int getMaxLevel() const = 0;

    /**
     * @brief Gets the level that this Enchantment should start at (also known as minimum level).
     *
     * @return Starting level of the Enchantment
     */
    [[nodiscard]] virtual int getStartLevel() const = 0;

    /**
     * @brief Check if this enchantment conflicts with another enchantment.
     *
     * @param other The enchantment to check against
     * @return True if there is a conflict.
     */
    [[nodiscard]] virtual bool conflictsWith(const Enchantment &other) const = 0;

    /**
     * @brief Checks if this Enchantment may be applied to the given ItemStack.
     *
     * This does not check if it conflicts with any enchantments already applied to the item.
     *
     * @param item Item to test
     * @return True if the enchantment may be applied, otherwise False
     */
    [[nodiscard]] virtual bool canEnchantItem(const ItemStack &item) const = 0;
};
}  // namespace endstone
