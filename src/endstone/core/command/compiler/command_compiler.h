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

#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include <nonstd/expected.hpp>

#include "endstone/brigadier/context.h"
#include "endstone/brigadier/tree.h"
#include "endstone/command/brigadier/command_source_stack.h"
#include "endstone/core/command/command_usage_parser.h"

namespace endstone::core {

struct CommandParameter {
    enum class Kind : std::uint8_t {
        Keyword,
        Enum,
        SoftEnum,
        Int,
        Float,
        String,
        Actor,
        Player,
        BlockPos,
        Pos,
        Message,
        RawText,
        Json,
        BlockStates,
        Dimension,
        BlockType,
        ItemType,
        ActorType,
    };

    Kind kind = Kind::String;
    std::string name;
    std::string enum_name;
    std::vector<std::string> values;
    bool optional = false;
    bool foreign = false;
    bool single = false;
    bool capture = false;

    [[nodiscard]] bool isEnum() const
    {
        return kind == Kind::Keyword || kind == Kind::Enum || kind == Kind::Dimension || kind == Kind::BlockType ||
               kind == Kind::ItemType || kind == Kind::ActorType;
    }
    [[nodiscard]] bool isWord() const { return kind == Kind::Keyword || (kind == Kind::Enum && !foreign); }
    [[nodiscard]] bool isIdClass() const
    {
        return kind == Kind::String || kind == Kind::Actor || kind == Kind::Player || kind == Kind::SoftEnum ||
               kind == Kind::RawText || kind == Kind::Message;
    }
    [[nodiscard]] bool isParsedAsText() const
    {
        return isEnum() || kind == Kind::Message || kind == Kind::Int || kind == Kind::Float ||
               (capture && kind != Kind::Actor && kind != Kind::Player);
    }
    [[nodiscard]] bool sameSlot(const CommandParameter &other) const
    {
        return kind == other.kind && enum_name == other.enum_name;
    }

    bool operator==(const CommandParameter &) const = default;
};

using CommandOverload = std::vector<CommandParameter>;

struct CompiledEnum {
    std::string name;
    std::vector<std::string> values;

    bool operator==(const CompiledEnum &) const = default;
};

struct CommandSlot {
    std::string string_value;
    std::optional<brigadier::ArgumentValue> value;
    bool is_set = false;
    bool quoted = false;
};

struct RegisteredOverload {
    CommandOverload parameters;
    std::vector<CommandOverload> sources;

    bool operator==(const RegisteredOverload &) const = default;
};

struct CompiledCommand {
    std::vector<CommandOverload> overloads;
    std::vector<CompiledEnum> enums;
    std::vector<RegisteredOverload> registered;
};

class CommandCompiler {
public:
    explicit CommandCompiler(std::string label);

    [[nodiscard]] std::optional<std::string> addUsage(const CommandUsageParser::Result &usage,
                                                      const std::set<std::string> &soft_enums);
    [[nodiscard]] std::optional<std::string> addOverloads(std::vector<CommandOverload> overloads, bool tree = false);
    [[nodiscard]] std::optional<std::string> addTree(const brigadier::LiteralCommandNode<CommandSource> &root);
    [[nodiscard]] const CompiledCommand &getCompiled() const;

    [[nodiscard]] static std::string nameEnum(const std::string &name, const std::vector<std::string> &values,
                                              const std::map<std::string, std::vector<std::string>> &taken);
    [[nodiscard]] static std::optional<brigadier::CommandContext<CommandSource>> bindArguments(
        const std::shared_ptr<brigadier::LiteralCommandNode<CommandSource>> &root, const CommandOverload &overload,
        const std::vector<CommandSlot> &slots, const CommandSource &source, const std::string &input);
    [[nodiscard]] static nonstd::expected<std::size_t, std::size_t> chooseSource(const RegisteredOverload &registered,
                                                                                 const std::vector<CommandSlot> &slots);
    [[nodiscard]] static std::optional<std::string> captureWords(
        std::vector<RegisteredOverload> &registered, const std::vector<CommandOverload> &advertised,
        const std::function<bool(const CommandParameter &, const CommandParameter &)> &reads, const std::string &root,
        bool tree);
    [[nodiscard]] static std::optional<std::size_t> findUsable(const brigadier::LiteralCommandNode<CommandSource> &root,
                                                               const std::vector<std::string> &names,
                                                               const CommandSource &source);

private:
    [[nodiscard]] std::vector<CommandOverload> normalize(std::vector<CommandOverload> overloads) const;
    [[nodiscard]] std::optional<std::string> findClash(const std::vector<const CommandOverload *> &overloads,
                                                       std::size_t position, const std::string &path, bool tree) const;
    [[nodiscard]] static std::optional<std::string> clashBetween(const CommandParameter &a, const CommandParameter &b,
                                                                 const std::string &path);
    [[nodiscard]] static std::string describe(const CommandParameter &parameter);

    std::string label_;
    CompiledCommand compiled_;
};

}  // namespace endstone::core
