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

#include "endstone/core/player_open_sign.h"

#include <utility>

namespace endstone::core {
namespace {

thread_local std::optional<OpenSignContext> scoped_open_sign_context;
thread_local std::optional<OpenSignContext> pending_open_sign_context;

bool matches(const OpenSignContext &context, const ::Player &player, const BlockPos &position)
{
    return context.player == &player && context.position == position;
}

}  // namespace

OpenSignCauseScope::OpenSignCauseScope(const ::Player &player, const BlockPos &position, OpenSignCause cause)
    : previous_scoped_context_(std::move(scoped_open_sign_context))
{
    scoped_open_sign_context = OpenSignContext{&player, position, cause};
}

OpenSignCauseScope::~OpenSignCauseScope()
{
    scoped_open_sign_context = std::move(previous_scoped_context_);
}

void setPendingOpenSignCause(const ::Player &player, const BlockPos &position, OpenSignCause cause)
{
    pending_open_sign_context = OpenSignContext{&player, position, cause};
}

void clearPendingOpenSignCause(const ::Player &player)
{
    if (pending_open_sign_context && pending_open_sign_context->player == &player) {
        pending_open_sign_context.reset();
    }
}

std::optional<OpenSignCause> consumeOpenSignCause(const ::Player &player, const BlockPos &position)
{
    if (scoped_open_sign_context && matches(*scoped_open_sign_context, player, position)) {
        const auto cause = scoped_open_sign_context->cause;
        scoped_open_sign_context.reset();
        if (pending_open_sign_context && matches(*pending_open_sign_context, player, position)) {
            pending_open_sign_context.reset();
        }
        return cause;
    }
    if (pending_open_sign_context && matches(*pending_open_sign_context, player, position)) {
        const auto cause = pending_open_sign_context->cause;
        pending_open_sign_context.reset();
        return cause;
    }
    return std::nullopt;
}

}  // namespace endstone::core
