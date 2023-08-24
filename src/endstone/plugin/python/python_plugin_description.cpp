//
// Created by Vincent on 22/08/2023.
//

#include "python_plugin_description.h"

PythonPluginDescription::PythonPluginDescription(py::object impl) : impl_(std::move(impl)) {}

PythonPluginDescription::~PythonPluginDescription()
{
    py::gil_scoped_acquire lock{};
    impl_.release();
}

std::string PythonPluginDescription::getName() const
{
    py::gil_scoped_acquire lock{};
    return impl_.attr("get_name")().cast<std::string>();
}

std::string PythonPluginDescription::getVersion() const
{
    py::gil_scoped_acquire lock{};
    return impl_.attr("get_version")().cast<std::string>();
}

std::optional<std::string> PythonPluginDescription::getDescription() const
{
    py::gil_scoped_acquire lock{};
    return impl_.attr("get_description")().cast<std::optional<std::string>>();
}

std::optional<std::vector<std::string>> PythonPluginDescription::getAuthors() const
{
    py::gil_scoped_acquire lock{};
    return impl_.attr("get_authors")().cast<std::optional<std::vector<std::string>>>();
}

std::optional<std::string> PythonPluginDescription::getPrefix() const
{
    py::gil_scoped_acquire lock{};
    return impl_.attr("get_prefix")().cast<std::optional<std::string>>();
}

std::string PythonPluginDescription::getFullName() const
{
    py::gil_scoped_acquire lock{};
    return impl_.attr("get_fullname")().cast<std::string>();
}
