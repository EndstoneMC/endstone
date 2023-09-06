//
// Created by Vincent on 05/09/2023.
//

#include "bedrock/server_instance.h"

#include "endstone/endstone.h"
#include "endstone/endstone_server.h"
#include "hook/hook.h"

void ServerInstance::startServerThread()
{
    dynamic_cast<EndstoneServer &>(Endstone::getServer()).loadPlugins();
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstance::startServerThread, this);
}

void ServerInstanceEventCoordinator::sendServerThreadStarted(ServerInstance &instance)
{
    dynamic_cast<EndstoneServer &>(Endstone::getServer()).enablePlugins();
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerThreadStarted, this, instance);
}

void ServerInstanceEventCoordinator::sendServerThreadStopped(ServerInstance &instance)
{
    dynamic_cast<EndstoneServer &>(Endstone::getServer()).disablePlugins();
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerThreadStopped, this, instance);
}

void ServerInstanceEventCoordinator::sendServerUpdateEnd(ServerInstance &instance)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerUpdateEnd, this, instance);
}
