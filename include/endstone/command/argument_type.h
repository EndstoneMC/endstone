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
#include <limits>
#include <memory>
#include <string>
#include <string_view>
#include <typeinfo>
#include <utility>
#include <vector>

#include "endstone/util/pointers.h"

namespace endstone {

class CommandSender;

/**
 * A parsed value for a single command argument.
 *
 * The value is type-erased so that Endstone can hand any argument type back to a plugin. Retrieval
 * is checked against the mangled type name rather than against typeinfo identity, because typeinfo
 * addresses are not shared between the server and a plugin module.
 */
class ArgumentValue {
public:
    ArgumentValue() = default;

    /**
     * Wraps a value of the given type.
     *
     * @tparam T Type of the value
     * @param value Value to wrap
     * @return An ArgumentValue holding a copy of the value
     */
    template <typename T>
    static ArgumentValue of(T value)
    {
        ArgumentValue result;
        result.type_name_ = typeid(T).name();
        result.storage_ = std::make_shared<T>(std::move(value));
        return result;
    }

    /**
     * Returns a pointer to the held value if it is of the given type.
     *
     * @tparam T Type to retrieve
     * @return Pointer to the value, or nullptr if this holds nothing or a different type
     */
    template <typename T>
    [[nodiscard]] const T *get() const
    {
        if (!storage_ || type_name_ != typeid(T).name()) {
            return nullptr;
        }
        return static_cast<const T *>(storage_.get());
    }

    /**
     * Returns whether this holds a value.
     *
     * @return true if a value is held, otherwise false
     */
    [[nodiscard]] bool hasValue() const { return storage_ != nullptr; }

    /**
     * Returns the mangled name of the held type, for diagnostics.
     *
     * @return Mangled type name, or an empty view if this holds nothing
     */
    [[nodiscard]] std::string_view getTypeName() const { return type_name_; }

private:
    std::string type_name_;
    std::shared_ptr<const void> storage_;
};

/**
 * The built-in argument types Endstone knows how to register with the client.
 *
 * An argument type that is not one of these reports Custom and supplies a native type to be
 * presented to the client in its place.
 */
enum class ArgumentKind : std::uint8_t {
    Custom = 0,
    Boolean,
    Integer,
    Float,
    String,
    Message,
    RawText,
    Player,
    Players,
    Entity,
    Entities,
    BlockPosition,
    Position,
    BlockType,
    BlockStates,
    EntityType,
    Json,
    IntegerRange,
    Enumeration,
    SoftEnum,
};

/**
 * Describes how a command argument is parsed and what the client is told about it.
 *
 * Use the Arguments factories for the built-in types. Subclass this to add an argument type of
 * your own, reporting Custom from getKind() and returning the built-in type it is presented as
 * from getNativeType().
 */
class ArgumentType {
public:
    virtual ~ArgumentType() = default;

    /**
     * Returns which built-in type this is, or Custom for a plugin-provided type.
     *
     * @return Kind of this argument type
     */
    [[nodiscard]] virtual ArgumentKind getKind() const { return ArgumentKind::Custom; }

    /**
     * Returns the built-in type this argument is presented to the client as.
     *
     * The client only validates and completes types it knows about, so a custom argument borrows
     * a built-in type's grammar and converts the value server-side.
     *
     * @return The type to register with the client
     */
    [[nodiscard]] virtual Nullable<ArgumentType> getNativeType() const { return nullptr; }

    /**
     * Converts a value parsed as the native type into this argument's value.
     *
     * Throw a CommandError to reject the input.
     *
     * @param native Value parsed as getNativeType()
     * @param sender Source of the command
     * @return The converted value
     */
    [[nodiscard]] virtual ArgumentValue convert(const ArgumentValue &native, const NotNull<CommandSender> &sender) const
    {
        return native;
    }
};

/**
 * An argument type provided by Endstone.
 *
 * Created through the Arguments factories rather than directly.
 */
class BuiltinArgumentType final : public ArgumentType {
public:
    BuiltinArgumentType(ArgumentKind kind, std::string name = {}, std::vector<std::string> values = {})
        : kind_(kind), name_(std::move(name)), values_(std::move(values))
    {
    }

    [[nodiscard]] ArgumentKind getKind() const override { return kind_; }

    /**
     * Returns the enum or soft enum name, for Enumeration and SoftEnum arguments.
     *
     * @return Name of the value set, or an empty string for other kinds
     */
    [[nodiscard]] const std::string &getName() const { return name_; }

    /**
     * Returns the allowed values, for Enumeration arguments.
     *
     * @return Allowed values, or an empty list for other kinds
     */
    [[nodiscard]] const std::vector<std::string> &getValues() const { return values_; }

    /**
     * Returns the inclusive lower bound, for Integer and Float arguments.
     *
     * @return Minimum accepted value
     */
    [[nodiscard]] double getMinimum() const { return minimum_; }

    /**
     * Returns the inclusive upper bound, for Integer and Float arguments.
     *
     * @return Maximum accepted value
     */
    [[nodiscard]] double getMaximum() const { return maximum_; }

    BuiltinArgumentType &setRange(double minimum, double maximum)
    {
        minimum_ = minimum;
        maximum_ = maximum;
        return *this;
    }

private:
    ArgumentKind kind_;
    std::string name_;
    std::vector<std::string> values_;
    double minimum_ = -std::numeric_limits<double>::infinity();
    double maximum_ = std::numeric_limits<double>::infinity();
};

}  // namespace endstone
