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

#include "endstone/core/command/tree/argument_types.h"

#include <charconv>
#include <format>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "endstone/command/command_error.h"

namespace endstone::core {

namespace {

int parseInteger(const std::string &text, const std::string_view name)
{
    int value = 0;
    const auto *first = text.data();
    const auto *last = text.data() + text.size();
    const auto [ptr, ec] = std::from_chars(first, last, value);
    if (ec != std::errc{} || ptr != last) {
        throw CommandError(std::format("'{}' is not a whole number for '{}'.", text, name));
    }
    return value;
}

float parseFloat(const std::string &text, const std::string_view name)
{
    try {
        std::size_t consumed = 0;
        const float value = std::stof(text, &consumed);
        if (consumed != text.size()) {
            throw std::invalid_argument("trailing characters");
        }
        return value;
    }
    catch (const std::exception &) {
        throw CommandError(std::format("'{}' is not a number for '{}'.", text, name));
    }
}

}  // namespace

ArgumentValue convertArgument(const ArgumentType &type, const std::string &text, const NotNull<CommandSender> &sender)
{
    switch (type.getKind()) {
    case ArgumentKind::Boolean:
        return ArgumentValue::of(text == "true" || text == "1");
    case ArgumentKind::Integer: {
        const auto &builtin = static_cast<const BuiltinArgumentType &>(type);
        const auto value = parseInteger(text, "integer");
        if (value < builtin.getMinimum() || value > builtin.getMaximum()) {
            throw CommandError(std::format("{} is out of range: expected {} to {}.", value,
                                           static_cast<long long>(builtin.getMinimum()),
                                           static_cast<long long>(builtin.getMaximum())));
        }
        return ArgumentValue::of(value);
    }
    case ArgumentKind::Float: {
        const auto &builtin = static_cast<const BuiltinArgumentType &>(type);
        const auto value = parseFloat(text, "number");
        if (value < builtin.getMinimum() || value > builtin.getMaximum()) {
            throw CommandError(std::format("{} is out of range: expected {} to {}.", value, builtin.getMinimum(),
                                           builtin.getMaximum()));
        }
        return ArgumentValue::of(value);
    }
    case ArgumentKind::Json: {
        try {
            return ArgumentValue::of(nlohmann::json::parse(text));
        }
        catch (const nlohmann::json::exception &e) {
            throw CommandError(std::format("'{}' is not valid JSON: {}", text, e.what()));
        }
    }
    case ArgumentKind::Custom: {
        const auto native = type.getNativeType();
        if (!native) {
            throw CommandError("This argument has no native type to parse as.");
        }
        return type.convert(convertArgument(*native, text, sender), sender);
    }
    default:
        return ArgumentValue::of(text);
    }
}

}  // namespace endstone::core
