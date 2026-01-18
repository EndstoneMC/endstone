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

#include "endstone/util/vector.h"

class VectorTest : public ::testing::Test {
protected:
    endstone::Vector v1_{1.0, 2.0, 3.0};
    endstone::Vector v2_{4.0, 5.0, 6.0};
};

TEST_F(VectorTest, GetX)
{
    EXPECT_EQ(v1_.getX(), 1.0);
}

TEST_F(VectorTest, SetX)
{
    v1_.setX(7.0);
    EXPECT_EQ(v1_.getX(), 7.0);
}

TEST_F(VectorTest, GetY)
{
    EXPECT_EQ(v1_.getY(), 2.0);
}

TEST_F(VectorTest, SetY)
{
    v1_.setY(8.0);
    EXPECT_EQ(v1_.getY(), 8.0);
}

TEST_F(VectorTest, GetZ)
{
    EXPECT_EQ(v1_.getZ(), 3.0);
}

TEST_F(VectorTest, SetZ)
{
    v1_.setZ(9.0);
    EXPECT_EQ(v1_.getZ(), 9.0);
}

TEST_F(VectorTest, AdditionOperator)
{
    auto v3 = v1_ + v2_;
    EXPECT_EQ(v3.getX(), 5.0);
    EXPECT_EQ(v3.getY(), 7.0);
    EXPECT_EQ(v3.getZ(), 9.0);
}

TEST_F(VectorTest, SubtractionOperator)
{
    auto v3 = v1_ - v2_;
    EXPECT_EQ(v3.getX(), -3.0);
    EXPECT_EQ(v3.getY(), -3.0);
    EXPECT_EQ(v3.getZ(), -3.0);
}

TEST_F(VectorTest, MultiplicationOperator)
{
    auto v3 = v1_ * v2_;
    EXPECT_EQ(v3.getX(), 4.0);
    EXPECT_EQ(v3.getY(), 10.0);
    EXPECT_EQ(v3.getZ(), 18.0);
}

TEST_F(VectorTest, DivisionOperator)
{
    auto v3 = v2_ / v1_;
    EXPECT_EQ(v3.getX(), 4.0);
    EXPECT_EQ(v3.getY(), 2.5);
    EXPECT_EQ(v3.getZ(), 2.0);
}

TEST_F(VectorTest, Length)
{
    EXPECT_NEAR(v1_.length(), 3.741657, 0.000001);
}

TEST_F(VectorTest, LengthSquared)
{
    EXPECT_EQ(v1_.lengthSquared(), 14.0);
}

TEST_F(VectorTest, Distance)
{
    EXPECT_NEAR(v1_.distance(v2_), 5.196152, 0.000001);
}

TEST_F(VectorTest, DistanceSquared)
{
    EXPECT_EQ(v1_.distanceSquared(v2_), 27.0);
}
