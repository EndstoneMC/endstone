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

#include "endstone/core/base64.h"

// NOLINTNEXTLINE
TEST(Base64Encode, EncodesEmpty)
{
    std::string const expected{};
    std::string const actual{endstone::core::base64_encode({})};
    ASSERT_EQ(expected, actual);
}

// NOLINTNEXTLINE
TEST(Base64Encode, EncodesThreeBytesZeros)
{
    std::array<std::uint8_t, 3> const input{0x00, 0x00, 0x00};
    auto const expected{"AAAA"};
    auto const actual{endstone::core::base64_encode<std::string>(begin(input), end(input))};
    ASSERT_EQ(expected, actual);
}

// NOLINTNEXTLINE
TEST(Base64Encode, EncodesThreeBytesRandom)
{
    std::array<std::uint8_t, 3> const input{0xFE, 0xE9, 0x72};
    auto const expected{"/uly"};
    auto const actual{endstone::core::base64_encode<std::string>(begin(input), end(input))};
    ASSERT_EQ(expected, actual);
}

// NOLINTNEXTLINE
TEST(Base64Encode, EncodesTwoBytes)
{
    std::array<std::uint8_t, 2> const input{0x00, 0x00};
    auto const expected{"AAA="};
    auto const actual{endstone::core::base64_encode<std::string>(begin(input), end(input))};
    ASSERT_EQ(expected, actual);
}

// NOLINTNEXTLINE
TEST(Base64Encode, EncodesOneByte)
{
    std::array<std::uint8_t, 1> const input{0x00};
    auto const expected{"AA=="};
    auto const actual{endstone::core::base64_encode<std::string>(begin(input), end(input))};
    ASSERT_EQ(expected, actual);
}

// NOLINTNEXTLINE
TEST(Base64Encode, EncodesFourBytes)
{
    std::array<std::uint8_t, 4> const input{0x74, 0x68, 0x65, 0x20};
    auto const expected{"dGhlIA=="};
    auto const actual{endstone::core::base64_encode<std::string>(begin(input), end(input))};
    ASSERT_EQ(expected, actual);
}

// NOLINTNEXTLINE
TEST(Base64Encode, EncodesFiveBytes)
{
    std::array<std::uint8_t, 5> const input{0x20, 0x62, 0x72, 0x6f, 0x77};
    auto const expected{"IGJyb3c="};
    auto const actual{endstone::core::base64_encode<std::string>(begin(input), end(input))};
    ASSERT_EQ(actual, expected);
}

// NOLINTNEXTLINE
TEST(Base64Encode, EncodesSixBytes)
{
    std::array<std::uint8_t, 6> const input{0x20, 0x6a, 0x75, 0x6d, 0x70, 0x73};
    auto const expected{"IGp1bXBz"};
    auto const actual{endstone::core::base64_encode<std::string>(begin(input), end(input))};
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
    auto const actual{endstone::core::base64_encode<std::string>(begin(input), end(input))};
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
    auto const actual{endstone::core::base64_encode<std::string>(begin(input), end(input))};
    ASSERT_EQ(actual, expected);
}

// NOLINTNEXTLINE
TEST(Base64Decode, FailDecodeOneString)
{
    std::string const input{"1"};
    ASSERT_FALSE(endstone::core::base64_decode(input).has_value());
}

// NOLINTNEXTLINE
TEST(Base64Decode, FailDecodeOneStringPadded)
{
    std::string const input{"1==="};
    ASSERT_FALSE(endstone::core::base64_decode(input).has_value());
}

// NOLINTNEXTLINE
TEST(Base64Decode, FailDecodeOneCharRemaining)
{
    std::string const input{"something"};
    ASSERT_FALSE(endstone::core::base64_decode(input).has_value());
}

// NOLINTNEXTLINE
TEST(Base64Decode, FailDecodeNonSize4Bigger)
{
    std::string const input{"SomethingEntirelyDifferent"};
    ASSERT_FALSE(endstone::core::base64_decode(input).has_value());
}

// NOLINTNEXTLINE
TEST(Base64Decode, FailDecodeNonBase64Short)
{
    std::string const input{"a aa"};
    ASSERT_FALSE(endstone::core::base64_decode(input).has_value());
}

// NOLINTNEXTLINE
TEST(Base64Decode, FailDecodeNonBase64Longer)
{
    std::string const input{"aaa`aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"};
    ASSERT_FALSE(endstone::core::base64_decode(input).has_value());
}

