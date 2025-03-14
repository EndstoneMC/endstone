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

#include "bedrock/core/utility/binary_stream.h"

TEST(ReadOnlyBinaryStreamTest, CopyBuffer)
{
    std::string buffer = "Hello, World!";
    ReadOnlyBinaryStream stream(buffer, true);
    EXPECT_EQ(stream.getView(), buffer);
    EXPECT_NE(stream.getView().data(), buffer.data());
}

TEST(ReadOnlyBinaryStreamTest, NonCopyBuffer)
{
    std::string buffer = "Hello, World!";
    ReadOnlyBinaryStream stream(buffer, false);
    EXPECT_EQ(stream.getView(), buffer);
    EXPECT_EQ(stream.getView().data(), buffer.data());
}

TEST(ReadOnlyBinaryStreamTest, InitialReadPointerIsZero)
{
    std::string buffer = "data";
    ReadOnlyBinaryStream stream(buffer, false);
    EXPECT_EQ(stream.getReadPointer(), 0U);
}

TEST(ReadOnlyBinaryStreamTest, GetByteReadsCorrectly)
{
    // Using characters 'A', 'B', 'C'
    std::string buffer = "\x41\x42\x43";
    ReadOnlyBinaryStream stream(buffer, false);

    auto result_a = stream.getByte();
    EXPECT_TRUE(result_a.ignoreError());
    EXPECT_EQ(result_a.discardError().value(), 0x41);
    EXPECT_EQ(stream.getReadPointer(), 1U);

    auto result_b = stream.getByte();
    EXPECT_TRUE(result_b.ignoreError());
    EXPECT_EQ(result_b.discardError().value(), 0x42);
    EXPECT_EQ(stream.getReadPointer(), 2U);

    auto result_c = stream.getByte();
    EXPECT_TRUE(result_c.ignoreError());
    EXPECT_EQ(result_c.discardError().value(), 0x43);
    EXPECT_EQ(stream.getReadPointer(), 3U);
}

TEST(ReadOnlyBinaryStreamTest, GetUnsignedVarIntSingleByte)
{
    // A value less than 128 should be encoded in one byte.
    std::string buffer = "\x05\x7F";
    ReadOnlyBinaryStream stream(buffer, false);
    auto result_a = stream.getUnsignedVarInt();
    EXPECT_TRUE(result_a.ignoreError());
    EXPECT_EQ(result_a.discardError().value(), 5U);

    auto result_b = stream.getUnsignedVarInt();
    EXPECT_TRUE(result_b.ignoreError());
    EXPECT_EQ(result_b.discardError().value(), 127U);
}

TEST(ReadOnlyBinaryStreamTest, GetUnsignedVarIntMultiByte)
{
    // Test decoding a multi-byte unsigned varint.
    std::string buffer = "\xAC\x02\xFF\xFF\x03";
    ReadOnlyBinaryStream stream(buffer, false);
    auto result_a = stream.getUnsignedVarInt();
    EXPECT_TRUE(result_a.ignoreError());
    EXPECT_EQ(result_a.discardError().value(), 300U);

    auto result_b = stream.getUnsignedVarInt();
    EXPECT_TRUE(result_b.ignoreError());
    EXPECT_EQ(result_b.discardError().value(), 65535U);
}

TEST(ReadOnlyBinaryStreamTest, ReadOverflowProducesError)
{
    std::string testData = "\x01";
    ReadOnlyBinaryStream stream(testData, true);

    // First byte should succeed.
    auto result1 = stream.getByte();
    EXPECT_TRUE(result1.ignoreError());
    EXPECT_FALSE(stream.hasOverflowed());

    // Second byte read should fail because the stream is overflowed.
    auto result2 = stream.getByte();
    EXPECT_FALSE(result2.ignoreError());
    EXPECT_TRUE(stream.hasOverflowed());
}
