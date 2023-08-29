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
        .def("set_level", &Logger::setLevel)
        .def("is_enabled_for", &Logger::isEnabledFor)
        .def("get_name", &Logger::getName)
        .def("log", (void(Logger::*)(LogLevel, const std::string &) const) & Logger::log)
        .def("verbose",
             [](const Logger &logger, const std::string &message) {
                 logger.verbose(message);
             })
        .def("info",
             [](const Logger &logger, const std::string &message) {
                 logger.info(message);
             })
        .def("warning",
             [](const Logger &logger, const std::string &message) {
                 logger.warning(message);
             })
        .def("error", [](const Logger &logger, const std::string &message) {
            logger.error(message);
        });
}
