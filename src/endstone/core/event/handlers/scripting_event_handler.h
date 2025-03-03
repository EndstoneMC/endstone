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

#include "bedrock/gameplayhandlers/scripting_event_handler.h"

namespace endstone::core {

class EndstoneScriptingEventHandler final : public ScriptingEventHandler {
public:
    explicit EndstoneScriptingEventHandler(std::unique_ptr<ScriptingEventHandler> handle);
    GameplayHandlerResult<CoordinatorResult> handleEvent(
        MutableScriptingGameplayEvent<CoordinatorResult> &event) override;
    GameplayHandlerResult<CoordinatorResult> handleEvent(
        const ScriptingGameplayEvent<CoordinatorResult> &event) override;
    std::unique_ptr<ScriptingEventHandler> unwrap();

private:
    bool handleEvent(const ScriptCommandMessageEvent &event);

    std::unique_ptr<ScriptingEventHandler> handle_;
};

}  // namespace endstone::core
