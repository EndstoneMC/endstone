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

#include <algorithm>
#include <charconv>
#include <exception>
#include <functional>
#include <optional>
#include <string>
#include <system_error>

#include "endstone/brigadier/reader.h"

namespace endstone::brigadier {

class BuiltInExceptions;

/**
 * @see com.mojang.brigadier.exceptions.CommandExceptionType
 */
class CommandExceptionType {
public:
    CommandExceptionType() = default;
    CommandExceptionType(const CommandExceptionType &) = default;
    CommandExceptionType &operator=(const CommandExceptionType &) = default;
    virtual ~CommandExceptionType() = default;
};

/**
 * The error a command fails with, carrying the input and the cursor it stopped at.
 *
 * @see com.mojang.brigadier.exceptions.CommandSyntaxException
 */
class CommandSyntaxException : public std::exception {
public:
    static constexpr int CONTEXT_AMOUNT = 10;
    static const BuiltInExceptions BUILT_IN_EXCEPTIONS;

    CommandSyntaxException(const CommandExceptionType &type, Message message)
        : type_(&type), message_(std::move(message)), full_message_(message_)
    {
    }

    CommandSyntaxException(const CommandExceptionType &type, Message message, std::string input, int cursor)
        : type_(&type), message_(std::move(message)), input_(std::move(input)), cursor_(cursor)
    {
        full_message_ = message_;
        if (const auto context = getContext()) {
            full_message_ += " at position " + std::to_string(cursor_) + ": " + *context;
        }
    }

    [[nodiscard]] const char *what() const noexcept override { return full_message_.c_str(); }
    [[nodiscard]] std::string getMessage() const { return full_message_; }
    [[nodiscard]] const Message &getRawMessage() const { return message_; }

    [[nodiscard]] std::optional<std::string> getContext() const
    {
        if (!input_ || cursor_ < 0) {
            return std::nullopt;
        }
        std::string builder;
        const int cursor = std::min(static_cast<int>(input_->length()), cursor_);
        if (cursor > CONTEXT_AMOUNT) {
            builder += "...";
        }
        const int start = std::max(0, cursor - CONTEXT_AMOUNT);
        builder += input_->substr(start, cursor - start);
        builder += "<--[HERE]";
        return builder;
    }

    [[nodiscard]] const CommandExceptionType &getType() const { return *type_; }
    [[nodiscard]] const std::optional<std::string> &getInput() const { return input_; }
    [[nodiscard]] int getCursor() const { return cursor_; }

private:
    const CommandExceptionType *type_;
    Message message_;
    std::string full_message_;
    std::optional<std::string> input_;
    int cursor_ = -1;
};

/**
 * @see com.mojang.brigadier.exceptions.SimpleCommandExceptionType
 */
class SimpleCommandExceptionType : public CommandExceptionType {
public:
    explicit SimpleCommandExceptionType(Message message) : message_(std::move(message)) {}

    [[nodiscard]] CommandSyntaxException create() const { return {*this, message_}; }

    [[nodiscard]] CommandSyntaxException createWithContext(const StringReader &reader) const
    {
        return {*this, message_, reader.getString(), reader.getCursor()};
    }

    [[nodiscard]] std::string toString() const { return message_; }

private:
    Message message_;
};

/**
 * @see com.mojang.brigadier.exceptions.DynamicCommandExceptionType
 */
class DynamicCommandExceptionType : public CommandExceptionType {
public:
    explicit DynamicCommandExceptionType(std::function<Message(const std::string &)> function)
        : function_(std::move(function))
    {
    }

    [[nodiscard]] CommandSyntaxException create(const std::string &arg) const { return {*this, function_(arg)}; }

    [[nodiscard]] CommandSyntaxException createWithContext(const StringReader &reader, const std::string &arg) const
    {
        return {*this, function_(arg), reader.getString(), reader.getCursor()};
    }

private:
    std::function<Message(const std::string &)> function_;
};

/**
 * @see com.mojang.brigadier.exceptions.Dynamic2CommandExceptionType
 */
class Dynamic2CommandExceptionType : public CommandExceptionType {
public:
    explicit Dynamic2CommandExceptionType(std::function<Message(const std::string &, const std::string &)> function)
        : function_(std::move(function))
    {
    }

