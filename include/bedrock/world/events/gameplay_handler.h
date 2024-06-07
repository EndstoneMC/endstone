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

#include "bedrock/bedrock.h"
#include "bedrock/world/events/coordinator_result.h"
#include "bedrock/world/events/event_ref.h"
#include "bedrock/world/events/event_result.h"
#include "bedrock/world/events/level_event.h"
#include "bedrock/world/events/player_event.h"
#include "bedrock/world/events/server_event.h"

template <typename Type>
struct GameplayHandlerResult {
    Type value;
    EventResult result;
};

class LevelGameplayHandler {
    virtual ~LevelGameplayHandler() = 0;
};
class ScriptLevelGameplayHandler : public LevelGameplayHandler {
public:
    ~ScriptLevelGameplayHandler() override = 0;
    virtual ENDSTONE_HOOK GameplayHandlerResult<CoordinatorResult> handleEvent(LevelWeatherChangedEvent &event);
};
