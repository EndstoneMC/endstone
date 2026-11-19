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

#include "endstone/brigadier/exceptions.h"
#include "endstone/brigadier/reader.h"
#include "support.h"

namespace endstone::brigadier {
namespace {

using support::expectSyntaxError;

TEST(StringReaderTest, canRead)
{
    StringReader reader("abc");
    EXPECT_TRUE(reader.canRead());
    reader.skip();
    EXPECT_TRUE(reader.canRead());
    reader.skip();
    EXPECT_TRUE(reader.canRead());
    reader.skip();
    EXPECT_FALSE(reader.canRead());
}

TEST(StringReaderTest, getRemainingLength)
{
    StringReader reader("abc");
    EXPECT_EQ(reader.getRemainingLength(), 3);
    reader.setCursor(1);
    EXPECT_EQ(reader.getRemainingLength(), 2);
    reader.setCursor(2);
    EXPECT_EQ(reader.getRemainingLength(), 1);
    reader.setCursor(3);
    EXPECT_EQ(reader.getRemainingLength(), 0);
}

TEST(StringReaderTest, canReadLength)
{
    StringReader reader("abc");
    EXPECT_TRUE(reader.canRead(1));
    EXPECT_TRUE(reader.canRead(2));
    EXPECT_TRUE(reader.canRead(3));
    EXPECT_FALSE(reader.canRead(4));
    EXPECT_FALSE(reader.canRead(5));
}

TEST(StringReaderTest, peek)
{
    StringReader reader("abc");
    EXPECT_EQ(reader.peek(), 'a');
    EXPECT_EQ(reader.getCursor(), 0);
    reader.setCursor(2);
    EXPECT_EQ(reader.peek(), 'c');
    EXPECT_EQ(reader.getCursor(), 2);
}

TEST(StringReaderTest, peekLength)
{
    StringReader reader("abc");
    EXPECT_EQ(reader.peek(0), 'a');
    EXPECT_EQ(reader.peek(2), 'c');
    EXPECT_EQ(reader.getCursor(), 0);
    reader.setCursor(1);
    EXPECT_EQ(reader.peek(1), 'c');
    EXPECT_EQ(reader.getCursor(), 1);
}

TEST(StringReaderTest, read)
{
    StringReader reader("abc");
    EXPECT_EQ(reader.read(), 'a');
    EXPECT_EQ(reader.read(), 'b');
    EXPECT_EQ(reader.read(), 'c');
    EXPECT_EQ(reader.getCursor(), 3);
}

TEST(StringReaderTest, skip)
{
    StringReader reader("abc");
    reader.skip();
    EXPECT_EQ(reader.getCursor(), 1);
}

TEST(StringReaderTest, getRemaining)
{
    StringReader reader("Hello!");
    EXPECT_EQ(reader.getRemaining(), "Hello!");
    reader.setCursor(3);
    EXPECT_EQ(reader.getRemaining(), "lo!");
    reader.setCursor(6);
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, getRead)
{
    StringReader reader("Hello!");
    EXPECT_EQ(reader.getRead(), "");
    reader.setCursor(3);
    EXPECT_EQ(reader.getRead(), "Hel");
    reader.setCursor(6);
    EXPECT_EQ(reader.getRead(), "Hello!");
}

TEST(StringReaderTest, skipWhitespaceNone)
{
    StringReader reader("Hello!");
    reader.skipWhitespace();
    EXPECT_EQ(reader.getCursor(), 0);
}

TEST(StringReaderTest, skipWhitespaceMixed)
{
    StringReader reader(" \t \t\nHello!");
    reader.skipWhitespace();
    EXPECT_EQ(reader.getCursor(), 5);
}

TEST(StringReaderTest, skipWhitespaceEmpty)
{
    StringReader reader("");
    reader.skipWhitespace();
    EXPECT_EQ(reader.getCursor(), 0);
}

TEST(StringReaderTest, readUnquotedString)
{
    StringReader reader("hello world");
    EXPECT_EQ(reader.readUnquotedString(), "hello");
    EXPECT_EQ(reader.getRead(), "hello");
    EXPECT_EQ(reader.getRemaining(), " world");
}

TEST(StringReaderTest, readUnquotedStringEmpty)
{
    StringReader reader("");
    EXPECT_EQ(reader.readUnquotedString(), "");
    EXPECT_EQ(reader.getRead(), "");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readUnquotedStringEmptyWithRemaining)
{
    StringReader reader(" hello world");
    EXPECT_EQ(reader.readUnquotedString(), "");
    EXPECT_EQ(reader.getRead(), "");
    EXPECT_EQ(reader.getRemaining(), " hello world");
}

TEST(StringReaderTest, readQuotedString)
{
    StringReader reader("\"hello world\"");
    EXPECT_EQ(reader.readQuotedString(), "hello world");
    EXPECT_EQ(reader.getRead(), "\"hello world\"");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readSingleQuotedString)
{
    StringReader reader("'hello world'");
    EXPECT_EQ(reader.readQuotedString(), "hello world");
    EXPECT_EQ(reader.getRead(), "'hello world'");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readMixedQuotedStringDoubleInsideSingle)
{
    StringReader reader("'hello \"world\"'");
    EXPECT_EQ(reader.readQuotedString(), "hello \"world\"");
    EXPECT_EQ(reader.getRead(), "'hello \"world\"'");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readMixedQuotedStringSingleInsideDouble)
{
    StringReader reader("\"hello 'world'\"");
    EXPECT_EQ(reader.readQuotedString(), "hello 'world'");
    EXPECT_EQ(reader.getRead(), "\"hello 'world'\"");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readQuotedStringEmpty)
{
    StringReader reader("");
    EXPECT_EQ(reader.readQuotedString(), "");
    EXPECT_EQ(reader.getRead(), "");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readQuotedStringEmptyQuoted)
{
    StringReader reader("\"\"");
    EXPECT_EQ(reader.readQuotedString(), "");
    EXPECT_EQ(reader.getRead(), "\"\"");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readQuotedStringEmptyQuotedWithRemaining)
{
    StringReader reader("\"\" hello world");
    EXPECT_EQ(reader.readQuotedString(), "");
    EXPECT_EQ(reader.getRead(), "\"\"");
    EXPECT_EQ(reader.getRemaining(), " hello world");
}

TEST(StringReaderTest, readQuotedStringWithEscapedQuote)
{
    StringReader reader(R"("hello \"world\"")");
    EXPECT_EQ(reader.readQuotedString(), "hello \"world\"");
    EXPECT_EQ(reader.getRead(), R"("hello \"world\"")");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readQuotedStringWithEscapedEscapes)
{
    StringReader reader(R"("\\o/")");
    EXPECT_EQ(reader.readQuotedString(), "\\o/");
    EXPECT_EQ(reader.getRead(), R"("\\o/")");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readQuotedStringWithRemaining)
{
    StringReader reader("\"hello world\" foo bar");
    EXPECT_EQ(reader.readQuotedString(), "hello world");
    EXPECT_EQ(reader.getRead(), "\"hello world\"");
    EXPECT_EQ(reader.getRemaining(), " foo bar");
}

TEST(StringReaderTest, readQuotedStringWithImmediateRemaining)
{
    StringReader reader("\"hello world\"foo bar");
    EXPECT_EQ(reader.readQuotedString(), "hello world");
    EXPECT_EQ(reader.getRead(), "\"hello world\"");
    EXPECT_EQ(reader.getRemaining(), "foo bar");
}

TEST(StringReaderTest, readQuotedStringNoOpen)
{
    StringReader reader("hello world\"");
    expectSyntaxError([&] { reader.readQuotedString(); },
                      CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedStartOfQuote(), 0);
}

TEST(StringReaderTest, readQuotedStringNoClose)
{
    StringReader reader("\"hello world");
    expectSyntaxError([&] { reader.readQuotedString(); },
                      CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedEndOfQuote(), 12);
}

TEST(StringReaderTest, readQuotedStringInvalidEscape)
{
    StringReader reader(R"("hello\nworld")");
    expectSyntaxError([&] { reader.readQuotedString(); },
                      CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerInvalidEscape(), 7);
}

TEST(StringReaderTest, readQuotedStringInvalidQuoteEscape)
{
    StringReader reader("'hello\\\"'world");
    expectSyntaxError([&] { reader.readQuotedString(); },
                      CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerInvalidEscape(), 7);
}

TEST(StringReaderTest, readStringNoQuotes)
{
    StringReader reader("hello world");
    EXPECT_EQ(reader.readString(), "hello");
    EXPECT_EQ(reader.getRead(), "hello");
    EXPECT_EQ(reader.getRemaining(), " world");
}

TEST(StringReaderTest, readStringSingleQuotes)
{
    StringReader reader("'hello world'");
    EXPECT_EQ(reader.readString(), "hello world");
    EXPECT_EQ(reader.getRead(), "'hello world'");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readStringDoubleQuotes)
{
    StringReader reader("\"hello world\"");
    EXPECT_EQ(reader.readString(), "hello world");
    EXPECT_EQ(reader.getRead(), "\"hello world\"");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readInt)
{
    StringReader reader("1234567890");
    EXPECT_EQ(reader.readInt(), 1234567890);
    EXPECT_EQ(reader.getRead(), "1234567890");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readIntNegative)
{
    StringReader reader("-1234567890");
    EXPECT_EQ(reader.readInt(), -1234567890);
    EXPECT_EQ(reader.getRead(), "-1234567890");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readIntInvalid)
{
    StringReader reader("12.34");
    expectSyntaxError([&] { reader.readInt(); }, CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerInvalidInt(), 0);
}

TEST(StringReaderTest, readIntNone)
{
    StringReader reader("");
    expectSyntaxError([&] { reader.readInt(); }, CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedInt(), 0);
}

TEST(StringReaderTest, readIntWithRemaining)
{
    StringReader reader("1234567890 foo bar");
    EXPECT_EQ(reader.readInt(), 1234567890);
    EXPECT_EQ(reader.getRead(), "1234567890");
    EXPECT_EQ(reader.getRemaining(), " foo bar");
}

TEST(StringReaderTest, readIntWithRemainingImmediate)
{
    StringReader reader("1234567890foo bar");
    EXPECT_EQ(reader.readInt(), 1234567890);
    EXPECT_EQ(reader.getRead(), "1234567890");
    EXPECT_EQ(reader.getRemaining(), "foo bar");
}

TEST(StringReaderTest, readLong)
{
    StringReader reader("1234567890");
    EXPECT_EQ(reader.readLong(), 1234567890);
    EXPECT_EQ(reader.getRead(), "1234567890");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readLongNegative)
{
    StringReader reader("-1234567890");
    EXPECT_EQ(reader.readLong(), -1234567890);
    EXPECT_EQ(reader.getRead(), "-1234567890");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readLongInvalid)
{
    StringReader reader("12.34");
    expectSyntaxError([&] { reader.readLong(); }, CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerInvalidLong(), 0);
}

TEST(StringReaderTest, readLongNone)
{
    StringReader reader("");
    expectSyntaxError([&] { reader.readLong(); }, CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedLong(), 0);
}

TEST(StringReaderTest, readLongWithRemaining)
{
    StringReader reader("1234567890 foo bar");
    EXPECT_EQ(reader.readLong(), 1234567890);
    EXPECT_EQ(reader.getRead(), "1234567890");
    EXPECT_EQ(reader.getRemaining(), " foo bar");
}

TEST(StringReaderTest, readLongWithRemainingImmediate)
{
    StringReader reader("1234567890foo bar");
    EXPECT_EQ(reader.readLong(), 1234567890);
    EXPECT_EQ(reader.getRead(), "1234567890");
    EXPECT_EQ(reader.getRemaining(), "foo bar");
}

TEST(StringReaderTest, readDouble)
{
    StringReader reader("123");
    EXPECT_DOUBLE_EQ(reader.readDouble(), 123.0);
    EXPECT_EQ(reader.getRead(), "123");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readDoubleWithDecimal)
{
    StringReader reader("12.34");
    EXPECT_DOUBLE_EQ(reader.readDouble(), 12.34);
    EXPECT_EQ(reader.getRead(), "12.34");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readDoubleNegative)
{
    StringReader reader("-123");
    EXPECT_DOUBLE_EQ(reader.readDouble(), -123.0);
    EXPECT_EQ(reader.getRead(), "-123");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readDoubleInvalid)
{
    StringReader reader("12.34.56");
    expectSyntaxError([&] { reader.readDouble(); }, CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerInvalidDouble(),
                      0);
}

TEST(StringReaderTest, readDoubleNone)
{
    StringReader reader("");
    expectSyntaxError([&] { reader.readDouble(); }, CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedDouble(),
                      0);
}

TEST(StringReaderTest, readDoubleWithRemaining)
{
    StringReader reader("12.34 foo bar");
    EXPECT_DOUBLE_EQ(reader.readDouble(), 12.34);
    EXPECT_EQ(reader.getRead(), "12.34");
    EXPECT_EQ(reader.getRemaining(), " foo bar");
}

TEST(StringReaderTest, readDoubleWithRemainingImmediate)
{
    StringReader reader("12.34foo bar");
    EXPECT_DOUBLE_EQ(reader.readDouble(), 12.34);
    EXPECT_EQ(reader.getRead(), "12.34");
    EXPECT_EQ(reader.getRemaining(), "foo bar");
}

TEST(StringReaderTest, readFloat)
{
    StringReader reader("123");
    EXPECT_FLOAT_EQ(reader.readFloat(), 123.0F);
    EXPECT_EQ(reader.getRead(), "123");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readFloatWithDecimal)
{
    StringReader reader("12.34");
    EXPECT_FLOAT_EQ(reader.readFloat(), 12.34F);
    EXPECT_EQ(reader.getRead(), "12.34");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readFloatNegative)
{
    StringReader reader("-123");
    EXPECT_FLOAT_EQ(reader.readFloat(), -123.0F);
    EXPECT_EQ(reader.getRead(), "-123");
    EXPECT_EQ(reader.getRemaining(), "");
}

TEST(StringReaderTest, readFloatInvalid)
{
    StringReader reader("12.34.56");
    expectSyntaxError([&] { reader.readFloat(); }, CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerInvalidFloat(), 0);
}

TEST(StringReaderTest, readFloatNone)
{
    StringReader reader("");
    expectSyntaxError([&] { reader.readFloat(); }, CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedFloat(),
                      0);
}

TEST(StringReaderTest, readFloatWithRemaining)
{
    StringReader reader("12.34 foo bar");
    EXPECT_FLOAT_EQ(reader.readFloat(), 12.34F);
    EXPECT_EQ(reader.getRead(), "12.34");
    EXPECT_EQ(reader.getRemaining(), " foo bar");
}

TEST(StringReaderTest, readFloatWithRemainingImmediate)
{
    StringReader reader("12.34foo bar");
    EXPECT_FLOAT_EQ(reader.readFloat(), 12.34F);
    EXPECT_EQ(reader.getRead(), "12.34");
    EXPECT_EQ(reader.getRemaining(), "foo bar");
}

TEST(StringReaderTest, expectCorrect)
{
    StringReader reader("abc");
    reader.expect('a');
    EXPECT_EQ(reader.getCursor(), 1);
}

TEST(StringReaderTest, expectIncorrect)
{
    StringReader reader("bcd");
    expectSyntaxError([&] { reader.expect('a'); }, CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedSymbol(),
                      0);
}

TEST(StringReaderTest, expectNone)
{
    StringReader reader("");
    expectSyntaxError([&] { reader.expect('a'); }, CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedSymbol(),
                      0);
}

TEST(StringReaderTest, readBooleanCorrect)
{
    StringReader reader("true");
    EXPECT_TRUE(reader.readBoolean());
    EXPECT_EQ(reader.getRead(), "true");
}

TEST(StringReaderTest, readBooleanIncorrect)
{
    StringReader reader("tuesday");
    expectSyntaxError([&] { reader.readBoolean(); }, CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerInvalidBool(),
                      0);
}

TEST(StringReaderTest, readBooleanNone)
{
    StringReader reader("");
    expectSyntaxError([&] { reader.readBoolean(); }, CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedBool(),
                      0);
}

}  // namespace
}  // namespace endstone::brigadier
