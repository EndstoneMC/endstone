//
// Created by Vincent on 17/08/2023.
//

#include "endstone/common.h"
#include "endstone/logger.h"
#include "pybind.h"

PYBIND11_MODULE(_logger, m)
{
    py::enum_<LogLevel>(m, "LogLevel")
        .value("ALL", LogLevel::All)
        .value("VERBOSE", LogLevel::Verbose)
        .value("INFO", LogLevel::Info)
        .value("WARNING", LogLevel::Warning)
        .value("ERROR", LogLevel::Error)
        .export_values();

    py::class_<Logger, std::shared_ptr<Logger>>(m, "Logger")
        .def("set_level", &Logger::setLevel, py::arg("level"))
        .def("is_enabled_for", &Logger::isEnabledFor, py::arg("level"))
        .def_property_readonly("name", &Logger::getName)
        .def(
            "log",
            [](const Logger &logger, LogLevel level, const std::string &message) {
                logger.log(level, message);
            },
            py::arg("level"), py::arg("msg"))
        .def(
            "verbose",
            [](const Logger &logger, const std::string &message) {
                logger.verbose(message);
            },
            py::arg("msg"))
        .def(
            "info",
            [](const Logger &logger, const std::string &message) {
                logger.info(message);
            },
            py::arg("msg"))
        .def(
            "warning",
            [](const Logger &logger, const std::string &message) {
                logger.warning(message);
            },
            py::arg("msg"))
        .def(
            "error",
            [](const Logger &logger, const std::string &message) {
                logger.error(message);
            },
            py::arg("msg"));
}
