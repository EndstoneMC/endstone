//
// Created by Vincent on 31/07/2023.
//

#ifndef ENDSTONE_SERVER_H
#define ENDSTONE_SERVER_H

#include "enstone.h"
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

    [[nodiscard]] PluginManager &getPluginManager() const;
    void enablePlugin(const py::object &plugin) const;
    void enablePlugins() const;

  private:
    Server();

  private:
    std::unique_ptr<PluginManager> pluginManager;
};

#endif // ENDSTONE_SERVER_H