    [[nodiscard]] CommandSyntaxException create(const std::string &a, const std::string &b) const
    {
        return {*this, function_(a, b)};
    }

    [[nodiscard]] CommandSyntaxException createWithContext(const StringReader &reader, const std::string &a,
                                                           const std::string &b) const
    {
        return {*this, function_(a, b), reader.getString(), reader.getCursor()};
    }

private:
    std::function<Message(const std::string &, const std::string &)> function_;
};

/**
 * @see com.mojang.brigadier.exceptions.BuiltInExceptions
 * @see com.mojang.brigadier.exceptions.BuiltInExceptionProvider
 */
class BuiltInExceptions {
public:
    [[nodiscard]] const Dynamic2CommandExceptionType &doubleTooLow() const { return double_too_small_; }
    [[nodiscard]] const Dynamic2CommandExceptionType &doubleTooHigh() const { return double_too_big_; }
    [[nodiscard]] const Dynamic2CommandExceptionType &floatTooLow() const { return float_too_small_; }
    [[nodiscard]] const Dynamic2CommandExceptionType &floatTooHigh() const { return float_too_big_; }
    [[nodiscard]] const Dynamic2CommandExceptionType &integerTooLow() const { return integer_too_small_; }
    [[nodiscard]] const Dynamic2CommandExceptionType &integerTooHigh() const { return integer_too_big_; }
    [[nodiscard]] const Dynamic2CommandExceptionType &longTooLow() const { return long_too_small_; }
    [[nodiscard]] const Dynamic2CommandExceptionType &longTooHigh() const { return long_too_big_; }

    [[nodiscard]] const DynamicCommandExceptionType &literalIncorrect() const { return literal_incorrect_; }

    [[nodiscard]] const SimpleCommandExceptionType &readerExpectedStartOfQuote() const
    {
        return reader_expected_start_of_quote_;
    }

    [[nodiscard]] const SimpleCommandExceptionType &readerExpectedEndOfQuote() const
    {
        return reader_expected_end_of_quote_;
    }

    [[nodiscard]] const DynamicCommandExceptionType &readerInvalidEscape() const { return reader_invalid_escape_; }
    [[nodiscard]] const DynamicCommandExceptionType &readerInvalidBool() const { return reader_invalid_bool_; }
    [[nodiscard]] const DynamicCommandExceptionType &readerInvalidInt() const { return reader_invalid_int_; }
    [[nodiscard]] const SimpleCommandExceptionType &readerExpectedInt() const { return reader_expected_int_; }
    [[nodiscard]] const DynamicCommandExceptionType &readerInvalidLong() const { return reader_invalid_long_; }
    [[nodiscard]] const SimpleCommandExceptionType &readerExpectedLong() const { return reader_expected_long_; }
    [[nodiscard]] const DynamicCommandExceptionType &readerInvalidDouble() const { return reader_invalid_double_; }
    [[nodiscard]] const SimpleCommandExceptionType &readerExpectedDouble() const { return reader_expected_double_; }
    [[nodiscard]] const DynamicCommandExceptionType &readerInvalidFloat() const { return reader_invalid_float_; }
    [[nodiscard]] const SimpleCommandExceptionType &readerExpectedFloat() const { return reader_expected_float_; }
    [[nodiscard]] const SimpleCommandExceptionType &readerExpectedBool() const { return reader_expected_bool_; }
    [[nodiscard]] const DynamicCommandExceptionType &readerExpectedSymbol() const { return reader_expected_symbol_; }

    [[nodiscard]] const SimpleCommandExceptionType &dispatcherUnknownCommand() const
    {
        return dispatcher_unknown_command_;
    }

