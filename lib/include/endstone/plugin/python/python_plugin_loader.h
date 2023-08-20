//
// Created by Vincent on 18/08/2023.
//

#ifndef ENDSTONE_PYTHON_PLUGIN_LOADER_H
#define ENDSTONE_PYTHON_PLUGIN_LOADER_H

#include "endstone/plugin/plugin_loader.h"

class PythonPluginLoader : public PluginLoader
{
  public:
    PythonPluginLoader(const std::string &module_name, const std::string &class_name);
    ~PythonPluginLoader() override;
    Plugin *loadPlugin(const std::string &file) override;
    std::vector<std::string> getPluginFilters() override;
    void enablePlugin(const Plugin &plugin) override;
    void disablePlugin(const Plugin &plugin) override;

  private:
    py::object impl_;
};

#endif // ENDSTONE_PYTHON_PLUGIN_LOADER_H
