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

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {
void init_lang(py::module_ &m)
{
    py::class_<Translatable>(
        m, "Translatable",
        "Represents an object with a text representation that can be translated by the Minecraft client.")
        .def(py::init([](std::string text, const std::optional<std::vector<std::string>> &params) {
                 return Translatable(std::move(text), params.value_or(std::vector<std::string>{}));
             }),
             py::arg("text"), py::arg("params") = py::none())
        .def_property_readonly("text", &Translatable::getText, "The text to be translated.")
        .def_property_readonly("params", &Translatable::getParameters, "The translation parameters.");

    py::class_<Language>(m, "Language", "Represents the interface for translating text into different languages.")
        .def(
            "translate",
            [](const Language &self, const std::string &text, const std::optional<std::vector<std::string>> &params,
               const std::optional<std::string> &locale) {
                return self.translate(text, params.value_or(std::vector<std::string>{}),
                                      locale.value_or(self.getLocale()));
            },
            py::arg("text"), py::arg("params") = std::nullopt, py::arg("locale") = std::nullopt, R"doc(
    Translates a given text using a set of parameters for a specific locale.

    Args:
        text: The text to be translated.
        params: A list of parameters to be used in the translation.
        locale: The locale identifier for the desired translation (e.g., ``en_US``). Defaults to the current locale.

    Returns:
        The translated text in the specified locale, with parameters applied.
)doc")
        .def(
            "translate",
            [](const Language &self, const Translatable &translatable, const std::optional<std::string> &locale) {
                return self.translate(translatable, locale.value_or(self.getLocale()));
            },
            py::arg("translatable"), py::arg("locale") = std::nullopt, R"doc(
    Translates a Translatable object into a specific locale.

    Args:
        translatable: A Translatable object containing text and parameters.
        locale: The locale identifier for the desired translation (e.g., ``en_US``). Defaults to the current locale.

    Returns:
        The translated text in the specified locale.
)doc")
        .def_property_readonly("locale", &Language::getLocale, "The current locale.");
}

}  // namespace endstone::python
