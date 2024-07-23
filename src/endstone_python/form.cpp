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

#include <utility>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// must be included after pybind11
#include "endstone/form/form.h"
#include "endstone/form/message_form.h"
#include "endstone/message.h"

namespace py = pybind11;

namespace endstone::detail {

void init_form(py::module_ &m)
{
    py::class_<endstone::MessageForm>(m, "MessageForm", "Represents a form with two buttons.")
        .def(py::init<>([](Message title, Message content, Message button1, Message button2) {
                 return MessageForm()
                     .setTitle(std::move(title))
                     .setContent(std::move(content))
                     .setButton1(std::move(button1))
                     .setButton2(std::move(button2));
             }),
             py::arg("title") = "", py::arg("content") = "", py::arg("button1") = "", py::arg("button2") = "")
        .def_property("title", &endstone::MessageForm::getTitle, &endstone::MessageForm::setTitle,
                      "Gets or sets the title of the form.")
        .def_property("content", &endstone::MessageForm::getContent, &endstone::MessageForm::setContent,
                      "Gets or sets the content of the form.")
        .def_property("button1", &endstone::MessageForm::getButton1, &endstone::MessageForm::setButton1,
                      "Gets or sets the text of button1.")
        .def_property("button2", &endstone::MessageForm::getButton2, &endstone::MessageForm::setButton2,
                      "Gets or sets the text of button2.");
}

}  // namespace endstone::detail
