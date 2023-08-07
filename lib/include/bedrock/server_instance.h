//
// Created by Vincent on 02/08/2023.
//

#ifndef ENDSTONE_SERVER_INSTANCE_H
#define ENDSTONE_SERVER_INSTANCE_H

/**
 * @class ServerInstance
 * @brief This class represents an instance of a server.
 */
class ServerInstance
{
  public:
    /**
     * @brief Starts the server thread.
     * This function is used to initiate the server thread and begin its operation.
     */
    void startServerThread();
};

/**
 * @class ServerInstanceEventCoordinator
 * @brief This class coordinates events related to the server instance.
 * It sends various notifications and handles events specific to the server's life cycle.
 */
class ServerInstanceEventCoordinator
{
  public:
    /**
     * @brief Notifies that the server thread has started.
     * @param serverInstance Pointer to the server instance that started.
     */
    void sendServerThreadStarted(ServerInstance *serverInstance);

    /**
     * @brief Notifies that the server thread has stopped.
     * @param serverInstance Pointer to the server instance that stopped.
     */
    void sendServerThreadStopped(ServerInstance *serverInstance);

    /**
     * @brief Sends an update signal at the end of a server cycle.
     * @param serverInstance Pointer to the server instance being updated.
     */
    void sendServerUpdateEnd(ServerInstance *serverInstance);
};

#endif // ENDSTONE_SERVER_INSTANCE_H
