//
// Created by Vincent on 17/08/2023.
//

#include "endstone/plugin/python/python_plugin.h"

#include "endstone/logger.h"
#include "endstone/plugin/plugin_logger.h"

PythonPlugin::PythonPlugin(py::object impl) : impl_(std::move(impl))
{
    description_ = std::make_unique<PythonPluginDescription>(impl_.attr("description"), *this);
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
    return impl_.attr("enabled").cast<bool>();
}

std::shared_ptr<const PluginLoader> PythonPlugin::getPluginLoader() const
{
    return loader_.lock();
}

bool PythonPlugin::onCommand(CommandSender &sender, const Command &command, const std::string &label,
                             const std::vector<std::string> &args) const noexcept
{
    py::gil_scoped_acquire lock{};
    //    return impl_.attr("on_command_dummy")().cast<bool>();
    return impl_.attr("on_command")(std::ref(sender), std::ref(command), label, args).cast<bool>();
}
