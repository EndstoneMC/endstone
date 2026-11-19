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

#include <array>
#include <charconv>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <limits>
#include <memory>
#include <string>
#include <string_view>
#include <typeinfo>
#include <variant>
#include <vector>

#include "endstone/brigadier/exceptions.h"
#include "endstone/brigadier/reader.h"
#include "endstone/command/brigadier/argument/resolvers/block_position_resolver.h"
#include "endstone/command/brigadier/argument/resolvers/fine_position_resolver.h"
#include "endstone/command/brigadier/argument/resolvers/selector/actor_selector_argument_resolver.h"
#include "endstone/command/brigadier/argument/resolvers/selector/player_selector_argument_resolver.h"
#include "endstone/util/pointers.h"

namespace endstone {

class ActorType;
class BlockType;
class Dimension;
class ItemType;

}  // namespace endstone

namespace endstone::brigadier {

template <typename S>
class CommandContext;

using ArgumentValue = std::variant<bool, int, std::int64_t, float, double, std::string, PlayerSelectorArgumentResolver,
                                   ActorSelectorArgumentResolver, BlockPositionResolver, FinePositionResolver,
                                   NotNull<Dimension>, const BlockType *, const ItemType *, const ActorType *>;

/**
 * Turns the text at the reader's cursor into an argument's value.
 *
 * @see com.mojang.brigadier.arguments.ArgumentType
 */
class ArgumentType {
public:
    ArgumentType() = default;
    ArgumentType(const ArgumentType &) = default;
    ArgumentType &operator=(const ArgumentType &) = default;
    virtual ~ArgumentType() = default;

    [[nodiscard]] virtual ArgumentValue parse(StringReader &reader) const = 0;
    [[nodiscard]] virtual std::vector<std::string> getExamples() const { return {}; }
    [[nodiscard]] virtual std::string toString() const = 0;

    [[nodiscard]] virtual bool isInstanceOf(const std::type_info &target) const
    {
        return std::string_view(target.name()) == typeid(ArgumentType).name();
    }

    template <std::derived_from<ArgumentType> T>
    [[nodiscard]] T *as()
    {
        return is<T>() ? static_cast<T *>(this) : nullptr;
    }

    template <std::derived_from<ArgumentType> T>
    [[nodiscard]] const T *as() const
    {
        return is<T>() ? static_cast<const T *>(this) : nullptr;
    }

    template <std::derived_from<ArgumentType> T>
    [[nodiscard]] bool is() const
    {
        return isInstanceOf(typeid(T));
    }

protected:
    template <std::floating_point T>
    [[nodiscard]] static std::string toJavaString(T value)
    {
        if (std::isnan(value)) {
            return "NaN";
        }
        if (std::isinf(value)) {
            return value > 0 ? "Infinity" : "-Infinity";
        }
        std::array<char, 64> buffer{};
        const auto magnitude = std::fabs(value);
        if (magnitude == 0 || (magnitude >= static_cast<T>(1e-3) && magnitude < static_cast<T>(1e7))) {
            const auto *end =
                std::to_chars(buffer.data(), buffer.data() + buffer.size(), value, std::chars_format::fixed).ptr;
            std::string text(buffer.data(), end - buffer.data());
            if (text.find('.') == std::string::npos) {
                text += ".0";
            }
            return text;
        }
        const auto *end =
            std::to_chars(buffer.data(), buffer.data() + buffer.size(), value, std::chars_format::scientific).ptr;
        const std::string text(buffer.data(), end - buffer.data());
        const auto e = text.find('e');
        auto mantissa = text.substr(0, e);
        if (mantissa.find('.') == std::string::npos) {
            mantissa += ".0";
        }
        const auto *digits = text.data() + e + 1;
        if (*digits == '+') {
            ++digits;
        }
        int exponent = 0;
        std::from_chars(digits, text.data() + text.size(), exponent);
        return mantissa + "E" + std::to_string(exponent);
    }
};

/**
 * @see com.mojang.brigadier.arguments.BoolArgumentType
 */
class BoolArgumentType : public ArgumentType {
    struct Passkey {
        explicit Passkey() = default;
    };

public:
    static std::shared_ptr<BoolArgumentType> bool_() { return std::make_shared<BoolArgumentType>(Passkey{}); }

    template <typename S>
    static bool getBool(const CommandContext<S> &context, const std::string &name)
    {
        return context.template getArgument<bool>(name);
    }

    explicit BoolArgumentType(Passkey) {}

    [[nodiscard]] bool isInstanceOf(const std::type_info &target) const override
    {
        return std::string_view(target.name()) == typeid(BoolArgumentType).name() || ArgumentType::isInstanceOf(target);
    }

