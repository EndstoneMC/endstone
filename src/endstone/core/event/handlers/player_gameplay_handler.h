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

#include "bedrock/gameplayhandlers/player_gameplay_handler.h"

namespace endstone::core {

class EndstonePlayerGameplayHandler final : public PlayerGameplayHandler {
public:
    explicit EndstonePlayerGameplayHandler(std::unique_ptr<PlayerGameplayHandler> handle);
    HandlerResult handleEvent(const PlayerGameplayEvent<void> &event) override;
    GameplayHandlerResult<CoordinatorResult> handleEvent(const PlayerGameplayEvent<CoordinatorResult> &event) override;
    GameplayHandlerResult<CoordinatorResult> handleEvent(MutablePlayerGameplayEvent<CoordinatorResult> &event) override;

private:
    bool handleEvent(const PlayerFormResponseEvent &event);
    bool handleEvent(const PlayerFormCloseEvent &event);
    bool handleEvent(const PlayerEmoteEvent &event);
    bool handleEvent(const PlayerInteractWithBlockBeforeEvent &event);
    bool handleEvent(const PlayerInteractWithEntityBeforeEvent &event);
    bool handleEvent(::PlayerGameModeChangeEvent &event);

    std::unique_ptr<PlayerGameplayHandler> handle_;
};

}  // namespace endstone::core
