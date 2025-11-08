

# File enchantment.h

[**File List**](files.md) **>** [**enchantments**](dir_5154bf3e53eefb0e1f350f5612107967.md) **>** [**enchantment.h**](enchantment_8h.md)

[Go to the documentation of this file](enchantment_8h.md)


```C++
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
    ENDSTONE_REGISTRY_TYPE(Enchantment)

    
    inline static const std::string Protection = getEnchantment("protection");

    inline static const std::string FireProtection = getEnchantment("fire_protection");

    inline static const std::string FeatherFalling = getEnchantment("feather_falling");

    inline static const std::string BlastProtection = getEnchantment("blast_protection");

    inline static const std::string ProjectileProtection = getEnchantment("projectile_protection");

    inline static const std::string Thorns = getEnchantment("thorns");

    inline static const std::string Respiration = getEnchantment("respiration");

    inline static const std::string DepthStrider = getEnchantment("depth_strider");

    inline static const std::string AquaAffinity = getEnchantment("aqua_affinity");

    inline static const std::string Sharpness = getEnchantment("sharpness");

    inline static const std::string Smite = getEnchantment("smite");

    inline static const std::string BaneOfArthropods = getEnchantment("bane_of_arthropods");

    inline static const std::string Knockback = getEnchantment("knockback");

    inline static const std::string FireAspect = getEnchantment("fire_aspect");

    inline static const std::string Looting = getEnchantment("looting");

    inline static const std::string Efficiency = getEnchantment("efficiency");

    inline static const std::string SilkTouch = getEnchantment("silk_touch");

    inline static const std::string Unbreaking = getEnchantment("unbreaking");

    inline static const std::string Fortune = getEnchantment("fortune");

    inline static const std::string Power = getEnchantment("power");

    inline static const std::string Punch = getEnchantment("punch");

    inline static const std::string Flame = getEnchantment("flame");

    inline static const std::string Infinity = getEnchantment("infinity");

    inline static const std::string LuckOfTheSea = getEnchantment("luck_of_the_sea");

    inline static const std::string Lure = getEnchantment("lure");

    inline static const std::string FrostWalker = getEnchantment("frost_walker");

    inline static const std::string Mending = getEnchantment("mending");

    inline static const std::string CurseOfBinding = getEnchantment("binding");

    inline static const std::string CurseOfVanishing = getEnchantment("vanishing");

    inline static const std::string Impaling = getEnchantment("impaling");

    inline static const std::string Riptide = getEnchantment("riptide");

    inline static const std::string Loyalty = getEnchantment("loyalty");

    inline static const std::string Channeling = getEnchantment("channeling");

    inline static const std::string Multishot = getEnchantment("multishot");

    inline static const std::string Piercing = getEnchantment("piercing");

    inline static const std::string QuickCharge = getEnchantment("quick_charge");

    inline static const std::string SoulSpeed = getEnchantment("soul_speed");

    inline static const std::string SwiftSneak = getEnchantment("swift_sneak");

    inline static const std::string WindBurst = getEnchantment("wind_burst");

    inline static const std::string Density = getEnchantment("density");

    inline static const std::string Breach = getEnchantment("breach");

    virtual ~Enchantment() = default;

    [[nodiscard]] virtual std::string getId() const = 0;

    [[nodiscard]] virtual std::string getTranslationKey() const = 0;

    [[nodiscard]] virtual int getMaxLevel() const = 0;

    [[nodiscard]] virtual int getStartLevel() const = 0;

    [[nodiscard]] virtual bool conflictsWith(const Enchantment &other) const = 0;

    [[nodiscard]] virtual bool canEnchantItem(const ItemStack &item) const = 0;
};
}  // namespace endstone
```


