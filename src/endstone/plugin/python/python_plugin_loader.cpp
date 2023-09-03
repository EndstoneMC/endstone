//
// Created by Vincent on 18/08/2023.
//

#include "endstone/plugin/python/python_plugin_loader.h"

#include "endstone/common.h"
#include "endstone/plugin/plugin_logger.h"
#include "endstone/server.h"

PythonPluginLoader::PythonPluginLoader(Server &server, const std::string &module_name, const std::string &class_name)
    : PluginLoader(server)
{
    py::gil_scoped_acquire gil{};
    auto module = py::module_::import(module_name.c_str());
    auto cls = module.attr(class_name.c_str());
    auto py_loader = cls(std::ref(server));
    loader_ = py_loader.cast<std::shared_ptr<PluginLoader>>();
}

std::unique_ptr<Plugin> PythonPluginLoader::loadPlugin(const std::string &file)
{
    auto plugin = std::move(loader_->loadPlugin(file));
    initPlugin(*plugin, std::make_shared<PluginLogger>(*plugin));
    return plugin;
}

std::vector<std::string> PythonPluginLoader::getPluginFileFilters() const noexcept
{
    return std::move(loader_->getPluginFileFilters());
}

void PythonPluginLoader::enablePlugin(Plugin &plugin) const noexcept
{
    loader_->enablePlugin(plugin);
}

void PythonPluginLoader::disablePlugin(Plugin &plugin) const noexcept
{
    loader_->disablePlugin(plugin);
}
