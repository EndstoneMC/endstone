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

#include "bedrock/world/level/block_pos.h"
#include "endstone/event/player/player_open_sign_event.h"

class Player;

namespace endstone::core {

using OpenSignCause = endstone::PlayerOpenSignEvent::Cause;

struct OpenSignContext {
    const ::Player *player;
    BlockPos position;
    OpenSignCause cause;
};

class OpenSignCauseScope {
public:
    OpenSignCauseScope(const ::Player &player, const BlockPos &position, OpenSignCause cause);
    ~OpenSignCauseScope();

    OpenSignCauseScope(const OpenSignCauseScope &) = delete;
    OpenSignCauseScope &operator=(const OpenSignCauseScope &) = delete;

private:
    std::optional<OpenSignContext> previous_scoped_context_;
};

void setPendingOpenSignCause(const ::Player &player, const BlockPos &position, OpenSignCause cause);
void clearPendingOpenSignCause(const ::Player &player);
std::optional<OpenSignCause> consumeOpenSignCause(const ::Player &player, const BlockPos &position);

}  // namespace endstone::core