    [[nodiscard]] ArgumentValue parse(StringReader &reader) const override { return reader.readBoolean(); }
    [[nodiscard]] std::string toString() const override { return "bool()"; }
    [[nodiscard]] std::vector<std::string> getExamples() const override { return {"true", "false"}; }
};

/**
 * @see com.mojang.brigadier.arguments.IntegerArgumentType
 */
class IntegerArgumentType : public ArgumentType {
    struct Passkey {
        explicit Passkey() = default;
    };

public:
    static std::shared_ptr<IntegerArgumentType> integer(int min = std::numeric_limits<int>::min(),
                                                        int max = std::numeric_limits<int>::max())
    {
        return std::make_shared<IntegerArgumentType>(Passkey{}, min, max);
    }

    template <typename S>
    static int getInteger(const CommandContext<S> &context, const std::string &name)
    {
        return context.template getArgument<int>(name);
    }

    IntegerArgumentType(Passkey, int minimum, int maximum) : minimum_(minimum), maximum_(maximum) {}

    [[nodiscard]] int getMinimum() const { return minimum_; }
    [[nodiscard]] int getMaximum() const { return maximum_; }

    [[nodiscard]] bool isInstanceOf(const std::type_info &target) const override
    {
        return std::string_view(target.name()) == typeid(IntegerArgumentType).name() ||
               ArgumentType::isInstanceOf(target);
    }

    [[nodiscard]] ArgumentValue parse(StringReader &reader) const override
    {
        const int start = reader.getCursor();
        const int result = reader.readInt();
        if (result < minimum_) {
            reader.setCursor(start);
            throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.integerTooLow().createWithContext(
                reader, std::to_string(result), std::to_string(minimum_));
        }
        if (result > maximum_) {
            reader.setCursor(start);
            throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.integerTooHigh().createWithContext(
                reader, std::to_string(result), std::to_string(maximum_));
        }
        return result;
    }

    bool operator==(const IntegerArgumentType &other) const
    {
        return minimum_ == other.minimum_ && maximum_ == other.maximum_;
    }

    [[nodiscard]] std::string toString() const override
    {
        if (minimum_ == std::numeric_limits<int>::min() && maximum_ == std::numeric_limits<int>::max()) {
            return "integer()";
        }
        if (maximum_ == std::numeric_limits<int>::max()) {
            return "integer(" + std::to_string(minimum_) + ")";
        }
        return "integer(" + std::to_string(minimum_) + ", " + std::to_string(maximum_) + ")";
    }

    [[nodiscard]] std::vector<std::string> getExamples() const override { return {"0", "123", "-123"}; }

private:
    int minimum_;
    int maximum_;
};

/**
 * @see com.mojang.brigadier.arguments.FloatArgumentType
 */
class FloatArgumentType : public ArgumentType {
    struct Passkey {
        explicit Passkey() = default;
    };

public:
    static std::shared_ptr<FloatArgumentType> floatArg(float min = -std::numeric_limits<float>::max(),
                                                       float max = std::numeric_limits<float>::max())
    {
        return std::make_shared<FloatArgumentType>(Passkey{}, min, max);
    }

    template <typename S>
    static float getFloat(const CommandContext<S> &context, const std::string &name)
    {
        return context.template getArgument<float>(name);
    }

    FloatArgumentType(Passkey, float minimum, float maximum) : minimum_(minimum), maximum_(maximum) {}

    [[nodiscard]] float getMinimum() const { return minimum_; }
    [[nodiscard]] float getMaximum() const { return maximum_; }

    [[nodiscard]] bool isInstanceOf(const std::type_info &target) const override
    {
        return std::string_view(target.name()) == typeid(FloatArgumentType).name() ||
               ArgumentType::isInstanceOf(target);
    }

    [[nodiscard]] ArgumentValue parse(StringReader &reader) const override
    {
        const int start = reader.getCursor();
        const float result = reader.readFloat();
        if (result < minimum_) {
            reader.setCursor(start);
            throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.floatTooLow().createWithContext(
                reader, toJavaString(result), toJavaString(minimum_));
        }
        if (result > maximum_) {
            reader.setCursor(start);
            throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.floatTooHigh().createWithContext(
                reader, toJavaString(result), toJavaString(maximum_));
        }
        return result;
    }

    bool operator==(const FloatArgumentType &other) const
    {
        return minimum_ == other.minimum_ && maximum_ == other.maximum_;
    }

    [[nodiscard]] std::string toString() const override
    {
        if (minimum_ == -std::numeric_limits<float>::max() && maximum_ == std::numeric_limits<float>::max()) {
            return "float()";
        }
        if (maximum_ == std::numeric_limits<float>::max()) {
            return "float(" + toJavaString(minimum_) + ")";
        }
        return "float(" + toJavaString(minimum_) + ", " + toJavaString(maximum_) + ")";
    }

    [[nodiscard]] std::vector<std::string> getExamples() const override
    {
        return {"0", "1.2", ".5", "-1", "-.5", "-1234.56"};
    }

private:
    float minimum_;
    float maximum_;
};

/**
 * @see com.mojang.brigadier.arguments.DoubleArgumentType
 */
class DoubleArgumentType : public ArgumentType {
    struct Passkey {
        explicit Passkey() = default;
    };

public:
    static std::shared_ptr<DoubleArgumentType> doubleArg(double min = -std::numeric_limits<double>::max(),
                                                         double max = std::numeric_limits<double>::max())
    {
        return std::make_shared<DoubleArgumentType>(Passkey{}, min, max);
    }

