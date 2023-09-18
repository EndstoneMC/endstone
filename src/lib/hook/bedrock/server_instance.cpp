// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include "bedrock/server_instance.h"

#include "endstone/endstone_server.h"
#include "hook/hook.h"

void ServerInstance::startServerThread()
{
    EndstoneServer::getInstance().loadPlugins();
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstance::startServerThread, this)
}

void ServerInstanceEventCoordinator::sendServerThreadStarted(ServerInstance &instance)
{
    EndstoneServer::getInstance().enablePlugins();
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerThreadStarted, this, instance)
}

void ServerInstanceEventCoordinator::sendServerThreadStopped(ServerInstance &instance)
{
    EndstoneServer::getInstance().disablePlugins();
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerThreadStopped, this, instance)
}

void ServerInstanceEventCoordinator::sendServerUpdateEnd(ServerInstance &instance)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerUpdateEnd, this, instance)
}
