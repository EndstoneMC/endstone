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

#include "endstone/core/command/compiler/command_compiler.h"

#include <algorithm>
#include <cctype>
#include <exception>
#include <format>
#include <functional>
#include <iterator>
#include <memory>
#include <set>
#include <string_view>
#include <typeinfo>
#include <unordered_map>
#include <utility>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "endstone/command/brigadier/argument/argument_types.h"

namespace endstone::core {

CommandCompiler::CommandCompiler(std::string label) : label_(std::move(label)) {}

std::optional<std::string> CommandCompiler::addUsage(const CommandUsageParser::Result &usage,
                                                     const std::set<std::string> &soft_enums)
{
    using Kind = CommandParameter::Kind;
    static const std::unordered_map<std::string, CommandParameter> types = {
        {"int", {.kind = Kind::Int}},
        {"float", {.kind = Kind::Float}},
        {"actor", {.kind = Kind::Actor}},
        {"entity", {.kind = Kind::Actor}},
        {"target", {.kind = Kind::Actor}},
        {"player", {.kind = Kind::Player}},
        {"string", {.kind = Kind::String}},
        {"str", {.kind = Kind::String}},
        {"block_pos", {.kind = Kind::BlockPos}},
        {"vec3i", {.kind = Kind::BlockPos}},
        {"pos", {.kind = Kind::Pos}},
        {"vec3", {.kind = Kind::Pos}},
        {"vec3f", {.kind = Kind::Pos}},
        {"message", {.kind = Kind::Message}},
        {"json", {.kind = Kind::Json}},
        {"block_states", {.kind = Kind::BlockStates}},
        {"bool", {.kind = Kind::Enum, .enum_name = "Boolean", .values = {"true", "false"}, .foreign = true}},
        {"block", {.kind = Kind::Enum, .enum_name = "Block", .foreign = true}},
        {"entity_type", {.kind = Kind::Enum, .enum_name = "EntityType", .foreign = true}},
    };

    CommandOverload overload;
    for (const auto &parameter : usage.parameters) {
        CommandParameter result;
        if (parameter.is_enum) {
            result.kind = parameter.values.size() == 1 ? Kind::Keyword : Kind::Enum;
            result.enum_name = parameter.type;
            result.values = parameter.values;
        }
        else if (soft_enums.contains(parameter.type)) {
            result.kind = Kind::SoftEnum;
            result.enum_name = parameter.type;
        }
        else if (const auto it = types.find(parameter.type); it != types.end()) {
            result = it->second;
        }
        else {
            return std::format("Unsupported type '{}'.", parameter.type);
        }
        result.name = parameter.name;
        result.optional = parameter.optional;
        overload.push_back(std::move(result));
    }

    std::vector<std::size_t> expanded;
    for (std::size_t i = 0; i < overload.size(); ++i) {
        if (overload[i].optional &&
            std::any_of(overload.begin() + static_cast<std::ptrdiff_t>(i) + 1, overload.end(),
                        [](const CommandParameter &next) { return !next.optional && next.kind != Kind::Message; })) {
            expanded.push_back(i);
        }
    }
    if (expanded.size() > 6) {
        return std::format("It has {} optional parameters before a required one, more than the 6 a usage can expand.",
                           expanded.size());
    }
    std::vector<CommandOverload> forms;
    for (std::size_t mask = 0; mask < (std::size_t{1} << expanded.size()); ++mask) {
        auto &form = forms.emplace_back();
        for (std::size_t i = 0; i < overload.size(); ++i) {
            const auto bit = static_cast<std::size_t>(std::ranges::find(expanded, i) - expanded.begin());
            if (bit == expanded.size()) {
                form.push_back(overload[i]);
            }
            else if ((mask >> bit & 1) != 0) {
                form.push_back(overload[i]);
                form.back().optional = false;
            }
        }
    }
    return addOverloads(std::move(forms));
}

std::optional<std::string> CommandCompiler::addOverloads(std::vector<CommandOverload> overloads, bool tree)
{
    using Kind = CommandParameter::Kind;
    const std::string root = "/" + label_;
    std::map<std::string, std::vector<std::string>> enums;
    for (const auto &overload : compiled_.overloads) {
        for (const auto &parameter : overload) {
            if (parameter.isEnum() && !parameter.foreign) {
                enums.emplace(parameter.enum_name, parameter.values);
            }
        }
    }
    for (auto &overload : overloads) {
        std::string path = root;
        for (std::size_t i = 0; i < overload.size(); ++i) {
            auto &parameter = overload[i];
            if (parameter.isEnum()) {
                std::set<std::string> seen;
                for (auto &value : parameter.values) {
                    std::ranges::transform(value, value.begin(), [](unsigned char c) { return std::tolower(c); });
                    if (!seen.insert(value).second) {
                        return std::format("'{}' has the value '{}' twice in '{}' once lowercased.", path, value,
                                           parameter.enum_name);
                    }
                }
                if (!parameter.foreign) {
                    const auto [it, inserted] = enums.emplace(parameter.enum_name, parameter.values);
                    if (!inserted && it->second != parameter.values) {
                        return std::format("'{}' declares the enum '{}' with the values '{}' and also '{}'.", root,
                                           parameter.enum_name, boost::algorithm::join(it->second, "|"),
                                           boost::algorithm::join(parameter.values, "|"));
                    }
                }
            }
            if ((parameter.kind == Kind::Message || parameter.kind == Kind::RawText) && i + 1 < overload.size()) {
                return std::format("'{} {}' is {} and is not last.", path, describe(parameter),
                                   parameter.kind == Kind::Message ? "a message" : "raw text");
            }
            path += " " + describe(parameter);
        }
    }

    auto all = compiled_.overloads;
    all.insert(all.end(), std::make_move_iterator(overloads.begin()), std::make_move_iterator(overloads.end()));
    auto normalized = normalize(std::move(all));

    std::vector<RegisteredOverload> registered;
    for (const auto &overload : normalized) {
        registered.push_back({overload, {overload}});
    }
    if (auto error = captureWords(
            registered, normalized,
            [](const CommandParameter & /*word*/, const CommandParameter &argument) { return argument.isIdClass(); },
            root, tree)) {
        return error;
    }

    std::vector<const CommandOverload *> pointers;
    for (const auto &entry : registered) {
        pointers.push_back(&entry.parameters);
    }
    if (auto clash = findClash(pointers, 0, root, tree)) {
        return clash;
    }
    if (registered.size() > 250) {
        return std::format("'{}' would have {} overloads, more than the 250 a command can have.", root,
                           registered.size());
    }

    compiled_.overloads = std::move(normalized);
    compiled_.registered = std::move(registered);
    compiled_.enums.clear();
    for (const auto &overload : compiled_.overloads) {
        for (const auto &parameter : overload) {
            if (parameter.isEnum() && !parameter.foreign &&
                std::ranges::none_of(compiled_.enums,
                                     [&](const CompiledEnum &e) { return e.name == parameter.enum_name; })) {
                compiled_.enums.push_back({parameter.enum_name, parameter.values});
            }
        }
    }
    return std::nullopt;
}

std::optional<std::string> CommandCompiler::captureWords(
    std::vector<RegisteredOverload> &registered, const std::vector<CommandOverload> &advertised,
    const std::function<bool(const CommandParameter &, const CommandParameter &)> &reads, const std::string &root,
    bool tree)
{
    using Kind = CommandParameter::Kind;
    std::size_t longest = 0;
    for (const auto &entry : registered) {
        longest = std::max(longest, entry.parameters.size());
    }
    for (std::size_t position = 0; position < longest; ++position) {
        for (std::size_t i = 0; i < registered.size(); ++i) {
            if (registered[i].parameters.size() <= position || !registered[i].parameters[position].isEnum()) {
                continue;
            }
            auto capture = registered[i];
            const auto argument = std::ranges::find_if(registered, [&](const RegisteredOverload &other) {
                return other.parameters.size() > position &&
                       reads(capture.parameters[position], other.parameters[position]) &&
                       std::equal(other.parameters.begin(),
                                  other.parameters.begin() + static_cast<std::ptrdiff_t>(position),
                                  capture.parameters.begin(), std::mem_fn(&CommandParameter::sameSlot));
            });
            if (argument == registered.end()) {
                continue;
            }
            auto &slot = capture.parameters[position];
            const auto word = slot;
            slot = argument->parameters[position];
            slot.optional = word.optional;
            slot.capture = true;
            if ((slot.kind == Kind::Message || slot.kind == Kind::RawText) &&
                position + 1 < capture.parameters.size()) {
                if (!tree) {
                    continue;
                }
                std::string path = root;
                for (std::size_t k = 0; k < position; ++k) {
                    path += " " + describe(capture.parameters[k]);
                }
                return std::format("'{}' has {} beside the greedy {}, which reads the rest of the line.", path,
                                   describe(word), describe(slot));
            }
            const auto same = std::ranges::find_if(registered, [&](const RegisteredOverload &other) {
                return std::ranges::equal(other.parameters, capture.parameters,
                                          [](const CommandParameter &a, const CommandParameter &b) {
                                              return a.sameSlot(b) && a.optional == b.optional;
                                          });
            });
            if (same == registered.end()) {
                registered.push_back(std::move(capture));
                continue;
            }
            same->parameters[position].capture = true;
            for (auto &source : capture.sources) {
                if (std::ranges::find(same->sources, source) == same->sources.end()) {
                    same->sources.push_back(std::move(source));
                }
            }
        }
    }
    for (auto &entry : registered) {
        for (const auto &source : advertised) {
            if (std::ranges::find(entry.sources, source) != entry.sources.end()) {
                continue;
            }
            bool fits = true;
            std::vector<std::size_t> words;
            for (std::size_t k = 0; k < std::min(source.size(), entry.parameters.size()) && fits; ++k) {
                if (source[k].sameSlot(entry.parameters[k])) {
                    continue;
                }
                fits = source[k].isWord() && reads(source[k], entry.parameters[k]);
                words.push_back(k);
            }
            if (fits && !words.empty()) {
                for (const auto k : words) {
                    entry.parameters[k].capture = true;
                }
                entry.sources.push_back(source);
            }
        }
        std::ranges::sort(entry.sources, {}, [&](const CommandOverload &source) {
            return std::ranges::find(advertised, source) - advertised.begin();
        });
    }
    return std::nullopt;
}

std::optional<std::string> CommandCompiler::addTree(const brigadier::LiteralCommandNode<CommandSource> &root)
{
    using Kind = CommandParameter::Kind;
    using Node = brigadier::CommandNode<CommandSource>;
    struct Entry {
        const Node *node;
        CommandOverload parameters;
        std::vector<bool> collapsible;
        std::vector<const Node *> path;
        std::string text;
    };
    const auto camel = [](const std::string &text) {
        std::string result;
        bool upper = true;
        for (const unsigned char c : text) {
            if (!std::isalnum(c)) {
                upper = true;
                continue;
            }
            result += static_cast<char>(upper ? std::toupper(c) : c);
            upper = false;
        }
        return result;
    };

    const std::string root_text = "/" + label_;
    if (root.getRedirect() != nullptr || root.isFork() || root.getRedirectModifier()) {
        return std::format("'{}' redirects to another node, which a Bedrock command cannot do.", root_text);
    }
    const auto command_name = camel(label_);
    std::vector<CommandOverload> overloads;
    std::vector<Entry> stack{{&root, {}, {}, {&root}, root_text}};
    while (!stack.empty()) {
        auto entry = std::move(stack.back());
        stack.pop_back();
        const auto &node = *entry.node;
        if (node.getCommand()) {
            auto overload = entry.parameters;
            for (auto i = overload.size(); i > 0 && entry.collapsible[i - 1]; --i) {
                overload[i - 1].optional = true;
            }
            overloads.push_back(std::move(overload));
        }

        const auto &children = node.getChildren();
        std::set<std::string> names;
        for (const auto &child : children) {
            if (!names.insert(boost::algorithm::to_lower_copy(child->getName())).second) {
                return std::format("'{}' has two children named '{}' once lowercased.", entry.text,
                                   boost::algorithm::to_lower_copy(child->getName()));
            }
        }
        for (auto it = children.rbegin(); it != children.rend(); ++it) {
            const auto &child = **it;
            if (std::ranges::find(entry.path, &child) != entry.path.end()) {
                return std::format("'{} {}' is reachable from itself.", entry.text, child.getName());
            }
            if (child.getRedirect() != nullptr || child.isFork() || child.getRedirectModifier()) {
                return std::format("'{} {}' redirects to another node, which a Bedrock command cannot do.", entry.text,
                                   child.getName());
            }
            CommandParameter parameter;
            if (const auto *literal = child.as<brigadier::LiteralCommandNode<CommandSource>>()) {
                parameter = {.kind = Kind::Keyword,
                             .name = boost::algorithm::to_lower_copy(literal->getLiteral()),
                             .enum_name = command_name + camel(literal->getLiteral()),
                             .values = {literal->getLiteral()}};
            }
            else if (const auto *argument = child.as<brigadier::ArgumentCommandNode<CommandSource>>()) {
                const auto &type = *argument->getType();
                parameter.name = argument->getName();
                if (const auto *string = type.as<brigadier::StringArgumentType>()) {
                    parameter.kind = string->getType() == brigadier::StringArgumentType::StringType::GreedyPhrase
                                       ? Kind::Message
                                       : Kind::String;
                }
                else if (type.is<brigadier::IntegerArgumentType>()) {
                    parameter.kind = Kind::Int;
                }
                else if (type.is<brigadier::FloatArgumentType>() || type.is<brigadier::DoubleArgumentType>()) {
                    parameter.kind = Kind::Float;
                }
                else if (type.is<brigadier::BoolArgumentType>()) {
                    parameter = {.kind = Kind::Enum,
                                 .name = parameter.name,
                                 .enum_name = "Boolean",
                                 .values = {"true", "false"},
                                 .foreign = true};
                }
                else if (const auto *soft_enum = type.as<SoftEnumArgumentType>()) {
                    parameter.kind = Kind::SoftEnum;
                    parameter.enum_name = soft_enum->getName();
                }
                else if (const auto *player = type.as<PlayerArgumentType>()) {
                    parameter.kind = Kind::Player;
                    parameter.single = player->single_;
                }
                else if (const auto *actor = type.as<ActorArgumentType>()) {
                    parameter.kind = Kind::Actor;
                    parameter.single = actor->single_;
                }
                else if (type.is<BlockPositionArgumentType>()) {
                    parameter.kind = Kind::BlockPos;
                }
                else if (type.is<FinePositionArgumentType>()) {
                    parameter.kind = Kind::Pos;
                }
                else if (type.is<DimensionArgumentType>()) {
                    parameter = {
                        .kind = Kind::Dimension, .name = parameter.name, .enum_name = "Dimension", .foreign = true};
                }
                else if (const auto *resource = type.as<ResourceArgumentType>();
                         resource != nullptr && resource->registry_type_ == "BlockType") {
                    parameter = {
                        .kind = Kind::BlockType, .name = parameter.name, .enum_name = "Block", .foreign = true};
                }
                else if (resource != nullptr && resource->registry_type_ == "ItemType") {
                    parameter = {.kind = Kind::ItemType, .name = parameter.name, .enum_name = "Item", .foreign = true};
                }
                else if (resource != nullptr && resource->registry_type_ == "ActorType") {
                    parameter = {
                        .kind = Kind::ActorType, .name = parameter.name, .enum_name = "EntityType", .foreign = true};
                }
                else {
                    return std::format("'{} <{}: {}>' has a type with no Bedrock counterpart.", entry.text,
                                       argument->getName(), type.toString());
                }
            }
            if (parameter.kind == Kind::Message && !child.getChildren().empty()) {
                return std::format("'{} {}' is a greedy string and is not last.", entry.text, describe(parameter));
            }
            Entry next{&child, entry.parameters, entry.collapsible, entry.path, entry.text + " " + describe(parameter)};
            next.collapsible.push_back(parameter.kind != Kind::Keyword && parameter.kind != Kind::Message &&
                                       static_cast<bool>(node.getCommand()));
            next.parameters.push_back(std::move(parameter));
            next.path.push_back(&child);
            stack.push_back(std::move(next));
        }
    }
    if (overloads.empty()) {
        return std::format("'{}' has no node that executes.", root_text);
    }
    return addOverloads(std::move(overloads), true);
}

const CompiledCommand &CommandCompiler::getCompiled() const
{
    return compiled_;
}

std::string CommandCompiler::nameEnum(const std::string &name, const std::vector<std::string> &values,
                                      const std::map<std::string, std::vector<std::string>> &taken)
{
    for (int attempt = 1;; ++attempt) {
        auto candidate = attempt == 1 ? name : name + std::to_string(attempt);
        const auto it = taken.find(candidate);
        if (it == taken.end() || it->second == values) {
            return candidate;
        }
    }
}

std::optional<brigadier::CommandContext<CommandSource>> CommandCompiler::bindArguments(
    const std::shared_ptr<brigadier::LiteralCommandNode<CommandSource>> &root, const CommandOverload &overload,
    const std::vector<CommandSlot> &slots, const CommandSource &source, const std::string &input)
{
    using Kind = CommandParameter::Kind;
    using Node = brigadier::CommandNode<CommandSource>;
    if (!root->canUse(source)) {
        return std::nullopt;
    }
    std::shared_ptr<Node> node = root;
    std::vector<brigadier::ParsedCommandNode<CommandSource>> nodes{{root, brigadier::StringRange::at(0)}};
    std::unordered_map<std::string, brigadier::ParsedArgument> arguments;
    for (std::size_t i = 0; i < overload.size() && i < slots.size() && slots[i].is_set; ++i) {
        const auto &parameter = overload[i];
        const auto &slot = slots[i];
        if (parameter.kind == Kind::Message && slot.string_value.empty() && node->getCommand()) {
            break;
        }
        std::shared_ptr<Node> child;
        for (const auto &candidate : node->getChildren()) {
            if ((candidate->as<brigadier::LiteralCommandNode<CommandSource>>() != nullptr) ==
                    (parameter.kind == Kind::Keyword) &&
                boost::algorithm::iequals(candidate->getName(), parameter.name)) {
                child = candidate;
                break;
            }
        }
        if (!child || !child->canUse(source)) {
            return std::nullopt;
        }
        nodes.emplace_back(child, brigadier::StringRange::at(0));
        if (const auto *argument = child->as<brigadier::ArgumentCommandNode<CommandSource>>()) {
            brigadier::ArgumentValue value = slot.string_value;
            if (slot.value) {
                value = *slot.value;
            }
            else if (const auto &type = *argument->getType();
                     type.is<brigadier::BoolArgumentType>() || type.is<brigadier::IntegerArgumentType>() ||
                     type.is<brigadier::FloatArgumentType>() || type.is<brigadier::DoubleArgumentType>()) {
                brigadier::StringReader reader(slot.string_value);
                try {
                    value = type.parse(reader);
                }
                catch (const std::exception &e) {
                    if (std::string_view(typeid(e).name()) != typeid(brigadier::CommandSyntaxException).name()) {
                        throw;
                    }
                    const auto &syntax = static_cast<const brigadier::CommandSyntaxException &>(e);
                    throw brigadier::CommandSyntaxException(syntax.getType(), syntax.getRawMessage());
                }
            }
            arguments.insert_or_assign(child->getName(), brigadier::ParsedArgument(0, 0, std::move(value)));
        }
        node = child;
    }
    if (!node->getCommand()) {
        return std::nullopt;
    }
    return brigadier::CommandContext<CommandSource>(source, input, std::move(arguments), node->getCommand(), root,
                                                    std::move(nodes), brigadier::StringRange::at(0), nullptr, nullptr,
                                                    false);
}

nonstd::expected<std::size_t, std::size_t> CommandCompiler::chooseSource(const RegisteredOverload &registered,
                                                                         const std::vector<CommandSlot> &slots)
{
    using Kind = CommandParameter::Kind;
    std::vector<std::size_t> candidates;
    for (std::size_t i = 0; i < registered.sources.size(); ++i) {
        candidates.push_back(i);
    }
    std::size_t position = 0;
    for (; position < slots.size() && position < registered.parameters.size() && slots[position].is_set; ++position) {
        const auto &slot = slots[position];
        const auto kind = registered.parameters[position].kind;
        const std::string_view text = slot.string_value;
        const auto token = kind == Kind::Message || kind == Kind::RawText ? text.substr(0, text.find(' ')) : text;
        std::vector<std::size_t> literals;
        std::vector<std::size_t> arguments;
        for (const auto candidate : candidates) {
            if (registered.sources[candidate].size() <= position) {
                continue;
            }
            const auto &parameter = registered.sources[candidate][position];
            const auto matches = std::ranges::any_of(
                parameter.values, [&](const std::string &value) { return boost::algorithm::iequals(value, token); });
            if (parameter.isWord()) {
                if (matches && !slot.quoted) {
                    literals.push_back(candidate);
                }
            }
            else if (!parameter.isEnum() || parameter.values.empty() || matches) {
                arguments.push_back(candidate);
            }
        }
        if (!literals.empty() && token.size() != text.size()) {
            return nonstd::make_unexpected(position);
        }
        candidates = literals.empty() ? std::move(arguments) : std::move(literals);
        if (candidates.empty()) {
            return nonstd::make_unexpected(position);
        }
    }
    const auto complete = std::ranges::find_if(candidates, [&](const std::size_t candidate) {
        const auto &source = registered.sources[candidate];
        return std::all_of(
            source.begin() + static_cast<std::ptrdiff_t>(std::min(position, source.size())), source.end(),
            [](const CommandParameter &parameter) { return parameter.optional || parameter.kind == Kind::Message; });
    });
    if (complete == candidates.end()) {
        return nonstd::make_unexpected(position);
    }
    return *complete;
}

std::optional<std::size_t> CommandCompiler::findUsable(const brigadier::LiteralCommandNode<CommandSource> &root,
                                                       const std::vector<std::string> &names,
                                                       const CommandSource &source)
{
    if (!root.canUse(source)) {
        return std::nullopt;
    }
    const brigadier::CommandNode<CommandSource> *node = &root;
    for (std::size_t i = 0; i < names.size(); ++i) {
        const brigadier::CommandNode<CommandSource> *child = nullptr;
        for (const auto &candidate : node->getChildren()) {
            if (boost::algorithm::iequals(candidate->getName(), names[i])) {
                child = candidate.get();
                break;
            }
        }
        if (child == nullptr || !child->canUse(source)) {
            return i;
        }
        node = child;
    }
    return names.size();
}

std::vector<CommandOverload> CommandCompiler::normalize(std::vector<CommandOverload> overloads) const
{
    using Kind = CommandParameter::Kind;
    std::vector<CommandOverload> expanded;
    for (auto &overload : overloads) {
        if (overload.empty() || overload.back().kind != Kind::Message) {
            expanded.push_back(std::move(overload));
            continue;
        }
        const auto first = expanded.size();
        for (std::size_t i = 0; i + 1 < overload.size(); ++i) {
            if (overload[i].optional) {
                expanded.emplace_back(overload.begin(), overload.begin() + static_cast<std::ptrdiff_t>(i));
            }
        }
        expanded.push_back(std::move(overload));
        for (auto k = first; k < expanded.size(); ++k) {
            for (auto &parameter : expanded[k]) {
                parameter.optional = false;
            }
        }
    }

    std::vector<CommandOverload> messages;
    std::ranges::copy_if(expanded, std::back_inserter(messages), [](const CommandOverload &overload) {
        return !overload.empty() && overload.back().kind == Kind::Message;
    });
    for (const auto &message : messages) {
        std::vector<CommandOverload> result;
        for (auto &overload : expanded) {
            if (overload.size() >= message.size() || !std::equal(overload.begin(), overload.end(), message.begin(),
                                                                 std::mem_fn(&CommandParameter::sameSlot))) {
                result.push_back(std::move(overload));
                continue;
            }
            const auto first = result.size();
            for (std::size_t i = 0; i < overload.size(); ++i) {
                if (overload[i].optional && i + 1 != message.size()) {
                    result.emplace_back(overload.begin(), overload.begin() + static_cast<std::ptrdiff_t>(i));
                }
            }
            if (overload.size() + 1 != message.size()) {
                result.push_back(std::move(overload));
            }
            for (auto k = first; k < result.size(); ++k) {
                for (auto &parameter : result[k]) {
                    parameter.optional = false;
                }
            }
        }
        expanded = std::move(result);
    }

    std::vector<CommandOverload> kept;
    for (std::size_t i = 0; i < expanded.size(); ++i) {
        const auto &shorter = expanded[i];
        bool covered = false;
        for (std::size_t j = 0; j < expanded.size() && !covered; ++j) {
            const auto &longer = expanded[j];
            if (i == j || longer.size() < shorter.size() || (longer.size() == shorter.size() && j > i)) {
                continue;
            }
            covered = std::equal(shorter.begin(), shorter.end(), longer.begin(),
                                 [](const CommandParameter &a, const CommandParameter &b) {
                                     return a.sameSlot(b) && a.optional == b.optional;
                                 }) &&
                      std::all_of(longer.begin() + static_cast<std::ptrdiff_t>(shorter.size()), longer.end(),
                                  [](const CommandParameter &parameter) { return parameter.optional; });
        }
        if (!covered) {
            kept.push_back(shorter);
        }
    }
    return kept;
}

std::optional<std::string> CommandCompiler::findClash(const std::vector<const CommandOverload *> &overloads,
                                                      std::size_t position, const std::string &path, bool tree) const
{
    std::vector<const CommandParameter *> distinct;
    for (const auto *overload : overloads) {
        if (overload->size() > position &&
            std::ranges::none_of(distinct, [&](const auto *p) { return p->sameSlot((*overload)[position]); })) {
            distinct.push_back(&(*overload)[position]);
        }
    }
    for (std::size_t i = 0; i < distinct.size(); ++i) {
        for (std::size_t j = i + 1; j < distinct.size(); ++j) {
            const auto &a = *distinct[i];
            const auto &b = *distinct[j];
            if (a.isEnum() != b.isEnum() || (!tree && !a.isEnum())) {
                continue;
            }
            if (auto clash = clashBetween(a, b, path)) {
                return clash;
            }
        }
    }
    for (const auto *parameter : distinct) {
        std::vector<const CommandOverload *> next;
        for (const auto *overload : overloads) {
            if (overload->size() > position + 1 && (*overload)[position].sameSlot(*parameter)) {
                next.push_back(overload);
            }
        }
        if (!next.empty()) {
            if (auto clash = findClash(next, position + 1, path + " " + describe(*parameter), tree)) {
                return clash;
            }
        }
    }
    return std::nullopt;
}

std::optional<std::string> CommandCompiler::clashBetween(const CommandParameter &a, const CommandParameter &b,
                                                         const std::string &path)
{
    if (a.isEnum() && b.isEnum()) {
        for (const auto &value : a.values) {
            if (std::ranges::find(b.values, value) != b.values.end()) {
                return std::format("'{}' has {} and {} at one position, and both take the word '{}'.", path,
                                   describe(a), describe(b), value);
            }
        }
        return std::nullopt;
    }
    if (a.isIdClass() && b.isIdClass()) {
        return std::format("'{}' has two Id-class parameters at one position: {} and {}.", path, describe(a),
                           describe(b));
    }
    return std::nullopt;
}

std::string CommandCompiler::describe(const CommandParameter &parameter)
{
    using Kind = CommandParameter::Kind;
    switch (parameter.kind) {
    case Kind::Keyword:
        return parameter.values.front();
    case Kind::Enum:
    case Kind::SoftEnum:
    case Kind::Dimension:
    case Kind::BlockType:
    case Kind::ItemType:
    case Kind::ActorType:
        return std::format("<{}: {}>", parameter.name, parameter.enum_name);
    case Kind::Int:
        return std::format("<{}: int>", parameter.name);
    case Kind::Float:
        return std::format("<{}: float>", parameter.name);
    case Kind::String:
        return std::format("<{}: string>", parameter.name);
    case Kind::Actor:
        return std::format("<{}: actor>", parameter.name);
    case Kind::Player:
        return std::format("<{}: player>", parameter.name);
    case Kind::BlockPos:
        return std::format("<{}: block_pos>", parameter.name);
    case Kind::Pos:
        return std::format("<{}: pos>", parameter.name);
    case Kind::Message:
        return std::format("<{}: message>", parameter.name);
    case Kind::RawText:
        return std::format("<{}: raw_text>", parameter.name);
    case Kind::Json:
        return std::format("<{}: json>", parameter.name);
    case Kind::BlockStates:
        return std::format("<{}: block_states>", parameter.name);
    }
    return parameter.name;
}

}  // namespace endstone::core
