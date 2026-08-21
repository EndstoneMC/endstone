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

#include <optional>
#include <utility>

#include "endstone/event/player/player_set_spawn_event.h"

class Player;

namespace endstone::core {

struct PlayerSpawnContext {
    const ::Player *player = nullptr;
    PlayerSetSpawnEvent::Cause cause = PlayerSetSpawnEvent::Cause::Unknown;
    bool suppress_event = false;
};

inline thread_local std::optional<PlayerSpawnContext> player_spawn_context;

class PlayerSpawnContextScope {
public:
    explicit PlayerSpawnContextScope(PlayerSpawnContext context) : previous_(std::move(player_spawn_context))
    {
        player_spawn_context = std::move(context);
    }

    ~PlayerSpawnContextScope() { player_spawn_context = std::move(previous_); }

    PlayerSpawnContextScope(const PlayerSpawnContextScope &) = delete;
    PlayerSpawnContextScope &operator=(const PlayerSpawnContextScope &) = delete;

private:
    std::optional<PlayerSpawnContext> previous_;
};

class PlayerSpawnContextSuspension {
public:
    PlayerSpawnContextSuspension() : previous_(std::move(player_spawn_context)) { player_spawn_context.reset(); }

    ~PlayerSpawnContextSuspension() { player_spawn_context = std::move(previous_); }

    PlayerSpawnContextSuspension(const PlayerSpawnContextSuspension &) = delete;
    PlayerSpawnContextSuspension &operator=(const PlayerSpawnContextSuspension &) = delete;

private:
    std::optional<PlayerSpawnContext> previous_;
};

inline std::optional<PlayerSpawnContext> consumePlayerSpawnContext(const ::Player &player)
{
    if (!player_spawn_context || (player_spawn_context->player && player_spawn_context->player != &player)) {
        return std::nullopt;
    }

    if (!player_spawn_context->player) {
        return player_spawn_context;
    }

    auto context = std::move(player_spawn_context);
    player_spawn_context.reset();
    return context;
}

}  // namespace endstone::core
