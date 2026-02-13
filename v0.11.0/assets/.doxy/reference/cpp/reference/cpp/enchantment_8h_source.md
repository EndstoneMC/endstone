

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

#include "endstone/detail.h"
#include "endstone/identifier.h"
#include "endstone/registry.h"
#include "endstone/server.h"

namespace endstone {
class ItemStack;
using EnchantmentId = Identifier<class Enchantment>;
class Enchantment {
public:
    ENDSTONE_REGISTRY_TYPE(Enchantment)
    static constexpr auto Protection = EnchantmentId::minecraft("protection");

    static constexpr auto FireProtection = EnchantmentId::minecraft("fire_protection");

    static constexpr auto FeatherFalling = EnchantmentId::minecraft("feather_falling");

    static constexpr auto BlastProtection = EnchantmentId::minecraft("blast_protection");

    static constexpr auto ProjectileProtection = EnchantmentId::minecraft("projectile_protection");

    static constexpr auto Thorns = EnchantmentId::minecraft("thorns");

    static constexpr auto Respiration = EnchantmentId::minecraft("respiration");

    static constexpr auto DepthStrider = EnchantmentId::minecraft("depth_strider");

    static constexpr auto AquaAffinity = EnchantmentId::minecraft("aqua_affinity");

    static constexpr auto Sharpness = EnchantmentId::minecraft("sharpness");

    static constexpr auto Smite = EnchantmentId::minecraft("smite");

    static constexpr auto BaneOfArthropods = EnchantmentId::minecraft("bane_of_arthropods");

    static constexpr auto Knockback = EnchantmentId::minecraft("knockback");

    static constexpr auto FireAspect = EnchantmentId::minecraft("fire_aspect");

    static constexpr auto Looting = EnchantmentId::minecraft("looting");

    static constexpr auto Efficiency = EnchantmentId::minecraft("efficiency");

    static constexpr auto SilkTouch = EnchantmentId::minecraft("silk_touch");

    static constexpr auto Unbreaking = EnchantmentId::minecraft("unbreaking");

    static constexpr auto Fortune = EnchantmentId::minecraft("fortune");

    static constexpr auto Power = EnchantmentId::minecraft("power");

    static constexpr auto Punch = EnchantmentId::minecraft("punch");

    static constexpr auto Flame = EnchantmentId::minecraft("flame");

    static constexpr auto Infinity = EnchantmentId::minecraft("infinity");

    static constexpr auto LuckOfTheSea = EnchantmentId::minecraft("luck_of_the_sea");

    static constexpr auto Lure = EnchantmentId::minecraft("lure");

    static constexpr auto FrostWalker = EnchantmentId::minecraft("frost_walker");

    static constexpr auto Mending = EnchantmentId::minecraft("mending");

    static constexpr auto CurseOfBinding = EnchantmentId::minecraft("binding");

    static constexpr auto CurseOfVanishing = EnchantmentId::minecraft("vanishing");

    static constexpr auto Impaling = EnchantmentId::minecraft("impaling");

    static constexpr auto Riptide = EnchantmentId::minecraft("riptide");

    static constexpr auto Loyalty = EnchantmentId::minecraft("loyalty");

    static constexpr auto Channeling = EnchantmentId::minecraft("channeling");

    static constexpr auto Multishot = EnchantmentId::minecraft("multishot");

    static constexpr auto Piercing = EnchantmentId::minecraft("piercing");

    static constexpr auto QuickCharge = EnchantmentId::minecraft("quick_charge");

    static constexpr auto SoulSpeed = EnchantmentId::minecraft("soul_speed");

    static constexpr auto SwiftSneak = EnchantmentId::minecraft("swift_sneak");

    static constexpr auto WindBurst = EnchantmentId::minecraft("wind_burst");

    static constexpr auto Density = EnchantmentId::minecraft("density");

    static constexpr auto Breach = EnchantmentId::minecraft("breach");

    virtual ~Enchantment() = default;

    [[nodiscard]] virtual EnchantmentId getId() const = 0;

    [[nodiscard]] virtual std::string getTranslationKey() const = 0;

    [[nodiscard]] virtual int getMaxLevel() const = 0;

    [[nodiscard]] virtual int getStartLevel() const = 0;

    [[nodiscard]] virtual bool conflictsWith(const Enchantment &other) const = 0;

    [[nodiscard]] virtual bool canEnchantItem(const ItemStack &item) const = 0;

    bool operator==(const EnchantmentId &other) const { return getId() == other; }

    bool operator!=(const EnchantmentId &other) const { return !(*this == other); }

    bool operator==(const Enchantment &other) const { return getId() == other.getId(); }

    bool operator!=(const Enchantment &other) const { return !(*this == other); }

    operator EnchantmentId() const { return getId(); }
};
}  // namespace endstone
```


