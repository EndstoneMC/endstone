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

#include "bedrock/gameplayhandlers/gameplay_handler.h"
#include "bedrock/gameplayhandlers/gameplay_handler_result.h"
#include "bedrock/world/events/block_events.h"

class BlockGameplayHandler : public GameplayHandler {
public:
    virtual HandlerResult handleEvent(const BlockGameplayEvent<void> &) = 0;
    virtual GameplayHandlerResult<CoordinatorResult> handleEvent(const BlockGameplayEvent<CoordinatorResult> &) = 0;
    virtual GameplayHandlerResult<std::optional<std::string>> handleEvent(
        const BlockGameplayEvent<std::optional<std::string>> &) = 0;
    virtual GameplayHandlerResult<CoordinatorResult> handleEvent(MutableBlockGameplayEvent<CoordinatorResult> &) = 0;
};
