//
// Created by Vincent on 31/07/2023.
//

#ifndef ENDSTONE_SERVER_H
#define ENDSTONE_SERVER_H

#include "endstone.h"
#include "plugin_manager.h"

class Server
{
  public:
    static Server &getInstance()
    {
        static Server instance;
        return instance;
    }

    Server(const Server &) = delete;            // Disable copy constructor
    Server &operator=(const Server &) = delete; // Disable assignment operator

    void loadPlugins();
    void enablePlugins();

  private:
    Server();

  private:
    py::object pluginManager_;
};

#endif // ENDSTONE_SERVER_H
