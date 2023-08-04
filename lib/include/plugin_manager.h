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

    py::object loadPlugin(const std::filesystem::path &path);
    std::vector<py::object> loadPlugins(const std::filesystem::path &directory);
    void enablePlugin(const py::object &plugin) const;
    void enablePlugins() const;
    void disablePlugin(const py::object &plugin) const;
    void disablePlugins() const;
    void clearPlugins();
    [[nodiscard]] const std::vector<py::object> &getPlugins() const;

  private:
    Server &server_;
    std::vector<py::object> plugins_;
};

#endif // ENDSTONE_PLUGIN_MANAGER_H
