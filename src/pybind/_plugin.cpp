//
// Created by Vincent on 17/08/2023.
//

#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_loader.h"
#include "endstone/plugin/plugin_logger.h"
#include "pybind.h"

class PyPluginDescription : public PluginDescription {
public:
    using PluginDescription::PluginDescription;

    std::string getName() const override
    {
        PYBIND11_OVERRIDE_PURE_NAME(std::string, PluginDescription, "get_name", getName);
    }

    std::string getVersion() const override
    {
        PYBIND11_OVERRIDE_PURE_NAME(std::string, PluginDescription, "get_version", getVersion);
    }

    std::optional<std::string> getDescription() const override
    {
        PYBIND11_OVERRIDE_PURE_NAME(std::optional<std::string>, PluginDescription, "get_description", getDescription);
    }

    std::optional<std::vector<std::string>> getAuthors() const override
    {
        PYBIND11_OVERRIDE_PURE_NAME(std::optional<std::vector<std::string>>, PluginDescription, "get_authors",
                                    getAuthors);
    }

    std::optional<std::string> getPrefix() const override
    {
        PYBIND11_OVERRIDE_PURE_NAME(std::optional<std::string>, PluginDescription, "get_prefix", getPrefix);
    }

    std::string getFullName() const override
    {
        return getName() + " v" + getVersion();
    }
};

PYBIND11_MODULE(_plugin, m)
{
    py::class_<PluginDescription, PyPluginDescription>(m, "IPluginDescription") //
        .def(py::init<>())                                                      //
        .def("get_name", &PluginDescription::getName)                           //
        .def("get_version", &PluginDescription::getVersion)                     //
        .def("get_description", &PluginDescription::getDescription)             //
        .def("get_authors", &PluginDescription::getAuthors)                     //
        .def("get_prefix", &PluginDescription::getPrefix)                       //
        .def("get_fullname", &PluginDescription::getFullName);

    py::class_<PluginLogger, std::shared_ptr<PluginLogger>>(m, "PluginLogger")
        .def(py::init<const Plugin &>())
        .def(
            "log",
            [](const PluginLogger &logger, const LogLevel level, const std::string &msg) {
                return logger.log(level, msg);
            },
            py::arg("level"), py::arg("msg"))
        .def(
            "verbose",
            [](const PluginLogger &logger, const std::string &msg) {
                return logger.log(LogLevel::Verbose, msg);
            },
            py::arg("msg"))
        .def(
            "info",
            [](const PluginLogger &logger, const std::string &msg) {
                return logger.log(LogLevel::Info, msg);
            },
            py::arg("msg"))
        .def(
            "warning",
            [](const PluginLogger &logger, const std::string &msg) {
                return logger.log(LogLevel::Warning, msg);
            },
            py::arg("msg"))
        .def(
            "error",
            [](const PluginLogger &logger, const std::string &msg) {
                return logger.log(LogLevel::Error, msg);
            },
            py::arg("msg"))
        .def("set_level", &PluginLogger::setLevel, py::arg("level"))
        .def("get_name", &PluginLogger::getName);
}
