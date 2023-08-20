//
// Created by Vincent on 18/08/2023.
//

#include "endstone/plugin/python/python_plugin_loader.h"
#include "common.h"
#include "endstone/logger.h"
#include "endstone/plugin/python/python_plugin.h"

PythonPluginLoader::PythonPluginLoader(const std::string &module_name, const std::string &class_name)
{
    try
    {
        py::gil_scoped_acquire lock{};
        auto module = py::module_::import(module_name.c_str());
        auto cls = module.attr(class_name.c_str());
        impl_ = cls();
    }
    catch (std::exception &e)
    {
        auto logger = Logger::getLogger("PythonPluginLoader");
        logger.error("Error occurred when initializing python plugin loader '%s.%s': %s",
                     module_name.c_str(),
                     class_name.c_str(),
                     e.what());
        throw e;
    }
}

PythonPluginLoader::~PythonPluginLoader()
{
    py::gil_scoped_acquire lock{};
    impl_.release();
}

Plugin *PythonPluginLoader::loadPlugin(const std::string &file)
{
    py::gil_scoped_acquire lock{};
    auto py_plugin = impl_.attr("load_plugin")(file);
    return new PythonPlugin(py_plugin);
}

std::vector<std::string> PythonPluginLoader::getPluginFilters()
{
    py::gil_scoped_acquire lock{};
    return impl_.attr("get_plugin_filters")().cast<std::vector<std::string>>();
}

void PythonPluginLoader::enablePlugin(const Plugin &plugin)
{
    py::gil_scoped_acquire lock{};
    impl_.attr("enable_plugin")(plugin);
}

void PythonPluginLoader::disablePlugin(const Plugin &plugin)
{
    py::gil_scoped_acquire lock{};
    impl_.attr("disable_plugin")(plugin);
}
