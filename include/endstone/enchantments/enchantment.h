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

#include "endstone/detail/endstone.h"
#include "endstone/namespaced_key.h"
#include "endstone/registry.h"

namespace endstone {
class ItemStack;

class Enchantment {
    static std::string getEnchantment(const std::string &key)
    {
        return NamespacedKey::minecraft(key);
    }

public:
    /**
     * @brief Provides protection against environmental damage
     */
    inline static const std::string Protection = getEnchantment("protection");

    /**
     * @brief Provides protection against fire damage
     */
    inline static const std::string FireProtection = getEnchantment("fire_protection");

    /**
     * @brief Provides protection against fall damage
     */
    inline static const std::string FeatherFalling = getEnchantment("feather_falling");

    /**
     * @brief Provides protection against explosive damage
     */
    inline static const std::string BlastProtection = getEnchantment("blast_protection");

    /**
     * @brief Provides protection against projectile damage
     */
    inline static const std::string ProjectileProtection = getEnchantment("projectile_protection");

    /**
     * @brief Damages the attacker
     */
    inline static const std::string Thorns = getEnchantment("thorns");

    /**
     * @brief Decreases the rate of air loss whilst underwater
     */
    inline static const std::string Respiration = getEnchantment("respiration");

    /**
     * @brief Increases walking speed while in water
     */
    inline static const std::string DepthStrider = getEnchantment("depth_strider");

    /**
     * @brief Increases the speed at which a player may mine underwater
     */
    inline static const std::string AquaAffinity = getEnchantment("aqua_affinity");

    /**
     * @brief Increases damage against all targets
     */
    inline static const std::string Sharpness = getEnchantment("sharpness");

    /**
     * @brief Increases damage against undead targets
     */
    inline static const std::string Smite = getEnchantment("smite");

    /**
     * @brief Increases damage against arthropod targets
     */
    inline static const std::string BaneOfArthropods = getEnchantment("bane_of_arthropods");

    /**
     * @brief All damage to other targets will knock them back when hit
     */
    inline static const std::string Knockback = getEnchantment("knockback");

    /**
     * @brief When attacking a target, has a chance to set them on fire
     */
    inline static const std::string FireAspect = getEnchantment("fire_aspect");

    /**
     * @brief Provides a chance of gaining extra loot when killing monsters
     */
    inline static const std::string Looting = getEnchantment("looting");

    /**
     * @brief Increases the rate at which you mine/dig
     */
    inline static const std::string Efficiency = getEnchantment("efficiency");

    /**
     * @brief Allows blocks to drop themselves instead of fragments (for example, stone instead of cobblestone)
     */
    inline static const std::string SilkTouch = getEnchantment("silk_touch");

    /**
     * @brief Decreases the rate at which a tool looses durability
     */
    inline static const std::string Unbreaking = getEnchantment("unbreaking");

    /**
     * @brief Provides a chance of gaining extra loot when destroying blocks
     */
    inline static const std::string Fortune = getEnchantment("fortune");

    /**
     * @brief Provides extra damage when shooting arrows from bows
     */
    inline static const std::string Power = getEnchantment("power");

    /**
     * @brief Provides a knockback when an entity is hit by an arrow from a bow
     */
    inline static const std::string Punch = getEnchantment("punch");

    /**
     * @brief Sets entities on fire when hit by arrows shot from a bow
     */
    inline static const std::string Flame = getEnchantment("flame");

    /**
     * @brief Provides infinite arrows when shooting a bow
     */
    inline static const std::string Infinity = getEnchantment("infinity");

    /**
     * @brief Decreases odds of catching worthless junk
     */
    inline static const std::string LuckOfTheSea = getEnchantment("luck_of_the_sea");

    /**
     * @brief Increases rate of fish biting your hook
     */
    inline static const std::string Lure = getEnchantment("lure");

    /**
     * @brief Freezes any still water adjacent to ice / frost which player is walking on
     */
    inline static const std::string FrostWalker = getEnchantment("frost_walker");

    /**
     * @brief Allows mending the item using experience orbs
     */
    inline static const std::string Mending = getEnchantment("mending");

    /**
     * @brief Item cannot be removed
     */
    inline static const std::string CurseOfBinding = getEnchantment("binding");

    /**
     * @brief Item disappears instead of dropping
     */
    inline static const std::string CurseOfVanishing = getEnchantment("vanishing");

    /**
     * @brief Deals more damage to mobs that live in the ocean
     */
    inline static const std::string Impaling = getEnchantment("impaling");

    /**
     * @brief When it is rainy, launches the player in the direction their trident is thrown
     */
    inline static const std::string Riptide = getEnchantment("riptide");

    /**
     * @brief Causes a thrown trident to return to the player who threw it
     */
    inline static const std::string Loyalty = getEnchantment("loyalty");

    /**
     * @brief Strikes lightning when a mob is hit with a trident if conditions are stormy
     */
    inline static const std::string Channeling = getEnchantment("channeling");

    /**
     * @brief Shoot multiple arrows from crossbows
     */
    inline static const std::string Multishot = getEnchantment("multishot");

    /**
     * @brief Crossbow projectiles pierce entities
     */
    inline static const std::string Piercing = getEnchantment("piercing");

    /**
     * @brief Charges crossbows quickly
     */
    inline static const std::string QuickCharge = getEnchantment("quick_charge");

    /**
     * @brief Walk quicker on soul blocks
     */
    inline static const std::string SoulSpeed = getEnchantment("soul_speed");

    /**
     * @brief Walk quicker while sneaking
     */
    inline static const std::string SwiftSneak = getEnchantment("swift_sneak");

    /**
     * @brief Emits wind burst upon hitting enemy
     */
    inline static const std::string WindBurst = getEnchantment("wind_burst");

    /**
     * @brief Increases fall damage of maces
     */
    inline static const std::string Density = getEnchantment("density");

    /**
     * @brief Reduces armor effectiveness against maces
     */
    inline static const std::string Breach = getEnchantment("breach");

    virtual ~Enchantment() = default;

    /**
     * @brief Return the identifier for this enchantment.
     *
     * @return this enchantment's id
     */
    [[nodiscard]] virtual std::string getId() const = 0;

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

    /**
     * @brief Attempts to get the Enchantment with the given name.
     *
     * @note This is a normal lookup, names must be the precise name.
     *
     * @param name Name of the Enchantment to get
     *
     * @return ItemType if found, or nullptr
     */
    static const Enchantment *get(const std::string &name)
    {
        return Endstone::getServer().getEnchantmentRegistry().get(name);
    }
};
}  // namespace endstone
