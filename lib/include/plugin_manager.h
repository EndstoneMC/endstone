//
// Created by Vincent on 22/07/2023.
//

#ifndef ENDSTONE_PLUGIN_MANAGER_H
#define ENDSTONE_PLUGIN_MANAGER_H

#include <utility>

#include "endstone.h"
#include "endstone_py.h"

class Server;

class PluginManager
{
  public:
    explicit PluginManager(Server &server);

    py::object load_plugin(const std::filesystem::path &path);
    std::vector<py::object> load_plugins(const std::filesystem::path &directory);
    [[nodiscard]] const std::vector<py::object> &getPlugins() const;

  private:
    Server &_server;
    std::vector<py::object> plugins;
};

#endif // ENDSTONE_PLUGIN_MANAGER_H
