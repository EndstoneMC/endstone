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

#include "bedrock/platform/uuid.h"

using mce::UUID;

TEST(UUIDTest, DefaultEmpty)
{
    UUID u = UUID::EMPTY;
    EXPECT_EQ(u.asString(), "00000000-0000-0000-0000-000000000000");
}

TEST(UUIDTest, ConstructAndStringify)
{
    uint64_t high = 0x123456789ABCDEF0ULL;
    uint64_t low = 0x0FEDCBA987654321ULL;
    UUID u(high, low);
    // Expected: 12345678-9abc-def0-0fed-cba987654321
    EXPECT_EQ(u.asString(), "12345678-9abc-def0-0fed-cba987654321");
}

TEST(UUIDTest, OperatorLess)
{
    UUID a(0x0001, 0x0000);
    UUID b(0x0002, 0x0000);
    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);

    UUID c(0x0001, 0x0001);
    UUID d(0x0001, 0x0002);
    EXPECT_TRUE(c < d);
    EXPECT_FALSE(d < c);

    // equal
    UUID e(0x0001, 0x0001);
    EXPECT_FALSE(c < e);
    EXPECT_FALSE(e < c);
}

TEST(UUIDTest, CanParseCompactValid)
{
    std::string s = "0123456789abcdefABCDEFabcdef0123";
    EXPECT_TRUE(UUID::canParse(s));
}

TEST(UUIDTest, CanParseHyphenatedValid)
{
    std::string s = "12345678-9ABC-def0-0fed-CBA987654321";
    EXPECT_TRUE(UUID::canParse(s));
}

TEST(UUIDTest, CanParseInvalidLengths)
{
    EXPECT_FALSE(UUID::canParse(""));
    EXPECT_FALSE(UUID::canParse("1234"));
    EXPECT_FALSE(UUID::canParse(std::string(35, 'a')));
    EXPECT_FALSE(UUID::canParse(std::string(37, 'a')));
}

TEST(UUIDTest, CanParseInvalidChars)
{
    std::string s = "12345678-9xyz-0000-0000-000000000000";
    EXPECT_FALSE(UUID::canParse(s));
}

TEST(UUIDTest, FromStringValidCompact)
{
    std::string comp = "123456789abcdef00fedcba987654321";
    UUID u = UUID::fromString(comp);
    EXPECT_EQ(u.asString(), "12345678-9abc-def0-0fed-cba987654321");
}

TEST(UUIDTest, FromStringValidHyphenated)
{
    std::string hyp = "12345678-9ABC-def0-0fed-CBA987654321";
    UUID u = UUID::fromString(hyp);
    EXPECT_EQ(u.asString(), "12345678-9abc-def0-0fed-cba987654321");
}

TEST(UUIDTest, FromStringInvalid)
{
    UUID u = UUID::fromString("invalid-uuid-string");
    EXPECT_EQ(u.asString(), UUID::EMPTY.asString());
}
