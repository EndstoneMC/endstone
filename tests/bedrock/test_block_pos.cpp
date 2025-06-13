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

#include "bedrock/world/level/block_pos.h"

struct TestParam {
    Mirror mirror;
    Rotation rotation;
    Vec3 pivot;
    int expected_x;
    int expected_z;
};

class BlockPosTest : public ::testing::TestWithParam<TestParam> {};

TEST_P(BlockPosTest, TransformTest)
{
    auto const &param = GetParam();

    // Always start at pos = (1, 2, 5)
    BlockPos pos{1, 2, 5};

    // Perform mirror+rotate around the given pivot
    BlockPos out = pos.transform(param.rotation, param.mirror, param.pivot);

    // Verify X,Z as expected; Y stays 2
    EXPECT_EQ(out.x, param.expected_x) << "mirror=" << static_cast<int>(param.mirror)
                                       << " rot=" << static_cast<int>(param.rotation) << " pivot=(" << param.pivot.x
                                       << "," << param.pivot.z << ")";
    EXPECT_EQ(out.y, 2);
    EXPECT_EQ(out.z, param.expected_z) << "mirror=" << static_cast<int>(param.mirror)
                                       << " rot=" << static_cast<int>(param.rotation) << " pivot=(" << param.pivot.x
                                       << "," << param.pivot.z << ")";
}

INSTANTIATE_TEST_SUITE_P(FullCoverage, BlockPosTest,
                         ::testing::Values(
                             // Pivot = (0,0,0), pos=(1,2,5)
                             TestParam{Mirror::None, Rotation::None, Vec3{0, 0, 0}, 1, 5},
                             TestParam{Mirror::None, Rotation::Rotate90, Vec3{0, 0, 0}, 5, -1},
                             TestParam{Mirror::None, Rotation::Rotate180, Vec3{0, 0, 0}, -1, -5},
                             TestParam{Mirror::None, Rotation::Rotate270, Vec3{0, 0, 0}, -5, 1},

                             TestParam{Mirror::X, Rotation::None, Vec3{0, 0, 0}, 1, -5},
                             TestParam{Mirror::X, Rotation::Rotate90, Vec3{0, 0, 0}, -5, -1},
                             TestParam{Mirror::X, Rotation::Rotate180, Vec3{0, 0, 0}, -1, 5},
                             TestParam{Mirror::X, Rotation::Rotate270, Vec3{0, 0, 0}, 5, 1},

                             TestParam{Mirror::Z, Rotation::None, Vec3{0, 0, 0}, -1, 5},
                             TestParam{Mirror::Z, Rotation::Rotate90, Vec3{0, 0, 0}, 5, 1},
                             TestParam{Mirror::Z, Rotation::Rotate180, Vec3{0, 0, 0}, 1, -5},
                             TestParam{Mirror::Z, Rotation::Rotate270, Vec3{0, 0, 0}, -5, -1},

                             TestParam{Mirror::XZ, Rotation::None, Vec3{0, 0, 0}, -1, -5},
                             TestParam{Mirror::XZ, Rotation::Rotate90, Vec3{0, 0, 0}, -5, 1},
                             TestParam{Mirror::XZ, Rotation::Rotate180, Vec3{0, 0, 0}, 1, 5},
                             TestParam{Mirror::XZ, Rotation::Rotate270, Vec3{0, 0, 0}, 5, -1},

                             // Pivot = (10,0,20), pos=(1,2,5)
                             TestParam{Mirror::None, Rotation::None, Vec3{10, 0, 20}, 1, 5},
                             TestParam{Mirror::None, Rotation::Rotate90, Vec3{10, 0, 20}, -5, 29},
                             TestParam{Mirror::None, Rotation::Rotate180, Vec3{10, 0, 20}, 19, 35},
                             TestParam{Mirror::None, Rotation::Rotate270, Vec3{10, 0, 20}, 25, 11},

                             TestParam{Mirror::X, Rotation::None, Vec3{10, 0, 20}, 1, 35},
                             TestParam{Mirror::X, Rotation::Rotate90, Vec3{10, 0, 20}, 25, 29},
                             TestParam{Mirror::X, Rotation::Rotate180, Vec3{10, 0, 20}, 19, 5},
                             TestParam{Mirror::X, Rotation::Rotate270, Vec3{10, 0, 20}, -5, 11},

                             TestParam{Mirror::Z, Rotation::None, Vec3{10, 0, 20}, 19, 5},
                             TestParam{Mirror::Z, Rotation::Rotate90, Vec3{10, 0, 20}, -5, 11},
                             TestParam{Mirror::Z, Rotation::Rotate180, Vec3{10, 0, 20}, 1, 35},
                             TestParam{Mirror::Z, Rotation::Rotate270, Vec3{10, 0, 20}, 25, 29},

                             TestParam{Mirror::XZ, Rotation::None, Vec3{10, 0, 20}, 19, 35},
                             TestParam{Mirror::XZ, Rotation::Rotate90, Vec3{10, 0, 20}, 25, 11},
                             TestParam{Mirror::XZ, Rotation::Rotate180, Vec3{10, 0, 20}, 1, 5},
                             TestParam{Mirror::XZ, Rotation::Rotate270, Vec3{10, 0, 20}, -5, 29}));
