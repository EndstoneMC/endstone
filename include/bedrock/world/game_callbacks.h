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

class GameCallbacks {
public:
    virtual ~GameCallbacks() = 0;
    virtual void onLevelCorrupt() = 0;
    virtual void onCriticalScriptError(char const *, char const *) = 0;
    virtual void onGameModeChanged() = 0;
    virtual void onBeforeSimTick() = 0;
    virtual void onTick() = 0;
    virtual void onInternetUpdate() = 0;
    virtual void onGameSessionReset() = 0;
    virtual void onLevelExit() = 0;
    virtual void onRequestResourceReload() = 0;
    virtual void updateScreens() = 0;
};
BEDROCK_STATIC_ASSERT_SIZE(GameCallbacks, 8, 8);
