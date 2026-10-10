// Copyright (c) 2026, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <chrono>

#include <gtest/gtest.h>

#include "endstone/core/util/rolling_average.h"

using endstone::core::RollingAverage;
using namespace std::chrono_literals;

TEST(RollingAverageTest, StartsAtInitialValue)
{
    const RollingAverage average(60, 20.0);
    EXPECT_DOUBLE_EQ(average.getAverage(), 20.0);
}

TEST(RollingAverageTest, WeightsSamplesByTime)
{
    RollingAverage average(2, 20.0);
    average.add(10.0, 2s);
    average.add(5.0, 4s);
    EXPECT_DOUBLE_EQ(average.getAverage(), 40.0 / 6.0);
}
