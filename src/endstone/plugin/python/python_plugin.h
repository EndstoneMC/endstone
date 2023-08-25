//
// Created by Vincent on 17/08/2023.
//

#ifndef ENDSTONE_PYTHON_PLUGIN_H
#define ENDSTONE_PYTHON_PLUGIN_H

#include "endstone/plugin/plugin.h"
#include "pybind/pybind.h"
#include "python_plugin_description.h"

class PythonPlugin : public Plugin {
public:
    explicit PythonPlugin(py::object impl);
    ~PythonPlugin() override;
    const PluginDescription &getDescription() const override;
    void onLoad() override;
    void onEnable() override;
    void onDisable() override;
    std::shared_ptr<Logger> getLogger() const override;
    bool isEnabled() const override;
    std::shared_ptr<const PluginLoader> getPluginLoader() const override;
    bool onCommand(CommandSender &sender, const Command &command, const std::string &label,
                   const std::vector<std::string> &args) const noexcept override;

    friend class PythonPluginLoader;

private:
    py::object impl_;
    std::weak_ptr<const PluginLoader> loader_;
    std::shared_ptr<Logger> logger_;
    std::unique_ptr<PythonPluginDescription> description_;
};

#endif // ENDSTONE_PYTHON_PLUGIN_H
