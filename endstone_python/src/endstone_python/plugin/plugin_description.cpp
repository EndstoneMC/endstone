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

#include "endstone/plugin/plugin_description.h"

#include <utility>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "endstone/logger.h"
#include "endstone/plugin/plugin.h"
#include "endstone/server.h"
#include "endstone_python/endstone_python.h"

namespace py = pybind11;

PluginDescription createPluginDescription(std::string name, std::string version, std::optional<std::string> description,
                                          std::optional<std::vector<std::string>> authors,
                                          std::optional<std::string> prefix, const py::args & /*args*/,
                                          const py::kwargs & /*kwargs*/)
{
    return {std::move(name), std::move(version), std::move(description), std::move(authors), std::move(prefix)};
}

void def_plugin_description(py::module &m)
{
    py::class_<PluginDescription>(m, "PluginDescription")
        .def(py::init(&createPluginDescription), py::arg("name"), py::arg("version"),
             py::arg("description") = py::none(), py::arg("authors") = py::none(), py::arg("prefix") = py::none())
        .def_property_readonly("name", &PluginDescription::getName)
        .def_property_readonly("version", &PluginDescription::getVersion)
        .def_property_readonly("full_name", &PluginDescription::getFullName)
        .def_property_readonly("description", &PluginDescription::getDescription)
        .def_property_readonly("authors", &PluginDescription::getAuthors)
        .def_property_readonly("prefix", &PluginDescription::getPrefix);
}
