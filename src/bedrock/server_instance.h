//
// Created by Vincent on 02/08/2023.
//

#ifndef ENDSTONE_SERVER_INSTANCE_H
#define ENDSTONE_SERVER_INSTANCE_H

#include "bedrock_common.h"

class ServerInstance {
public:
    BEDROCK_API void startServerThread();
};

class ServerInstanceEventCoordinator {
public:
    BEDROCK_API void sendServerThreadStarted(ServerInstance &instance);
    BEDROCK_API void sendServerThreadStopped(ServerInstance &instance);
    BEDROCK_API void sendServerUpdateEnd(ServerInstance &instance);
};

#endif // ENDSTONE_SERVER_INSTANCE_H
