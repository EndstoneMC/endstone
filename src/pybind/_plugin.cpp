//
// Created by Vincent on 17/08/2023.
//

#include "endstone/plugin/plugin_logger.h"
#include "pybind.h"

PYBIND11_MODULE(_plugin, m)
{
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
