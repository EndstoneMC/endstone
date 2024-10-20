// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <optional>
#include <string>
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "endstone/lang/language.h"
#include "endstone/lang/translatable.h"

namespace py = pybind11;

namespace endstone::detail {
void init_lang(py::module_ &m)
{
    py::class_<Translatable>(
        m, "Translatable",
        "Represents an object with a text representation that can be translated by the Minecraft client.")
        .def(py::init([](std::string text, const std::optional<std::vector<std::string>> &params) {
                 return Translatable(std::move(text), params.value_or(std::vector<std::string>{}));
             }),
             py::arg("text"), py::arg("params") = py::none())
        .def_property_readonly("text", &Translatable::getText, "Get the text to be translated.")
        .def_property_readonly("params", &Translatable::getParameters, "Get the translation parameters.");

    py::class_<Language>(m, "Language", "Represents the interface for translating text into different languages.")
        .def(
            "translate",
            [](const Language &self, const std::string &text, const std::optional<std::vector<std::string>> &params,
               const std::optional<std::string> &locale) {
                return self.translate(text, params.value_or(std::vector<std::string>{}),
                                      locale.value_or(self.getLocale()));
            },
            "Translates a given text using a set of parameters for a specific locale.", py::arg("text"),
            py::arg("params") = std::nullopt, py::arg("locale") = std::nullopt)
        .def_property_readonly("locale", &Language::getLocale, "Gets the current locale.");
}

}  // namespace endstone::detail
