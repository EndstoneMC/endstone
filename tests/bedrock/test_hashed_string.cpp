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

#include "bedrock/core/hashed_string.h"

TEST(HashedStringTest, ComputerHash)
{
    ASSERT_EQ(HashedString::computeHash("minecraft:player.exhaustion"), 0x8afb6992bde84d57);
    ASSERT_EQ(HashedString::computeHash("minecraft:player.experience"), 0xd5c6a6b9319a39cb);
    ASSERT_EQ(HashedString::computeHash("minecraft:player.hunger"), 0x3c90c06ed4382c12);
    ASSERT_EQ(HashedString::computeHash("minecraft:player.level"), 0x82284d95df87fac7);
    ASSERT_EQ(HashedString::computeHash("minecraft:player.saturation"), 0x5185acc9f6e9a50b);
}
