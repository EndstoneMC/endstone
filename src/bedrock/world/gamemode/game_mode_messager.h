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

#include <memory>

#include <gsl/gsl>

struct IGameModeMessenger {
    virtual ~IGameModeMessenger();
    using BlockBreakCaptureToken = gsl::final_action<std::function<void()>>;
    using BlockBreakCallback = std::function<void(const ItemStack &, const ItemStack &, const BlockPos &)>;
    virtual BlockBreakCaptureToken createBlockBreakCaptureScope(BlockBreakCallback) = 0;
    virtual void sendDenyDestroyBlock(const BlockPos &, int) = 0;
    virtual void sendStartDestroyBlock(const BlockPos &, int) = 0;
    virtual bool sendTryDestroyBlock(const BlockPos &, int,
                                     const std::function<bool(std::optional<ItemStack> &)> &) = 0;
    virtual void sendDestroyBlock(const Block &, const BlockPos &, int) = 0;
    virtual void sendChangeContinueDestroyBlock(const BlockPos &, int) = 0;
    virtual void sendContinueDestroyAboutToDestroyBlock(const BlockPos &, int) = 0;
    virtual void sendStopDestroyBlock(const BlockPos &, float) = 0;
    virtual void sendStartItemUseOn(const BlockPos &, const BlockPos &, int) = 0;
    virtual void sendStopItemUseOn(const BlockPos &) = 0;
    virtual void tryRotateTowardsAimAssist() = 0;
};

namespace GameModeExt {
struct MessengerFactory {
    using FactoryFunction = gsl::not_null<std::unique_ptr<IGameModeMessenger> (*)(Player &)>;
    FactoryFunction create_client_auth;
    FactoryFunction create_server_auth;
};
}  // namespace GameModeExt
