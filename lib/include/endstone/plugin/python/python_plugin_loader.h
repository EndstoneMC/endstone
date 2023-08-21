//
// Created by Vincent on 18/08/2023.
//

#ifndef ENDSTONE_PYTHON_PLUGIN_LOADER_H
#define ENDSTONE_PYTHON_PLUGIN_LOADER_H

#include "endstone/plugin/plugin_loader.h"
#include "endstone/pybind.h"

class PythonPluginLoader : public PluginLoader, public std::enable_shared_from_this<PythonPluginLoader>
{
  public:
    PythonPluginLoader(const std::string &module_name, const std::string &class_name);
    ~PythonPluginLoader() override;
    Plugin *loadPlugin(const std::string &file) const override;
    std::vector<std::string> getPluginFilters() const noexcept override;
    void enablePlugin(Plugin &plugin) const override;
    void disablePlugin(Plugin &plugin) const override;

  private:
    py::object impl_;
};

#endif // ENDSTONE_PYTHON_PLUGIN_LOADER_H
