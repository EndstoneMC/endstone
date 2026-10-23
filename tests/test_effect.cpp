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

#include "endstone/potion/effect.h"

#include <optional>
#include <string>

#include <gtest/gtest.h>

using namespace endstone;

TEST(EffectTest, OwnsItsTypeBeyondTheLifetimeOfTheSource)
{
    std::optional<Effect> effect;
    {
        std::string name = "minecraft:speed";
        effect.emplace(EffectId{name}, 200, 1);
        name.assign(64, 'x');
    }
    EXPECT_EQ(effect->getType(), "minecraft:speed");
    EXPECT_EQ(std::string(effect->getType()), "minecraft:speed");
}

TEST(EffectTest, OwnsItsTypeWhenBuiltFromATemporary)
{
    const Effect effect{EffectId{std::string{"minecraft:regeneration"}}, 100, 0};
    EXPECT_EQ(effect.getType().getNamespace(), "minecraft");
    EXPECT_EQ(effect.getType().getKey(), "regeneration");
}

TEST(EffectTest, DefaultsToMinecraftNamespace)
{
    const Effect effect{"poison", 100, 0};
    EXPECT_EQ(effect.getType(), "minecraft:poison");
}

TEST(EffectTest, SurvivesCopyAndAssignment)
{
    const Effect effect{EffectType::Wither, 60, 2};
    Effect copy = effect;
    EXPECT_EQ(copy.getType(), "minecraft:wither");

    const Effect other{EffectType::Darkness, 20, 0};
    copy = other;
    EXPECT_EQ(copy.getType(), "minecraft:darkness");
    EXPECT_EQ(effect.getType(), "minecraft:wither");
}

TEST(EffectTest, DefaultsAndAccessors)
{
    const Effect effect{EffectType::Strength, 300, 2};
    EXPECT_EQ(effect.getDuration(), 300);
    EXPECT_FALSE(effect.isInfinite());
    EXPECT_EQ(effect.getAmplifier(), 2);
    EXPECT_FALSE(effect.isAmbient());
    EXPECT_TRUE(effect.hasParticles());
    EXPECT_TRUE(effect.hasIcon());
}

TEST(EffectTest, InfiniteWhenDurationIsInfiniteDuration)
{
    const Effect effect{EffectType::NightVision, Effect::InfiniteDuration, 0};
    EXPECT_TRUE(effect.isInfinite());
    EXPECT_EQ(effect.getDuration(), -1);
}

TEST(EffectTest, EqualityNormalisesTheNamespace)
{
    const Effect bare{"speed", 200, 1};
    const Effect qualified{"minecraft:speed", 200, 1};
    EXPECT_EQ(bare, qualified);
    EXPECT_FALSE(bare != qualified);
}

TEST(EffectTest, EqualityComparesEveryField)
{
    const Effect effect{EffectType::Speed, 200, 1};
    EXPECT_NE(effect, Effect(EffectType::Slowness, 200, 1));
    EXPECT_NE(effect, Effect(EffectType::Speed, 201, 1));
    EXPECT_NE(effect, Effect(EffectType::Speed, 200, 2));
    EXPECT_NE(effect, Effect(EffectType::Speed, 200, 1, true));
    EXPECT_NE(effect, Effect(EffectType::Speed, 200, 1, false, false));
    EXPECT_NE(effect, Effect(EffectType::Speed, 200, 1, false, true, false));
}