    [[nodiscard]] const SimpleCommandExceptionType &dispatcherUnknownArgument() const
    {
        return dispatcher_unknown_argument_;
    }

    [[nodiscard]] const SimpleCommandExceptionType &dispatcherExpectedArgumentSeparator() const
    {
        return dispatcher_expected_argument_separator_;
    }

    [[nodiscard]] const DynamicCommandExceptionType &dispatcherParseException() const
    {
        return dispatcher_parse_exception_;
    }

private:
    Dynamic2CommandExceptionType double_too_small_{[](const std::string &found, const std::string &min) {
        return "Double must not be less than " + min + ", found " + found;
    }};
    Dynamic2CommandExceptionType double_too_big_{[](const std::string &found, const std::string &max) {
        return "Double must not be more than " + max + ", found " + found;
    }};
    Dynamic2CommandExceptionType float_too_small_{[](const std::string &found, const std::string &min) {
        return "Float must not be less than " + min + ", found " + found;
    }};
    Dynamic2CommandExceptionType float_too_big_{[](const std::string &found, const std::string &max) {
        return "Float must not be more than " + max + ", found " + found;
    }};
    Dynamic2CommandExceptionType integer_too_small_{[](const std::string &found, const std::string &min) {
        return "Integer must not be less than " + min + ", found " + found;
    }};
    Dynamic2CommandExceptionType integer_too_big_{[](const std::string &found, const std::string &max) {
        return "Integer must not be more than " + max + ", found " + found;
    }};
    Dynamic2CommandExceptionType long_too_small_{[](const std::string &found, const std::string &min) {
        return "Long must not be less than " + min + ", found " + found;
    }};
    Dynamic2CommandExceptionType long_too_big_{[](const std::string &found, const std::string &max) {
        return "Long must not be more than " + max + ", found " + found;
    }};
    DynamicCommandExceptionType literal_incorrect_{
        [](const std::string &expected) { return "Expected literal " + expected; }};
    SimpleCommandExceptionType reader_expected_start_of_quote_{"Expected quote to start a string"};
    SimpleCommandExceptionType reader_expected_end_of_quote_{"Unclosed quoted string"};
    DynamicCommandExceptionType reader_invalid_escape_{
        [](const std::string &character) { return "Invalid escape sequence '" + character + "' in quoted string"; }};
    DynamicCommandExceptionType reader_invalid_bool_{
        [](const std::string &value) { return "Invalid bool, expected true or false but found '" + value + "'"; }};
    DynamicCommandExceptionType reader_invalid_int_{
        [](const std::string &value) { return "Invalid integer '" + value + "'"; }};
    SimpleCommandExceptionType reader_expected_int_{"Expected integer"};
    DynamicCommandExceptionType reader_invalid_long_{
        [](const std::string &value) { return "Invalid long '" + value + "'"; }};
    SimpleCommandExceptionType reader_expected_long_{"Expected long"};
    DynamicCommandExceptionType reader_invalid_double_{
        [](const std::string &value) { return "Invalid double '" + value + "'"; }};
    SimpleCommandExceptionType reader_expected_double_{"Expected double"};
    DynamicCommandExceptionType reader_invalid_float_{
        [](const std::string &value) { return "Invalid float '" + value + "'"; }};
    SimpleCommandExceptionType reader_expected_float_{"Expected float"};
    SimpleCommandExceptionType reader_expected_bool_{"Expected bool"};
    DynamicCommandExceptionType reader_expected_symbol_{
        [](const std::string &symbol) { return "Expected '" + symbol + "'"; }};
    SimpleCommandExceptionType dispatcher_unknown_command_{"Unknown command"};
    SimpleCommandExceptionType dispatcher_unknown_argument_{"Incorrect argument for command"};
    SimpleCommandExceptionType dispatcher_expected_argument_separator_{
        "Expected whitespace to end one argument, but found trailing data"};
    DynamicCommandExceptionType dispatcher_parse_exception_{
        [](const std::string &message) { return "Could not parse command: " + message; }};
};

inline const BuiltInExceptions CommandSyntaxException::BUILT_IN_EXCEPTIONS{};

inline bool StringReader::isAllowedNumber(char c)
{
    return (c >= '0' && c <= '9') || c == '.' || c == '-';
}

inline bool StringReader::isQuotedStringStart(char c)
{
    return c == SYNTAX_DOUBLE_QUOTE || c == SYNTAX_SINGLE_QUOTE;
}

inline void StringReader::skipWhitespace()
{
    while (canRead() && (peek() == ' ' || (peek() >= '\t' && peek() <= '\r') || (peek() >= 0x1C && peek() <= 0x1F))) {
        skip();
    }
}

template <typename T>
inline T StringReader::readNumber(const SimpleCommandExceptionType &expected,
                                  const DynamicCommandExceptionType &invalid)
{
    const int start = cursor_;
    while (canRead() && isAllowedNumber(peek())) {
        skip();
    }
    const std::string number = string_.substr(start, cursor_ - start);
    if (number.empty()) {
        throw expected.createWithContext(*this);
    }
    T value{};
    const char *end = number.data() + number.size();
    const auto result = std::from_chars(number.data(), end, value);
    if (result.ec != std::errc{} || result.ptr != end) {
        cursor_ = start;
        throw invalid.createWithContext(*this, number);
    }
    return value;
}

inline int StringReader::readInt()
{
    return readNumber<int>(CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedInt(),
                           CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerInvalidInt());
}

inline std::int64_t StringReader::readLong()
{
    return readNumber<std::int64_t>(CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedLong(),
                                    CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerInvalidLong());
}

inline double StringReader::readDouble()
{
    return readNumber<double>(CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedDouble(),
                              CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerInvalidDouble());
}

inline float StringReader::readFloat()
{
    return readNumber<float>(CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedFloat(),
                             CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerInvalidFloat());
}

inline bool StringReader::isAllowedInUnquotedString(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || c == '-' ||
           c == '.' || c == '+';
}

inline std::string StringReader::readUnquotedString()
{
    const int start = cursor_;
    while (canRead() && isAllowedInUnquotedString(peek())) {
        skip();
    }
    return string_.substr(start, cursor_ - start);
}

inline std::string StringReader::readQuotedString()
{
    if (!canRead()) {
        return "";
    }
    const char next = peek();
    if (!isQuotedStringStart(next)) {
        throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedStartOfQuote().createWithContext(*this);
    }
    skip();
    return readStringUntil(next);
}

inline std::string StringReader::readStringUntil(char terminator)
{
    std::string result;
    bool escaped = false;
    while (canRead()) {
        const char c = read();
        if (escaped) {
            if (c == terminator || c == SYNTAX_ESCAPE) {
                result += c;
                escaped = false;
            }
            else {
                setCursor(getCursor() - 1);
                throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerInvalidEscape().createWithContext(
                    *this, std::string(1, c));
            }
        }
        else if (c == SYNTAX_ESCAPE) {
            escaped = true;
        }
        else if (c == terminator) {
            return result;
        }
        else {
            result += c;
        }
    }
    throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedEndOfQuote().createWithContext(*this);
}

inline std::string StringReader::readString()
{
    if (!canRead()) {
        return "";
    }
    const char next = peek();
    if (isQuotedStringStart(next)) {
        skip();
        return readStringUntil(next);
    }
    return readUnquotedString();
}

inline bool StringReader::readBoolean()
{
    const int start = cursor_;
    const std::string value = readString();
    if (value.empty()) {
        throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedBool().createWithContext(*this);
    }
    if (value == "true") {
        return true;
    }
    if (value == "false") {
        return false;
    }
    cursor_ = start;
    throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerInvalidBool().createWithContext(*this, value);
}

inline void StringReader::expect(char c)
{
    if (!canRead() || peek() != c) {
        throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.readerExpectedSymbol().createWithContext(*this,
                                                                                                   std::string(1, c));
    }
    skip();
}

}  // namespace endstone::brigadier
