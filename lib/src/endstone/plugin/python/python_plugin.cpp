//
// Created by Vincent on 17/08/2023.
//

#include <utility>

#include "endstone/logger.h"
#include "endstone/plugin/plugin_logger.h"
#include "endstone/plugin/python/python_plugin.h"

PythonPlugin::PythonPlugin(py::object impl) : impl_(std::move(impl))
{
    description_ = std::make_unique<PythonPluginDescription>(impl_.attr("get_description")());
}

PythonPlugin::~PythonPlugin()
{
    py::gil_scoped_acquire lock{};
    impl_.release();
}

const PluginDescription &PythonPlugin::getDescription() const
{
    return *description_;
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

std::shared_ptr<Logger> PythonPlugin::getLogger() const
{
    return logger_;
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

void PythonPlugin::init(const std::shared_ptr<const PluginLoader> &loader)
{
    loader_ = loader;
    logger_ = std::make_shared<PluginLogger>(*this);
}
