//
// Created by Vincent on 17/08/2023.
//

#include <utility>

#include "endstone/plugin/plugin_logger.h"
#include "endstone/plugin/python/python_plugin.h"

PythonPlugin::PythonPlugin(py::object impl) : impl_(std::move(impl))
{
}

PythonPlugin::~PythonPlugin()
{
    py::gil_scoped_acquire lock{};
    impl_.release();
}

PluginDescription &PythonPlugin::getDescription() const
{
    py::gil_scoped_acquire lock{};
    return impl_.attr("get_description")().cast<PluginDescription &>();
}

void PythonPlugin::onLoad()
{
    py::gil_scoped_acquire lock{};
    impl_.attr("on_load")();
}

void PythonPlugin::onEnable()
{
    py::gil_scoped_acquire lock{};
    impl_.attr("on_enable")();
}

void PythonPlugin::onDisable()
{
    py::gil_scoped_acquire lock{};
    impl_.attr("on_disable")();
}

Logger &PythonPlugin::getLogger() const
{
    py::gil_scoped_acquire lock{};
    return impl_.attr("get_logger")().cast<Logger &>();
}

bool PythonPlugin::isEnabled() const
{
    py::gil_scoped_acquire lock{};
    return impl_.attr("is_enabled")().cast<bool>();
}

std::shared_ptr<const PluginLoader> PythonPlugin::getPluginLoader() const
{
    return loader_.lock();
}
