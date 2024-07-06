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

// Test suite ported from https://github.com/matheusgomes28/base64pp
#include <array>
#include <cstdint>
#include <string>

#include <gtest/gtest.h>

#include "endstone/detail/base64.h"

// NOLINTNEXTLINE
TEST(Base64Encode, EncodesEmpty)
{
    std::string const expected{};
    std::string const actual{endstone::detail::base64_encode({})};
    ASSERT_EQ(expected, actual);
}

// NOLINTNEXTLINE
TEST(Base64Encode, EncodesThreeBytesZeros)
{
    std::array<std::uint8_t, 3> const input{0x00, 0x00, 0x00};
    auto const expected{"AAAA"};
    auto const actual{endstone::detail::base64_encode<std::string>(begin(input), end(input))};
    ASSERT_EQ(expected, actual);
}

// NOLINTNEXTLINE
TEST(Base64Encode, EncodesThreeBytesRandom)
{
    std::array<std::uint8_t, 3> const input{0xFE, 0xE9, 0x72};
    auto const expected{"/uly"};
    auto const actual{endstone::detail::base64_encode<std::string>(begin(input), end(input))};
    ASSERT_EQ(expected, actual);
}

// NOLINTNEXTLINE
TEST(Base64Encode, EncodesTwoBytes)
{
    std::array<std::uint8_t, 2> const input{0x00, 0x00};
    auto const expected{"AAA="};
    auto const actual{endstone::detail::base64_encode<std::string>(begin(input), end(input))};
    ASSERT_EQ(expected, actual);
}

// NOLINTNEXTLINE
TEST(Base64Encode, EncodesOneByte)
{
    std::array<std::uint8_t, 1> const input{0x00};
    auto const expected{"AA=="};
    auto const actual{endstone::detail::base64_encode<std::string>(begin(input), end(input))};
    ASSERT_EQ(expected, actual);
}

// NOLINTNEXTLINE
TEST(Base64Encode, EncodesFourBytes)
{
    std::array<std::uint8_t, 4> const input{0x74, 0x68, 0x65, 0x20};
    auto const expected{"dGhlIA=="};
    auto const actual{endstone::detail::base64_encode<std::string>(begin(input), end(input))};
    ASSERT_EQ(expected, actual);
}

// NOLINTNEXTLINE
TEST(Base64Encode, EncodesFiveBytes)
{
    std::array<std::uint8_t, 5> const input{0x20, 0x62, 0x72, 0x6f, 0x77};
    auto const expected{"IGJyb3c="};
    auto const actual{endstone::detail::base64_encode<std::string>(begin(input), end(input))};
    ASSERT_EQ(actual, expected);
}

// NOLINTNEXTLINE
TEST(Base64Encode, EncodesSixBytes)
{
    std::array<std::uint8_t, 6> const input{0x20, 0x6a, 0x75, 0x6d, 0x70, 0x73};
    auto const expected{"IGp1bXBz"};
    auto const actual{endstone::detail::base64_encode<std::string>(begin(input), end(input))};
    ASSERT_EQ(actual, expected);
}

// NOLINTNEXTLINE
TEST(Base64Encode, EncodesBrownFox)
{
    std::array<std::uint8_t, 43> const input{0x74, 0x68, 0x65, 0x20, 0x71, 0x75, 0x69, 0x63, 0x6b, 0x20, 0x62,
                                             0x72, 0x6f, 0x77, 0x6e, 0x20, 0x66, 0x6f, 0x78, 0x20, 0x6a, 0x75,
                                             0x6d, 0x70, 0x73, 0x20, 0x6f, 0x76, 0x65, 0x72, 0x20, 0x74, 0x68,
                                             0x65, 0x20, 0x6c, 0x61, 0x7a, 0x79, 0x20, 0x64, 0x6f, 0x67};

    auto const expected{"dGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZw=="};
    auto const actual{endstone::detail::base64_encode<std::string>(begin(input), end(input))};
    ASSERT_EQ(actual, expected);
}

// NOLINTNEXTLINE
TEST(Base64Encode, EncodesBrownFastFoxNullInMiddle)
{
    std::array<std::uint8_t, 45> const input{0x74, 0x68, 0x65, 0x20, 0x71, 0x75, 0x69, 0x63, 0x6b, 0x21, 0x20, 0x62,
                                             0x72, 0x6f, 0x77, 0x6e, 0x20, 0x66, 0x6f, 0x78, 0x20, 0x6a, 0x75, 0x6d,
                                             0x70, 0x73, 0x20, 0x6f, 0x76, 0x65, 0x72, 0x20, 0x74, 0x68, 0x65, 0x00,
                                             0x20, 0x6c, 0x61, 0x7a, 0x79, 0x20, 0x64, 0x6f, 0x67};

    auto const expected{"dGhlIHF1aWNrISBicm93biBmb3gganVtcHMgb3ZlciB0aGUAIGxhenkgZG9n"};
    auto const actual{endstone::detail::base64_encode<std::string>(begin(input), end(input))};
    ASSERT_EQ(actual, expected);
}

// NOLINTNEXTLINE
TEST(Base64OverloadTests, EncodesString1)
{
    std::array<std::pair<std::string, std::string>, 11> const test_cases = {
        {{"", ""},
         {"Hello, World!", "SGVsbG8sIFdvcmxkIQ=="},
         {"abcdefghijklmnopqrstuvwxyz0123456789\\`!\"£$%^&*()_+",
          "YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXowMTIzNDU2Nzg5XGAhIsKjJCVeJiooKV8r"},
         {"Base64 encoding", "QmFzZTY0IGVuY29kaW5n"},
         {"I love coding", "SSBsb3ZlIGNvZGluZw=="},
         {"C++23 is awesome", "QysrMjMgaXMgYXdlc29tZQ=="},
         {"This is a sample", "VGhpcyBpcyBhIHNhbXBsZQ=="},
         {"Base64 is useful", "QmFzZTY0IGlzIHVzZWZ1bA=="},
         {"Encode and decode", "RW5jb2RlIGFuZCBkZWNvZGU="},
         {"Data encryption", "RGF0YSBlbmNyeXB0aW9u"},
         {"Th3 Quickk  Br0wn f0x", "VGgzIFF1aWNrayAgQnIwd24gZjB4"}}};

    for (auto const &[input, expected] : test_cases) {
        auto const actual = endstone::detail::base64_encode(input);
        ASSERT_EQ(actual, expected);
    }
}
