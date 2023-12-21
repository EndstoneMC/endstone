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

#include <pybind11/pybind11.h>

#include "endstone_python/endstone_python.h"

namespace py = pybind11;

void def_logger(py::module &m)
{
    auto logger =  //
        py::class_<Logger>(m, "Logger")
            .def("set_level", &Logger::setLevel, py::arg("level"))
            .def("is_enabled_for", &Logger::isEnabledFor, py::arg("level"))
            .def(
                "trace",
                [](const Logger &logger, const std::string &message) {
                    logger.trace(message);
                },
                py::arg("msg"))
            .def(
                "debug",
                [](const Logger &logger, const std::string &message) {
                    logger.debug(message);
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
                py::arg("msg"))
            .def(
                "critical",
                [](const Logger &logger, const std::string &message) {
                    logger.critical(message);
                },
                py::arg("msg"))
            .def_property_readonly("name", &Logger::getName);

    py::enum_<Logger::Level>(logger, "Level")
        .value("TRACE", Logger::Level::Trace)
        .value("DEBUG", Logger::Level::Debug)
        .value("INFO", Logger::Level::Info)
        .value("WARNING", Logger::Level::Warning)
        .value("ERROR", Logger::Level::Error)
        .value("CRITICAL", Logger::Level::Critical)
        .export_values();
}
