//
// Created by Vincent on 17/08/2023.
//

#include "endstone/common.h"
#include "endstone/logger.h"
#include "pybind.h"

void def_logger(py::module &m)
{
    auto logger = py::class_<Logger, py::smart_holder>(m, "Logger")
                      .def("set_level", &Logger::setLevel, py::arg("level"))
                      .def("is_enabled_for", &Logger::isEnabledFor, py::arg("level"))
                      .def_property_readonly("name", &Logger::getName)
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

    py::enum_<Logger::Level>(logger, "Level")
        .value("ALL", Logger::Level::All)
        .value("VERBOSE", Logger::Level::Verbose)
        .value("INFO", Logger::Level::Info)
        .value("WARNING", Logger::Level::Warning)
        .value("ERROR", Logger::Level::Error)
        .export_values();
}
