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

#include "bedrock/server/server_instance.h"

#include <endstone/runtime/hook.h>

void ServerInstance::_resetServerScriptManager()
{
    // TODO(hook): maybe we could use ScriptModuleShutdownEvent?
    // This function is called when the server loop stops (safely executed from the server thread)
    // or during the destruction of the ServerInstance object.
    entt::locator<endstone::core::EndstoneServer>::reset();
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstance::_resetServerScriptManager, this);
}
