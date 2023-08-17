//
// Created by Vincent on 31/07/2023.
//

#ifndef ENDSTONE_SERVER_H
#define ENDSTONE_SERVER_H

#include "common.h"
#include "logger.h"
#include "plugin_manager.h"

class Server
{
  public:
    Server(const Server &) = delete;            // Disable copy constructor
    Server &operator=(const Server &) = delete; // Disable assignment operator

    void start();
    void loadPlugins();
    void enablePlugins();
    void disablePlugins();
    void clearPlugins();
    const Logger &getLogger();

    Server();
    ~Server()
    {
        py::gil_scoped_acquire lock{};
        pluginManager_.release();
    };

  private:
    Logger &logger_;

    //    py::scoped_interpreter interpreter_{};
    py::object pluginManager_;
    //    py::gil_scoped_release release_{}; // Must be the last member to re-acquire GIL before destroying the
    //    PluginManager
};

#endif // ENDSTONE_SERVER_H