// NOLINTNEXTLINE
TEST(Base64Decode, DecodesMissingTwoPads0)
{
    std::string const input{"12"};
    ASSERT_FALSE(endstone::core::base64_decode(input).has_value());
}

// NOLINTNEXTLINE
TEST(Base64Decode, DecodesMissingTwoPads1)
{
    std::string const input = "AA";
    ASSERT_FALSE(endstone::core::base64_decode(input).has_value());
}

// NOLINTNEXTLINE
TEST(Base64Decode, DecodesMissingOnePad0)
{
    std::string const input = "AAA";
    ASSERT_FALSE(endstone::core::base64_decode(input).has_value());
}

// NOLINTNEXTLINE
TEST(Base64Decode, DecodesMissingOnePad1)
{
    std::string const input{"12a"};
    ASSERT_FALSE(endstone::core::base64_decode(input).has_value());
}

// NOLINTNEXTLINE
TEST(Base64Decode, DecodesMissingIssueExample)
{
    std::string const input = "eyJuYW1lIjoiSm9obiBEb2UifQ";
    ASSERT_FALSE(endstone::core::base64_decode(input).has_value());
}

// NOLINTNEXTLINE
TEST(Base64Decode, DecodesEmptyString)
{
    std::string const input{};
    std::string expected{};
    auto const actual{endstone::core::base64_decode("")};

    ASSERT_EQ(expected, actual);
}

// NOLINTNEXTLINE
TEST(Base64Decode, DecodesZeroArray)
{
    std::string const input{"AAAA"};
    std::vector<std::uint8_t> const expected{0x00, 0x00, 0x00};
    auto const actual{endstone::core::base64_decode<std::vector<std::uint8_t>>(input)};

    ASSERT_EQ(actual, expected);
}