    template <typename S>
    static double getDouble(const CommandContext<S> &context, const std::string &name)
    {
        return context.template getArgument<double>(name);
    }

    DoubleArgumentType(Passkey, double minimum, double maximum) : minimum_(minimum), maximum_(maximum) {}

    [[nodiscard]] double getMinimum() const { return minimum_; }
    [[nodiscard]] double getMaximum() const { return maximum_; }

    [[nodiscard]] bool isInstanceOf(const std::type_info &target) const override
    {
        return std::string_view(target.name()) == typeid(DoubleArgumentType).name() ||
               ArgumentType::isInstanceOf(target);
    }

    [[nodiscard]] ArgumentValue parse(StringReader &reader) const override
    {
        const int start = reader.getCursor();
        const double result = reader.readDouble();
        if (result < minimum_) {
            reader.setCursor(start);
            throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.doubleTooLow().createWithContext(
                reader, toJavaString(result), toJavaString(minimum_));
        }
        if (result > maximum_) {
            reader.setCursor(start);
            throw CommandSyntaxException::BUILT_IN_EXCEPTIONS.doubleTooHigh().createWithContext(
                reader, toJavaString(result), toJavaString(maximum_));
        }
        return result;
    }

    bool operator==(const DoubleArgumentType &other) const
    {
        return minimum_ == other.minimum_ && maximum_ == other.maximum_;
    }

    [[nodiscard]] std::string toString() const override
    {
        if (minimum_ == -std::numeric_limits<double>::max() && maximum_ == std::numeric_limits<double>::max()) {
            return "double()";
        }
        if (maximum_ == std::numeric_limits<double>::max()) {
            return "double(" + toJavaString(minimum_) + ")";
        }
        return "double(" + toJavaString(minimum_) + ", " + toJavaString(maximum_) + ")";
    }

    [[nodiscard]] std::vector<std::string> getExamples() const override
    {
        return {"0", "1.2", ".5", "-1", "-.5", "-1234.56"};
    }

private:
    double minimum_;
    double maximum_;
};

/**
 * @see com.mojang.brigadier.arguments.StringArgumentType
 */
class StringArgumentType : public ArgumentType {
    struct Passkey {
        explicit Passkey() = default;
    };

public:
    enum class StringType : std::uint8_t {
        SingleWord,
        QuotablePhrase,
        GreedyPhrase,
    };

    static std::shared_ptr<StringArgumentType> word() { return create(StringType::SingleWord); }
    static std::shared_ptr<StringArgumentType> string() { return create(StringType::QuotablePhrase); }
    static std::shared_ptr<StringArgumentType> greedyString() { return create(StringType::GreedyPhrase); }

    template <typename S>
    static std::string getString(const CommandContext<S> &context, const std::string &name)
    {
        return context.template getArgument<std::string>(name);
    }

    StringArgumentType(Passkey, StringType type) : type_(type) {}

    [[nodiscard]] StringType getType() const { return type_; }

    [[nodiscard]] bool isInstanceOf(const std::type_info &target) const override
    {
        return std::string_view(target.name()) == typeid(StringArgumentType).name() ||
               ArgumentType::isInstanceOf(target);
    }

    [[nodiscard]] ArgumentValue parse(StringReader &reader) const override
    {
        if (type_ == StringType::GreedyPhrase) {
            std::string text = reader.getRemaining();
            reader.setCursor(reader.getTotalLength());
            return text;
        }
        if (type_ == StringType::SingleWord) {
            return reader.readUnquotedString();
        }
        return reader.readString();
    }

    [[nodiscard]] std::string toString() const override { return "string()"; }

    [[nodiscard]] std::vector<std::string> getExamples() const override
    {
        switch (type_) {
        case StringType::SingleWord:
            return {"word", "words_with_underscores"};
        case StringType::QuotablePhrase:
            return {"\"quoted phrase\"", "word", "\"\""};
        case StringType::GreedyPhrase:
            return {"word", "words with spaces", "\"and symbols\""};
        }
        return {};
    }

    static std::string escapeIfRequired(const std::string &input)
    {
        for (const char c : input) {
            if (!StringReader::isAllowedInUnquotedString(c)) {
                return escape(input);
            }
        }
        return input;
    }

private:
    static std::shared_ptr<StringArgumentType> create(StringType type)
    {
        return std::make_shared<StringArgumentType>(Passkey{}, type);
    }

    static std::string escape(const std::string &input)
    {
        std::string result = "\"";
        for (const char c : input) {
            if (c == '\\' || c == '"') {
                result += '\\';
            }
            result += c;
        }
        result += '"';
        return result;
    }

    StringType type_;
};

}  // namespace endstone::brigadier
