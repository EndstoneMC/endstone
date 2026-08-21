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

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "endstone/actor/actor.h"
#include "endstone/command/command_error.h"
#include "endstone/player.h"

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

std::vector<NotNull<Actor>> toActors(const CommandResultVector &results)
{
    std::vector<NotNull<Actor>> actors;
    if (!results) {
        return actors;
    }
    for (auto *actor : *results) {
        if (actor) {
            actors.emplace_back(actor->getEndstoneActor<Actor>());
        }
    }
    return actors;
}

std::vector<NotNull<Player>> toPlayers(const CommandResultVector &results)
{
    std::vector<NotNull<Player>> players;
    if (!results) {
        return players;
    }
    for (auto *actor : *results) {
        if (actor && actor->isPlayer()) {
            players.emplace_back(actor->getEndstoneActor<Player>());
        }
    }
    return players;
}

template <typename T>
T requireExactlyOne(std::vector<T> matched)
{
    if (matched.empty()) {
        throw CommandError("No targets matched selector");
    }
    if (matched.size() > 1) {
        throw CommandError("This argument accepts a single target, but the selector matched more than one");
    }
    return matched.front();
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

ArgumentValue convertSelector(const ArgumentType &type, const CommandResultVector &results,
                              const NotNull<CommandSender> &sender)
{
    if (type.getKind() == ArgumentKind::Custom) {
        const auto native = type.getNativeType();
        if (!native) {
            throw CommandError("This argument has no native type to parse as.");
        }
        return type.convert(convertSelector(*native, results, sender), sender);
    }

    switch (type.getKind()) {
    case ArgumentKind::Player:
        return ArgumentValue::of(requireExactlyOne(toPlayers(results)));
    case ArgumentKind::Players: {
        auto players = toPlayers(results);
        if (players.empty()) {
            throw CommandError("No targets matched selector");
        }
        return ArgumentValue::of(std::move(players));
    }
    case ArgumentKind::Entity:
        return ArgumentValue::of(requireExactlyOne(toActors(results)));
    case ArgumentKind::Entities: {
        auto actors = toActors(results);
        if (actors.empty()) {
            throw CommandError("No targets matched selector");
        }
        return ArgumentValue::of(std::move(actors));
    }
    default:
        throw CommandError("This argument is not a target selector.");
    }
}

}  // namespace endstone::core
