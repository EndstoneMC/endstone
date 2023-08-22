//
// Created by Vincent on 31/07/2023.
//

#ifndef ENDSTONE_SERVER_H
#define ENDSTONE_SERVER_H

#include "common.h"
#include "logger.h"
#include "plugin/plugin_manager.h"

class Server
{
  public:
    Server(const Server &) = delete;            // Disable copy constructor
    Server &operator=(const Server &) = delete; // Disable assignment operator

    void loadPlugins();
    void enablePlugins();
    void disablePlugins();
    std::shared_ptr<Logger> getLogger();

    Server();
    ~Server() = default;

  private:
    std::shared_ptr<Logger> logger_;
    std::unique_ptr<PluginManager> pluginManager_;
};

#endif // ENDSTONE_SERVER_H
