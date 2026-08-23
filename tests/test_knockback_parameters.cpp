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

#include <gtest/gtest.h>

#include "endstone/actor/knockback_parameters.h"

using endstone::KnockbackParameters;

TEST(KnockbackParametersTest, PreservesEveryNativeValue)
{
    const KnockbackParameters parameters{
        0.4F, 0.36F, 0.4F, 0.5F, true, false, true, 1.0F, KnockbackParameters::ExtraKnockbackApproach::ReapplyDefault};

    EXPECT_FLOAT_EQ(parameters.getHorizontalPower(), 0.4F);
    EXPECT_FLOAT_EQ(parameters.getVerticalPower(), 0.36F);
    EXPECT_FLOAT_EQ(parameters.getVerticalVelocityCap(), 0.4F);
    EXPECT_FLOAT_EQ(parameters.getSlowdownScale(), 0.5F);
    EXPECT_TRUE(parameters.isScaleWithDamage());
    EXPECT_FALSE(parameters.isSlowDownAttacker());
    EXPECT_TRUE(parameters.isCheckLegacyKnockback());
    EXPECT_FLOAT_EQ(parameters.getExtraKnockbackPower(), 1.0F);
    EXPECT_EQ(parameters.getExtraKnockbackApproach(), KnockbackParameters::ExtraKnockbackApproach::ReapplyDefault);
}

TEST(KnockbackParametersTest, AllowsEveryNativeValueToBeChanged)
{
    KnockbackParameters parameters{
        0.4F, 0.4F, 0.4F, 0.5F, false, false, false, 0.0F, KnockbackParameters::ExtraKnockbackApproach::ReapplyDefault};

    parameters.setHorizontalPower(0.3F);
    parameters.setVerticalPower(0.2F);
    parameters.setVerticalVelocityCap(0.6F);
    parameters.setSlowdownScale(0.25F);
    parameters.setScaleWithDamage(true);
    parameters.setSlowDownAttacker(true);
    parameters.setCheckLegacyKnockback(true);
    parameters.setExtraKnockbackPower(2.0F);
    parameters.setExtraKnockbackApproach(KnockbackParameters::ExtraKnockbackApproach::MultiplyReduced);

    EXPECT_FLOAT_EQ(parameters.getHorizontalPower(), 0.3F);
    EXPECT_FLOAT_EQ(parameters.getVerticalPower(), 0.2F);
    EXPECT_FLOAT_EQ(parameters.getVerticalVelocityCap(), 0.6F);
    EXPECT_FLOAT_EQ(parameters.getSlowdownScale(), 0.25F);
    EXPECT_TRUE(parameters.isScaleWithDamage());
    EXPECT_TRUE(parameters.isSlowDownAttacker());
    EXPECT_TRUE(parameters.isCheckLegacyKnockback());
    EXPECT_FLOAT_EQ(parameters.getExtraKnockbackPower(), 2.0F);
    EXPECT_EQ(parameters.getExtraKnockbackApproach(), KnockbackParameters::ExtraKnockbackApproach::MultiplyReduced);
}
