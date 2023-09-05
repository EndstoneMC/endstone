//
// Created by Vincent on 05/09/2023.
//

#include "bedrock/server_instance.h"

#include "../../hook/hook.h"

void ServerInstance::startServerThread()
{
    printf("Hook is called: %s\n", __FUNCSIG__);
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstance::startServerThread, this);
}

void ServerInstanceEventCoordinator::sendServerThreadStarted(ServerInstance &instance)
{
    printf("Hook is called: %s\n", __FUNCSIG__);
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerThreadStarted, this, instance);
}

void ServerInstanceEventCoordinator::sendServerThreadStopped(ServerInstance &instance)
{
    printf("Hook is called: %s\n", __FUNCSIG__);
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerThreadStopped, this, instance);
}

void ServerInstanceEventCoordinator::sendServerUpdateEnd(ServerInstance &instance)
{
    printf("Hook is called: %s\n", __FUNCSIG__);
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerUpdateEnd, this, instance);
}
