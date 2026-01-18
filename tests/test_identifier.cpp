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

#include <gtest/gtest.h>

#include "endstone/enchantments/enchantment.h"
#include "endstone/identifier.h"
#include "endstone/inventory/item_type.h"

using namespace endstone;

static_assert(Enchantment::Protection.getNamespace() == "minecraft");
static_assert(Enchantment::Protection.getKey() == "protection");
static_assert(Enchantment::Protection == "minecraft:protection");

TEST(IdentifierTest, ParsesWithExplicitNamespace)
{
    constexpr auto id = ItemTypeId{"endstone:custom"};
    EXPECT_EQ(id.getNamespace(), "endstone");
    EXPECT_EQ(id.getKey(), "custom");
    EXPECT_TRUE(id == "endstone:custom");
    EXPECT_FALSE(id != "endstone:custom");
}

TEST(IdentifierTest, DefaultsToMinecraftNamespace)
{
    constexpr auto id = Enchantment::Sharpness;
    EXPECT_EQ(id.getNamespace(), "minecraft");
    EXPECT_EQ(id.getKey(), "sharpness");
    EXPECT_TRUE(id == "sharpness");  // treated as "minecraft:sharpness"
    EXPECT_TRUE(id == "minecraft:sharpness");
    EXPECT_FALSE(id == "minecraft:smite");
}

TEST(IdentifierTest, TwoArgConstructor)
{
    constexpr auto id = EnchantmentId{"minecraft", "thorns"};
    EXPECT_EQ(id.getNamespace(), "minecraft");
    EXPECT_EQ(id.getKey(), "thorns");
    EXPECT_TRUE(id == "minecraft:thorns");
}

TEST(IdentifierTest, EqualityAndInequalityBetweenIdentifiers)
{
    constexpr EnchantmentId a{"minecraft", "mending"};
    constexpr EnchantmentId b{"minecraft:mending"};
    constexpr EnchantmentId c{"minecraft:unbreaking"};
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_NE(a, c);
}

TEST(IdentifierTest, ToStdStringRoundTrip)
{
    constexpr auto id = Enchantment::SilkTouch;
    std::string s = id;
    EXPECT_EQ(s, "minecraft:silk_touch");
}
