//
// Created by Vincent on 31/07/2023.
//

#ifndef ENDSTONE_SERVER_H
#define ENDSTONE_SERVER_H

#include "endstone.h"
#include "logger.h"
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
    void disablePlugins();
    void clearPlugins();

  private:
    Server();

  private:
    Logger &logger_;

    py::scoped_interpreter interpreter_{};
    py::object pluginManager_;
    py::gil_scoped_release release_{}; // Must be the last member to re-acquire GIL before destroying the PluginManager
};

#endif // ENDSTONE_SERVER_H
