//
// Created by Vincent on 18/08/2023.
//

#ifndef ENDSTONE_PYTHON_PLUGIN_LOADER_H
#define ENDSTONE_PYTHON_PLUGIN_LOADER_H

#include "endstone/plugin/plugin_loader.h"

#include <pybind11/pybind11.h>
namespace py = pybind11;

class PythonPluginLoader : public PluginLoader {
public:
    PythonPluginLoader(const std::string &module_name, const std::string &class_name);
    ~PythonPluginLoader() override;
    std::unique_ptr<Plugin> loadPlugin(const std::string &file) override;
    std::vector<std::string> getPluginFileFilters() const noexcept override;
    void enablePlugin(Plugin &plugin) const noexcept override;
    void disablePlugin(Plugin &plugin) const noexcept override;

private:
    std::shared_ptr<PluginLoader> loader_;
    py::object py_loader_; // keep the python instance alive
};

#endif // ENDSTONE_PYTHON_PLUGIN_LOADER_H
