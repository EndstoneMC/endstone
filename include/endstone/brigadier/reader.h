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

#pragma once

#include <cstdint>
#include <string>
#include <utility>

namespace endstone::brigadier {

class SimpleCommandExceptionType;
class DynamicCommandExceptionType;

using Message = std::string;

/**
 * Reads a command line one character at a time, with the cursor Brigadier's arguments parse from.
 *
 * @see com.mojang.brigadier.StringReader
 * @see com.mojang.brigadier.ImmutableStringReader
 */
class StringReader {
public:
    StringReader(const StringReader &other) = default;
    explicit StringReader(std::string string) : string_(std::move(string)) {}

    [[nodiscard]] const std::string &getString() const { return string_; }
    void setCursor(int cursor) { cursor_ = cursor; }
    [[nodiscard]] int getRemainingLength() const { return getTotalLength() - cursor_; }
    [[nodiscard]] int getTotalLength() const { return static_cast<int>(string_.length()); }
    [[nodiscard]] int getCursor() const { return cursor_; }
    [[nodiscard]] std::string getRead() const { return string_.substr(0, cursor_); }
    [[nodiscard]] std::string getRemaining() const { return string_.substr(cursor_); }
    [[nodiscard]] bool canRead(int length) const { return cursor_ + length <= getTotalLength(); }
    [[nodiscard]] bool canRead() const { return canRead(1); }
    [[nodiscard]] char peek() const { return string_.at(cursor_); }
    [[nodiscard]] char peek(int offset) const { return string_.at(cursor_ + offset); }
    char read() { return string_.at(cursor_++); }
    void skip() { cursor_++; }

    static bool isAllowedNumber(char c);
    static bool isQuotedStringStart(char c);
    void skipWhitespace();
    int readInt();
    std::int64_t readLong();
    double readDouble();
    float readFloat();
    static bool isAllowedInUnquotedString(char c);
    std::string readUnquotedString();
    std::string readQuotedString();
    std::string readStringUntil(char terminator);
    std::string readString();
    bool readBoolean();
    void expect(char c);

private:
    template <typename T>
    T readNumber(const SimpleCommandExceptionType &expected, const DynamicCommandExceptionType &invalid);

    static constexpr char SYNTAX_ESCAPE = '\\';
    static constexpr char SYNTAX_DOUBLE_QUOTE = '"';
    static constexpr char SYNTAX_SINGLE_QUOTE = '\'';

    std::string string_;
    int cursor_ = 0;
};

}  // namespace endstone::brigadier

#include "endstone/brigadier/exceptions.h"
