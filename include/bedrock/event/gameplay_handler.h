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
#include "bedrock/event/coordinator_result.h"
#include "bedrock/event/event_ref.h"
#include "bedrock/event/event_result.h"
#include "bedrock/event/level_event.h"
#include "bedrock/event/player_event.h"

enum class HandlerResult : int {
    Deny = 0,
    Default = 1
};

template <typename Type>
struct GameplayHandlerResult {
    Type value;
    EventResult result;
};

class LevelGameplayHandler {};
class ScriptLevelGameplayHandler : public LevelGameplayHandler {};