// NOLINTNEXTLINE
TEST(Base64Decode, DecodesZeroArrayTwice)
{
    std::string const input{"AAAAAAAA"};
    std::vector<std::uint8_t> const expected{0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    auto const actual{endstone::core::base64_decode<std::vector<std::uint8_t>>(input)};

    ASSERT_EQ(actual, expected);
}

// NOLINTNEXTLINE
TEST(Base64Decode, DecodesZeroArrayOneByte)
{
    std::string const input{"AA=="};
    std::vector<std::uint8_t> const expected{0x00};
    auto const actual{endstone::core::base64_decode<std::vector<std::uint8_t>>(input)};

    ASSERT_EQ(actual, expected);
}

// NOLINTNEXTLINE
TEST(Base64Decode, DecodesZeroArrayTwoBytes)
{
    std::string const input{"AAA="};
    std::vector<std::uint8_t> const expected{0x00, 0x00};
    auto const actual{endstone::core::base64_decode<std::vector<std::uint8_t>>(input)};

    ASSERT_EQ(actual, expected);
}

// NOLINTNEXTLINE
TEST(Base64Decode, DecodesQuickFox)
{
    std::string const input{"VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZw=="};
    std::vector<std::uint8_t> const expected{0x54, 0x68, 0x65, 0x20, 0x71, 0x75, 0x69, 0x63, 0x6b, 0x20, 0x62,
                                             0x72, 0x6f, 0x77, 0x6e, 0x20, 0x66, 0x6f, 0x78, 0x20, 0x6a, 0x75,
                                             0x6d, 0x70, 0x73, 0x20, 0x6f, 0x76, 0x65, 0x72, 0x20, 0x74, 0x68,
                                             0x65, 0x20, 0x6c, 0x61, 0x7a, 0x79, 0x20, 0x64, 0x6f, 0x67};
    auto const actual{endstone::core::base64_decode<std::vector<std::uint8_t>>(input)};
    ASSERT_EQ(actual, expected);
}

// NOLINTNEXTLINE
TEST(Base64RoundTripTests, AllPossibleBytes)
{
    std::vector<std::uint8_t> all_possible_bytes;
    for (std::size_t i = 0; i <= 255; ++i) {
        all_possible_bytes.push_back(static_cast<std::uint8_t>(i));
    }

    auto const encode_string =
        endstone::core::base64_encode<std::string>(begin(all_possible_bytes), end(all_possible_bytes));
    auto const decoded_bytes = endstone::core::base64_decode<std::vector<std::uint8_t>>(encode_string);
    ASSERT_EQ(all_possible_bytes, decoded_bytes);
}

// NOLINTNEXTLINE
TEST(Base64RoundTripTests, ExhaustiveTests)
{
    std::vector<std::string> const base64_strings = {"YW55IGNhcm5hbCBwbGVhcw==",
                                                     "bGVnYWwgcGFzcw==",
                                                     "dGVzdCBzdHJpbmc=",
                                                     "bGVnYWwgcHJvdmlkZXI=",
                                                     "ZW5vdWdoIHRoZSBzYW1lIG9mIHRoZSBwbGFjZQ==",
                                                     "YW5vdGhlciB0aGUgc3RyYWlnaHQ=",
                                                     "d2FzIG1lIGFkZHJlc3MgcHJvdmlkZXI=",
                                                     "YWJvdXQgdGhlIG1hc3RlciBvZiB0aGUgZGFtYWdl",
                                                     "ZW50aXJlIHRoYXQgYnJvdWdodCBvZiB0aGUgbW9uZXk=",
                                                     "bGVnYWwgc2VjdXJpdHk=",
                                                     "YmFzaWMgZ29vZCBvZiB0aGUgcGFkIHN0cmluZw==",
                                                     "ZGVsZXRlIHN0cmluZyBvZiB0aGUgc3RyYWlnaHQ=",
                                                     "YnJvdWdodCBvZiB0aGUgcGFkIGZvbGRlciBvZiB0aGUgZGFtYWdl",
                                                     "aW50ZXJmYWNlIHN0cmluZw==",
                                                     "Y29uc29sZS1tZS1jb21wYW55",
                                                     "aW5mb3JtYXRpb24tbWVkaWE=",
                                                     "c3RhdHVzLXNlY3VyZQ==",
                                                     "Y3JlYXRlLWNvbXBhbnktc3RyaW5n",
                                                     "b3JkZXItbGVhZGVy",
                                                     "Y2F0YWxvZy1wcm9maWxl",
                                                     "dGVzdC1jb25zdWx0aW5n",
                                                     "YnJvdWdodC1sZWFkZXI=",
                                                     "YXNzaWduLW1lY2hhbmlzbQ==",
                                                     "bGVnYWwtY29udGFpbmVy",
                                                     "ZW1haWwtY29udGFpbmVy",
                                                     "aW5zdGFuY2UtY29udGFpbmVy",
                                                     "dGVzdC1jb21wYW55LWFuZC1wcm9maWxl",
                                                     "YmFzZTY0LWJhc2U=",
                                                     "cGFzc3dvcmQ=",
                                                     "Zm9vYmFy",
                                                     "Y29vbC1iYXNl",
                                                     "YmFzZTY0LXNlY3VyZQ==",
                                                     "aW50ZXJ2YWw=",
                                                     "dGhlLW1hc3Rlci1vZi10aGUtZGFtYWdl",
                                                     "c2FtZS1wbGFjZS1vZi10aGUtZGFtYWdl",
                                                     "aGFzaC1zb21ldGhpbmc="};

    for (auto const &b64_string : base64_strings) {
        auto const decoded = endstone::core::base64_decode(b64_string);
        ASSERT_TRUE(decoded.has_value());
        auto const encoded_round_trip = endstone::core::base64_encode(decoded.value());
        ASSERT_EQ(encoded_round_trip, b64_string);
    }
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
        auto const actual = endstone::core::base64_encode(input);
        ASSERT_EQ(actual, expected);
    }
}

