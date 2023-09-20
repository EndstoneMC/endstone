// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "endstone/logger.h"
#include "pybind/pybind.h"

void def_logger(py::module &m)
{
    auto logger = py::class_<Logger>(m, "Logger")
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
        .value("ALL", Logger::All)
        .value("VERBOSE", Logger::Verbose)
        .value("INFO", Logger::Info)
        .value("WARNING", Logger::Warning)
        .value("ERROR", Logger::Error)
        .export_values();
}
