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
#include "endstone/form/action_form.h"
#include "endstone/form/controls/button.h"
#include "endstone/form/form.h"
#include "endstone/form/message_form.h"
#include "endstone/message.h"

namespace py = pybind11;

namespace endstone::detail {

void init_form(py::module_ &m)
{
    py::class_<Button>(m, "Button", "Represents a button with text and an optional icon.")
        .def(py::init<Message, std::optional<std::string>>(), py::arg("text") = "", py::arg("icon") = py::none())
        .def_property("text", &Button::getText, &Button::setText, "Gets or sets the text of the button")
        .def_property("icon", &Button::getIcon, &Button::setIcon, "Gets or sets the icon path or URL of the button");

    py::class_<MessageForm>(m, "MessageForm", "Represents a form with two buttons.")
        .def(py::init<>([](Message title, Message content, Message button1, Message button2) {
                 return MessageForm()
                     .setTitle(std::move(title))
                     .setContent(std::move(content))
                     .setButton1(std::move(button1))
                     .setButton2(std::move(button2));
             }),
             py::arg("title") = "", py::arg("content") = "", py::arg("button1") = "", py::arg("button2") = "")
        .def_property("title", &MessageForm::getTitle, &MessageForm::setTitle, "Gets or sets the title of the form.",
                      py::return_value_policy::reference)
        .def_property("content", &MessageForm::getContent, &MessageForm::setContent,
                      "Gets or sets the content of the form.", py::return_value_policy::reference)
        .def_property("button1", &MessageForm::getButton1, &MessageForm::setButton1,
                      "Gets or sets the text of button1.", py::return_value_policy::reference)
        .def_property("button2", &MessageForm::getButton2, &MessageForm::setButton2,
                      "Gets or sets the text of button2.", py::return_value_policy::reference);

    py::class_<ActionForm>(m, "ActionForm", "Represents a form with buttons that let the player take action.")
        .def(py::init<>([](Message title, Message content, const std::optional<std::vector<Button>> &buttons) {
                 return ActionForm()
                     .setTitle(std::move(title))
                     .setContent(std::move(content))
                     .setButtons(buttons.value_or(std::vector<Button>{}));
             }),
             py::arg("title") = "", py::arg("content") = "", py::arg("buttons") = py::none())
        .def_property("title", &ActionForm::getTitle, &ActionForm::setTitle, "Gets or sets the title of the form.",
                      py::return_value_policy::reference)
        .def_property("content", &ActionForm::getContent, &ActionForm::setContent,
                      "Gets or sets the content of the form.", py::return_value_policy::reference)
        .def("add_button", &ActionForm::addButton, "Adds a button to the form.", py::arg("text"),
             py::arg("icon") = py::none(), py::return_value_policy::reference)
        .def_property("buttons", &ActionForm::getButtons, &ActionForm::setButtons,
                      "Gets or sets the buttons of the action form.", py::return_value_policy::reference);
}

}  // namespace endstone::detail
