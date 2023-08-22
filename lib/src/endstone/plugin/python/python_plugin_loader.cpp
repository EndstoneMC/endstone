//
// Created by Vincent on 18/08/2023.
//

#include "endstone/plugin/python/python_plugin_loader.h"
#include "endstone/common.h"
#include "endstone/plugin/plugin_logger.h"
#include "endstone/plugin/python/python_plugin.h"

PythonPluginLoader::PythonPluginLoader(const std::string &module_name, const std::string &class_name)
{
    py::gil_scoped_acquire lock{};
    auto module = py::module_::import(module_name.c_str());
    auto cls = module.attr(class_name.c_str());
    impl_ = cls();
}

PythonPluginLoader::~PythonPluginLoader()
{
    py::gil_scoped_acquire lock{};
    impl_.release();
}

Plugin *PythonPluginLoader::loadPlugin(const std::string &file) const
{
    py::gil_scoped_acquire lock{};
    auto py_plugin = impl_.attr("load_plugin")(file);
    auto plugin = new PythonPlugin(py_plugin);
    plugin->loader_ = shared_from_this();
    plugin->logger_ = std::make_shared<PluginLogger>(*plugin);
    return plugin;
}

std::vector<std::string> PythonPluginLoader::getPluginFilters() const noexcept
{
    py::gil_scoped_acquire lock{};
    return impl_.attr("get_plugin_filters")().cast<std::vector<std::string>>();
}

void PythonPluginLoader::enablePlugin(Plugin &plugin) const
{
    try
    {
        auto &py_plugin = dynamic_cast<PythonPlugin &>(plugin);
        py::gil_scoped_acquire lock{};
        impl_.attr("enable_plugin")(py_plugin.impl_);
    }
    catch (const std::bad_cast &e)
    {
        throw std::runtime_error("Plugin is not associated with this PluginLoader");
    }
}

void PythonPluginLoader::disablePlugin(Plugin &plugin) const
{
    try
    {
        const auto &py_plugin = dynamic_cast<const PythonPlugin &>(plugin);
        py::gil_scoped_acquire lock{};
        impl_.attr("disable_plugin")(py_plugin.impl_);
    }
    catch (const std::bad_cast &e)
    {
        throw std::runtime_error("Plugin is not associated with this PluginLoader");
    }
}
