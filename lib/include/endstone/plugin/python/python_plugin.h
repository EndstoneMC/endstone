//
// Created by Vincent on 17/08/2023.
//

#ifndef ENDSTONE_PYTHON_PLUGIN_H
#define ENDSTONE_PYTHON_PLUGIN_H

#include "endstone/plugin/plugin.h"
#include "endstone/pybind.h"

class PythonPlugin : public Plugin
{
  public:
    PythonPlugin(py::object impl);
    ~PythonPlugin() override;
    PluginDescription &getDescription() const override;
    void onLoad() override;
    void onEnable() override;
    void onDisable() override;
    Logger &getLogger() const override;
    bool isEnabled() const override;
    std::shared_ptr<const PluginLoader> getPluginLoader() const override;

    friend class PythonPluginLoader;

  private:
    std::weak_ptr<const PluginLoader> loader_;
    py::object impl_;
};

#endif // ENDSTONE_PYTHON_PLUGIN_H
