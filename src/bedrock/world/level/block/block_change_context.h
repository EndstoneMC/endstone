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

#include <variant>

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

struct ScriptOrCommandChangeContext {
    ScriptOrCommandChangeContext() = default;
};

using BlockChangeContextSource =
    std::variant<std::monostate, EnvironmentChangeContext, ActorChangeContext, ScriptOrCommandChangeContext>;

class BlockChangeContext {
public:
    BlockChangeContext();
    BlockChangeContext(const bool);
    BlockChangeContext(const BlockPos &pos) : context_source_(pos) {}
    BlockChangeContext(Actor *actor) : context_source_(actor) {}
    Actor *getActorSource();
    const std::optional<BlockPos> getBlockPos();
    const bool isScriptOrCommandContext();
    const bool hasValue();

private:
    BlockChangeContextSource context_source_;
};
