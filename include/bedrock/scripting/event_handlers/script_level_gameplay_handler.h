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
#include "bedrock/forward.h"
#include "bedrock/gameplayhandlers/coordinator_result.h"
#include "bedrock/gameplayhandlers/event_handler_dispatcher.h"
#include "bedrock/gameplayhandlers/gameplay_handler_result.h"
#include "bedrock/gameplayhandlers/level_gameplay_handler.h"
#include "bedrock/scripting/event_handlers/script_event_handler.h"
#include "bedrock/world/events/level_events.h"

class ScriptLevelGameplayHandler : public EventHandlerDispatcher<LevelGameplayHandler>,
                                   public ScriptEventHandler<ScriptModuleMinecraft::IScriptWorldBeforeEvents> {
public:
    virtual ~ScriptLevelGameplayHandler() = 0;
    virtual ENDSTONE_HOOK GameplayHandlerResult<CoordinatorResult> handleEvent(LevelWeatherChangedEvent &event);
};
