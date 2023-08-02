//
// Created by Vincent on 02/08/2023.
//

#ifndef ENDSTONE_SERVER_INSTANCE_H
#define ENDSTONE_SERVER_INSTANCE_H

class ServerInstance
{
  public:
    void startServerThread();
};

class ServerInstanceEventCoordinator
{
  public:
    void sendServerThreadStarted(ServerInstance *serverInstance);
    void sendServerThreadStopped(ServerInstance *serverInstance);
    void sendServerUpdateEnd(ServerInstance *serverInstance);
};

#endif // ENDSTONE_SERVER_INSTANCE_H
