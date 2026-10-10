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

#include "endstone/core/util/tick_times.h"

using endstone::core::TickTimes;
using namespace std::chrono_literals;

TEST(TickTimesTest, NoData)
{
    const TickTimes times(100);
    EXPECT_DOUBLE_EQ(times.getAverage().count(), 0.0);
}

TEST(TickTimesTest, AveragesOnlyRecordedTicks)
{
    TickTimes times(100);
    times.add(10ms);
    times.add(20ms);
    EXPECT_DOUBLE_EQ(times.getAverage().count(), 15.0);
}

TEST(TickTimesTest, KeepsTheLatestTicks)
{
    TickTimes times(2);
    times.add(10ms);
    times.add(20ms);
    times.add(30ms);
    EXPECT_DOUBLE_EQ(times.getAverage().count(), 25.0);
}
