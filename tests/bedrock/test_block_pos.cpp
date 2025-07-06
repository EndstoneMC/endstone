// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in coMirrorliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or iMirrorlied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <gtest/gtest.h>

#include "bedrock/world/level/block_pos.h"

// Test fixture for shared pivots
class TransformTest : public ::testing::Test {
protected:
    // Zero pivot
    Vec3 originPivot{0.0, 0.0, 0.0};
    // Non-zero pivot for secondary tests
    Vec3 nonZeroPivot{1.0, 0.0, 2.0};
};

// ----- Zero-Pivot Tests -----
TEST_F(TransformTest, NoMirrorNoRotation)
{
    BlockPos p(2, 5, -3);
    BlockPos res = p.transform(Rotation::None, Mirror::None, originPivot);
    EXPECT_EQ(res, BlockPos(2, 5, -3));
}

TEST_F(TransformTest, MirrorX_NoRotation)
{
    BlockPos p(4, 1, 7);
    BlockPos res = p.transform(Rotation::None, Mirror::X, originPivot);
    EXPECT_EQ(res, BlockPos(4, 1, -7));
}

TEST_F(TransformTest, MirrorZ_NoRotation)
{
    BlockPos p(4, 1, 7);
    BlockPos res = p.transform(Rotation::None, Mirror::Z, originPivot);
    EXPECT_EQ(res, BlockPos(-4, 1, 7));
}

TEST_F(TransformTest, MirrorXZ_NoRotation)
{
    BlockPos p(4, 1, 7);
    BlockPos res = p.transform(Rotation::None, Mirror::XZ, originPivot);
    EXPECT_EQ(res, BlockPos(-4, 1, -7));
}

TEST_F(TransformTest, NoMirror_Rotate90)
{
    BlockPos p(1, 0, 2);
    BlockPos res = p.transform(Rotation::Rotate90, Mirror::None, originPivot);
    EXPECT_EQ(res, BlockPos(-2, 0, 1));
}

TEST_F(TransformTest, NoMirror_Rotate180)
{
    BlockPos p(1, 0, 2);
    BlockPos res = p.transform(Rotation::Rotate180, Mirror::None, originPivot);
    EXPECT_EQ(res, BlockPos(-1, 0, -2));
}

TEST_F(TransformTest, NoMirror_Rotate270)
{
    BlockPos p(1, 0, 2);
    BlockPos res = p.transform(Rotation::Rotate270, Mirror::None, originPivot);
    EXPECT_EQ(res, BlockPos(2, 0, -1));
}

TEST_F(TransformTest, MirrorX_Rotate90)
{
    BlockPos p(1, 0, 2);
    BlockPos res = p.transform(Rotation::Rotate90, Mirror::X, originPivot);
    EXPECT_EQ(res, BlockPos(2, 0, 1));
}

TEST_F(TransformTest, MirrorZ_Rotate90)
{
    BlockPos p(1, 0, 2);
    BlockPos res = p.transform(Rotation::Rotate90, Mirror::Z, originPivot);
    EXPECT_EQ(res, BlockPos(-2, 0, -1));
}

// ----- Non-Zero-Pivot Tests -----
TEST_F(TransformTest, NonZeroPivot_NoMirrorNoRotation)
{
    BlockPos p(2, 5, -3);
    BlockPos res = p.transform(Rotation::None, Mirror::None, nonZeroPivot);
    EXPECT_EQ(res, BlockPos(2, 5, -3));
}

TEST_F(TransformTest, NonZeroPivot_MirrorX_NoRotation)
{
    BlockPos p(4, 1, 7);
    BlockPos res = p.transform(Rotation::None, Mirror::X, nonZeroPivot);
    // dz=5->-5 => z=2-5=-3, x stays 1+3=4
    EXPECT_EQ(res, BlockPos(4, 1, -3));
}

TEST_F(TransformTest, NonZeroPivot_MirrorZ_NoRotation)
{
    BlockPos p(4, 1, 7);
    BlockPos res = p.transform(Rotation::None, Mirror::Z, nonZeroPivot);
    // dx=3->-3 => x=1-3=-2, z stays 2+5=7
    EXPECT_EQ(res, BlockPos(-2, 1, 7));
}

TEST_F(TransformTest, NonZeroPivot_MirrorXZ_NoRotation)
{
    BlockPos p(4, 1, 7);
    BlockPos res = p.transform(Rotation::None, Mirror::XZ, nonZeroPivot);
    // dx=3->-3, dz=5->-5 => x=1-3=-2, z=2-5=-3
    EXPECT_EQ(res, BlockPos(-2, 1, -3));
}

TEST_F(TransformTest, NonZeroPivot_NoMirror_Rotate90)
{
    BlockPos p(2, 0, 3);
    BlockPos res = p.transform(Rotation::Rotate90, Mirror::None, nonZeroPivot);
    // dx=1,dz=1 -> rx=-1, rz=1 => x=0,z=3
    EXPECT_EQ(res, BlockPos(0, 0, 3));
}

TEST_F(TransformTest, NonZeroPivot_NoMirror_Rotate180)
{
    BlockPos p(2, 0, 3);
    BlockPos res = p.transform(Rotation::Rotate180, Mirror::None, nonZeroPivot);
    // dx=1,dz=1 -> rx=-1, rz=-1 => x=0,z=1
    EXPECT_EQ(res, BlockPos(0, 0, 1));
}

TEST_F(TransformTest, NonZeroPivot_NoMirror_Rotate270)
{
    BlockPos p(2, 0, 3);
    BlockPos res = p.transform(Rotation::Rotate270, Mirror::None, nonZeroPivot);
    // dx=1,dz=1 -> rx=1, rz=-1 => x=2,z=1
    EXPECT_EQ(res, BlockPos(2, 0, 1));
}

TEST_F(TransformTest, NonZeroPivot_MirrorX_Rotate90)
{
    BlockPos p(2, 0, 3);
    BlockPos res = p.transform(Rotation::Rotate90, Mirror::X, nonZeroPivot);
    // dz=1->-1 => (dx=1,dz=-1) -> rotate90: rx=1,rz=1 => x=2,z=3
    EXPECT_EQ(res, BlockPos(2, 0, 3));
}

TEST_F(TransformTest, NonZeroPivot_MirrorZ_Rotate90)
{
    BlockPos p(2, 0, 3);
    BlockPos res = p.transform(Rotation::Rotate90, Mirror::Z, nonZeroPivot);
    // dx=1->-1 => (dx=-1,dz=1) -> rotate90: rx=-1,rz=-1 => x=0,z=1
    EXPECT_EQ(res, BlockPos(0, 0, 1));
}
