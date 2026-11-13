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
#include <optional>
#include <variant>

#include "bedrock/world/level/block_pos.h"

class Actor;
class Block;

struct EnvironmentChangeContext {
    EnvironmentChangeContext() = default;
    EnvironmentChangeContext(const BlockPos &pos) : block_pos_context(pos) {}
    std::optional<BlockPos> block_pos_context = std::nullopt;
};

struct ActorChangeContext {
    ActorChangeContext();
    ActorChangeContext(Actor *actor) : actor_context(actor) {}
    Actor *actor_context = nullptr;
};

enum class StatelessBlockChangeContext : std::uint8_t {
    Scripts = 0,
    ScriptsPermutation = 1,
    Commands = 2,
    Structure = 3,
    MultiBlock = 4,
    ExtraBlock = 5,
    DelayedMultiBlock = 6,
    SupportLost = 7,
    Count = 8,
};

class NeighborChangeHistory {
    const Block *previous_permutation_;
    const Block *block_permutation_;
};

using BlockChangeContextSource =
    std::variant<std::monostate, EnvironmentChangeContext, ActorChangeContext, StatelessBlockChangeContext>;

class BlockChangeContext {
public:
    BlockChangeContext() = default;
    BlockChangeContext(const BlockPos &pos) : context_source_(pos) {}
    BlockChangeContext(Actor *actor) : context_source_(actor) {}
    [[nodiscard]] Actor *getActorSource() const;
    [[nodiscard]] const std::optional<BlockPos> getBlockPos() const;
    [[nodiscard]] const bool hasValue() const;
    [[nodiscard]] const bool isStatelessChangeContext(StatelessBlockChangeContext type) const;
    [[nodiscard]] bool isTickSchedulingEnabled() const;

private:
    BlockChangeContextSource context_source_;
    int command_version_ = 0;
    bool tick_scheduling_enabled_ = true;
    std::optional<NeighborChangeHistory> neighbor_change_history_;
};
