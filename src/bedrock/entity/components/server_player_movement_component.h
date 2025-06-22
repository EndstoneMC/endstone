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
#include <bitset>
#include <deque>
#include <memory>
#include <optional>

struct MovementPackets {};

struct IPlayerTickPolicy {
    enum class TickAction : int8_t {
        ProcessTick = 0,
        StopProcessing = 1,
        SkipTick = 2,
    };
    virtual ~IPlayerTickPolicy() = default;
    [[nodiscard]] virtual uint64_t getMaxTickCount(uint64_t, bool) const = 0;
    [[nodiscard]] virtual TickAction shouldTickPlayer(uint64_t, size_t) const = 0;
    [[nodiscard]] virtual bool isStrictMovement() const = 0;
    [[nodiscard]] virtual bool isStrictDismount() const = 0;
};
static_assert(sizeof(IPlayerTickPolicy) == 8);

struct ServerPlayerMovementComponent {
    [[nodiscard]] const std::unique_ptr<IPlayerTickPolicy> &getPolicy() const
    {
        return policy_;
    }

    std::bitset<3UL> server_has_movement_authority;
    bool server_has_inventory_authority;
    std::optional<float> accept_client_pos_if_within_distance_sq;
    uint64_t player_tick_credits;

private:
    std::unique_ptr<IPlayerTickPolicy> policy_;
    std::deque<MovementPackets> queued_updates_;
};