// NOLINTNEXTLINE
TEST(Base64RoundTripTests, TypeMixTests)
{
    const std::string strinput{"Hello, World!"};
    const std::string stroutput{"SGVsbG8sIFdvcmxkIQ=="};

    typedef std::vector<std::uint8_t> u8vec_type;
    const u8vec_type uvecinput(strinput.begin(), strinput.end());
    const u8vec_type uvecoutput(stroutput.begin(), stroutput.end());

    typedef std::vector<std::int8_t> s8vec_type;
    const s8vec_type svecinput(strinput.begin(), strinput.end());
    const s8vec_type svecoutput(stroutput.begin(), stroutput.end());

    // str -> str
    {
        auto tmp1 = endstone::core::base64_encode<std::string>(strinput.begin(), strinput.end());
        ASSERT_EQ(tmp1, stroutput);
        auto tmp2 = endstone::core::base64_decode<std::string>(stroutput.begin(), stroutput.end());
        ASSERT_EQ(tmp2, strinput);
        auto tmp3 = endstone::core::base64_encode<std::string>(strinput);
        ASSERT_EQ(tmp3, stroutput);
        auto tmp4 = endstone::core::base64_decode<std::string>(stroutput);
        ASSERT_EQ(tmp4, strinput);
        auto tmp5 = endstone::core::base64_encode(strinput);
        ASSERT_EQ(tmp5, stroutput);
        auto tmp6 = endstone::core::base64_decode(stroutput);
        ASSERT_EQ(tmp6, strinput);
    }

    // str -> u8
    {
        auto tmp1 = endstone::core::base64_encode<u8vec_type>(strinput.begin(), strinput.end());
        ASSERT_EQ(tmp1, uvecoutput);
        auto tmp2 = endstone::core::base64_decode<u8vec_type>(stroutput.begin(), stroutput.end());
        ASSERT_EQ(tmp2, uvecinput);
        auto tmp3 = endstone::core::base64_encode<u8vec_type>(strinput);
        ASSERT_EQ(tmp3, uvecoutput);
        auto tmp4 = endstone::core::base64_decode<u8vec_type>(stroutput);
        ASSERT_EQ(tmp4, uvecinput);
    }

    // str -> s8
    {
        auto tmp1 = endstone::core::base64_encode<s8vec_type>(strinput.begin(), strinput.end());
        ASSERT_EQ(tmp1, svecoutput);
        auto tmp2 = endstone::core::base64_decode<s8vec_type>(stroutput.begin(), stroutput.end());
        ASSERT_EQ(tmp2, svecinput);
        auto tmp3 = endstone::core::base64_encode<s8vec_type>(strinput);
        ASSERT_EQ(tmp3, svecoutput);
        auto tmp4 = endstone::core::base64_decode<s8vec_type>(stroutput);
        ASSERT_EQ(tmp4, svecinput);
    }

    // u8 -> str
    {
        auto tmp1 = endstone::core::base64_encode<std::string>(uvecinput.begin(), uvecinput.end());
        ASSERT_EQ(tmp1, stroutput);
        auto tmp2 = endstone::core::base64_decode<std::string>(uvecoutput.begin(), uvecoutput.end());
        ASSERT_EQ(tmp2, strinput);
    }

    // u8 -> u8
    {
        auto tmp1 = endstone::core::base64_encode<u8vec_type>(uvecinput.begin(), uvecinput.end());
        ASSERT_EQ(tmp1, uvecoutput);
        auto tmp2 = endstone::core::base64_decode<u8vec_type>(uvecoutput.begin(), uvecoutput.end());
        ASSERT_EQ(tmp2, uvecinput);
    }

    // u8 -> s8
    {
        auto tmp1 = endstone::core::base64_encode<s8vec_type>(uvecinput.begin(), uvecinput.end());
        ASSERT_EQ(tmp1, svecoutput);
        auto tmp2 = endstone::core::base64_decode<s8vec_type>(uvecoutput.begin(), uvecoutput.end());
        ASSERT_EQ(tmp2, svecinput);
    }

    // s8 -> str
    {
        auto tmp1 = endstone::core::base64_encode<std::string>(svecinput.begin(), svecinput.end());
        ASSERT_EQ(tmp1, stroutput);
        auto tmp2 = endstone::core::base64_decode<std::string>(svecoutput.begin(), svecoutput.end());
        ASSERT_EQ(tmp2, strinput);
    }

    // s8 -> u8
    {
        auto tmp1 = endstone::core::base64_encode<u8vec_type>(svecinput.begin(), svecinput.end());
        ASSERT_EQ(tmp1, uvecoutput);
        auto tmp2 = endstone::core::base64_decode<u8vec_type>(svecoutput.begin(), svecoutput.end());
        ASSERT_EQ(tmp2, uvecinput);
    }

    // s8 -> s8
    {
        auto tmp1 = endstone::core::base64_encode<s8vec_type>(svecinput.begin(), svecinput.end());
        ASSERT_EQ(tmp1, svecoutput);
        auto tmp2 = endstone::core::base64_decode<s8vec_type>(svecoutput.begin(), svecoutput.end());
        ASSERT_EQ(tmp2, svecinput);
    }
}
