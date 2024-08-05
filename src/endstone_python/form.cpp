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

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// must be included after pybind11
#include "endstone/form/action_form.h"
#include "endstone/form/controls/dropdown.h"
#include "endstone/form/controls/label.h"
#include "endstone/form/controls/slider.h"
#include "endstone/form/controls/step_slider.h"
#include "endstone/form/controls/text_input.h"
#include "endstone/form/controls/toggle.h"
#include "endstone/form/message_form.h"
#include "endstone/form/modal_form.h"
#include "endstone/message.h"
#include "endstone/player.h"

namespace py = pybind11;

namespace endstone::detail {

void init_form(py::module_ &m)
{
    py::class_<Dropdown>(m, "Dropdown", "Represents a dropdown with a set of predefined options.")
        .def(py::init<>([](Message label, const std::optional<std::vector<std::string>> &options,
                           std::optional<int> default_index) {
                 return Dropdown(std::move(label), options.value_or(std::vector<std::string>{}), default_index);
             }),
             py::arg("label") = "", py::arg("options") = py::none(), py::arg("default_index") = py::none())
        .def_property("label", &Dropdown::getLabel, &Dropdown::setLabel, "Gets or sets the label of the dropdown.")
        .def_property("options", &Dropdown::getOptions, &Dropdown::setOptions,
                      "Gets or sets the options of the dropdown.")
        .def_property("default_index", &Dropdown::getDefaultIndex, &Dropdown::setDefaultIndex,
                      "Gets or sets the optional default index of the dropdown.")
        .def("add_option", &Dropdown::addOption, "Adds a new option to the dropdown.", py::arg("option"));

    py::class_<Label>(m, "Label", "Represents a text label.")
        .def(py::init<Message>(), py::arg("text") = "")
        .def_property("text", &Label::getText, &Label::setText, "Gets or sets the text of the label.",
                      py::return_value_policy::reference);

    py::class_<Slider>(m, "Slider", "Represents a slider with a label.")
        .def(py::init<Message, float, float, float, std::optional<float>>(), py::arg("label") = "", py::arg("min") = 0,
             py::arg("max") = 100, py::arg("step") = 20, py::arg("default_value") = py::none())
        .def_property("label", &Slider::getLabel, &Slider::setLabel, "Gets or sets the label of the slider.")
        .def_property("min", &Slider::getMin, &Slider::setMin, "Gets or sets the minimum value of the slider.")
        .def_property("max", &Slider::getMax, &Slider::setMax, "Gets or sets the maximum value of the slider.")
        .def_property("step", &Slider::getStep, &Slider::setStep, "Gets or sets the step size of the slider.")
        .def_property("default_value", &Slider::getDefaultValue, &Slider::setDefaultValue,
                      "Gets or sets the optional default value of the slider.");

    py::class_<StepSlider>(m, "StepSlider", "Represents a step slider with a set of predefined options.")
        .def(py::init<>([](Message label, const std::optional<std::vector<std::string>> &options,
                           std::optional<int> default_index) {
                 return StepSlider(std::move(label), options.value_or(std::vector<std::string>{}), default_index);
             }),
             py::arg("label") = "", py::arg("options") = py::none(), py::arg("default_index") = py::none())
        .def_property("label", &StepSlider::getLabel, &StepSlider::setLabel,
                      "Gets or sets the label of the step slider.")
        .def_property("options", &StepSlider::getOptions, &StepSlider::setOptions,
                      "Gets or sets the options of the step slider.")
        .def_property("default_index", &StepSlider::getDefaultIndex, &StepSlider::setDefaultIndex,
                      "Gets or sets the optional default index of the step slider.")
        .def("add_option", &StepSlider::addOption, "Adds a new option to the step slider.", py::arg("option"));

    py::class_<TextInput>(m, "TextInput", "Represents a text input field.")
        .def(py::init<Message, Message, std::optional<std::string>>(), py::arg("label") = "",
             py::arg("placeholder") = "", py::arg("default_value") = py::none())
        .def_property("label", &TextInput::getLabel, &TextInput::setLabel,
                      "Gets or sets the label of the text input field.")
        .def_property("placeholder", &TextInput::getPlaceholder, &TextInput::setPlaceholder,
                      "Gets or sets the placeholder of the text input field.")
        .def_property("default_value", &TextInput::getDefaultValue, &TextInput::setDefaultValue,
                      "Gets or sets the optional default text of the text input field.");

    py::class_<Toggle>(m, "Toggle", "Represents a toggle button with a label.")
        .def(py::init<Message, bool>(), py::arg("label") = "", py::arg("default_value") = false)
        .def_property("label", &Toggle::getLabel, &Toggle::setLabel, "Gets or sets the label of the toggle.",
                      py::return_value_policy::reference)
        .def_property("default_value", &Toggle::getDefaultValue, &Toggle::setDefaultValue,
                      "Gets or sets the value of the toggle.", py::return_value_policy::reference);

    py::class_<MessageForm>(m, "MessageForm", "Represents a form with two buttons.")
        .def(py::init<>([](Message title, Message content, Message button1, Message button2,
                           MessageForm::OnSubmitCallback on_submit, MessageForm::OnCloseCallback on_close) {
                 return MessageForm()
                     .setTitle(std::move(title))
                     .setContent(std::move(content))
                     .setButton1(std::move(button1))
                     .setButton2(std::move(button2))
                     .setOnSubmit(std::move(on_submit))
                     .setOnClose(std::move(on_close));
             }),
             py::arg("title") = "", py::arg("content") = "", py::arg("button1") = "", py::arg("button2") = "",
             py::arg("on_submit") = MessageForm::OnSubmitCallback{},
             py::arg("on_close") = MessageForm::OnCloseCallback{})
        .def_property("title", &MessageForm::getTitle, &MessageForm::setTitle, "Gets or sets the title of the form.",
                      py::return_value_policy::reference)
        .def_property("on_submit", &MessageForm::getOnSubmit, &MessageForm::setOnSubmit,
                      "Gets or sets the on submit callback.", py::return_value_policy::reference)
        .def_property("on_close", &MessageForm::getOnClose, &MessageForm::setOnClose,
                      "Gets or sets the on close callback.", py::return_value_policy::reference)
        .def_property("content", &MessageForm::getContent, &MessageForm::setContent,
                      "Gets or sets the content of the form.", py::return_value_policy::reference)
        .def_property("button1", &MessageForm::getButton1, &MessageForm::setButton1,
                      "Gets or sets the text of button1.", py::return_value_policy::reference)
        .def_property("button2", &MessageForm::getButton2, &MessageForm::setButton2,
                      "Gets or sets the text of button2.", py::return_value_policy::reference);

    auto action_form =
        py::class_<ActionForm>(m, "ActionForm", "Represents a form with buttons that let the player take action.");

    py::class_<ActionForm::Button>(action_form, "Button", "Represents a button with text and an optional icon.")
        .def(py::init<Message, std::optional<std::string>, ActionForm::Button::OnClickCallback>(), py::arg("text") = "",
             py::arg("icon") = py::none(), py::arg("on_click") = ActionForm::Button::OnClickCallback{})
        .def_property("text", &ActionForm::Button::getText, &ActionForm::Button::setText,
                      "Gets or sets the text of the button", py::return_value_policy::reference)
        .def_property("icon", &ActionForm::Button::getIcon, &ActionForm::Button::setIcon,
                      "Gets or sets the icon path or URL of the button", py::return_value_policy::reference)
        .def_property("on_click", &ActionForm::Button::getOnClick, &ActionForm::Button::setOnClick,
                      "Gets or sets the on click callback.", py::return_value_policy::reference);

    action_form
        .def(
            py::init<>([](Message title, Message content, const std::optional<std::vector<ActionForm::Button>> &buttons,
                          ActionForm::OnSubmitCallback on_submit, ActionForm::OnCloseCallback on_close) {
                return ActionForm()
                    .setTitle(std::move(title))
                    .setContent(std::move(content))
                    .setButtons(buttons.value_or(std::vector<ActionForm::Button>{}))
                    .setOnSubmit(std::move(on_submit))
                    .setOnClose(std::move(on_close));
            }),
            py::arg("title") = "", py::arg("content") = "", py::arg("buttons") = py::none(),
            py::arg("on_submit") = ActionForm::OnSubmitCallback{}, py::arg("on_close") = ActionForm::OnCloseCallback{})
        .def_property("title", &ActionForm::getTitle, &ActionForm::setTitle, "Gets or sets the title of the form.",
                      py::return_value_policy::reference)
        .def_property("on_submit", &ActionForm::getOnSubmit, &ActionForm::setOnSubmit,
                      "Gets or sets the on submit callback.", py::return_value_policy::reference)
        .def_property("on_close", &ActionForm::getOnClose, &ActionForm::setOnClose,
                      "Gets or sets the on close callback.", py::return_value_policy::reference)
        .def_property("content", &ActionForm::getContent, &ActionForm::setContent,
                      "Gets or sets the content of the form.", py::return_value_policy::reference)
        .def("add_button", &ActionForm::addButton, "Adds a button to the form.", py::arg("text"),
             py::arg("icon") = py::none(), py::arg("on_click") = ActionForm::Button::OnClickCallback{},
             py::return_value_policy::reference)
        .def_property("buttons", &ActionForm::getButtons, &ActionForm::setButtons,
                      "Gets or sets the buttons of the action form.", py::return_value_policy::reference);

    py::class_<ModalForm>(m, "ModalForm", "Represents a modal form with controls.")
        .def(py::init<>([](Message title, const std::optional<std::vector<ModalForm::Control>> &controls,
                           std::optional<Message> submit_button, std::optional<std::string> icon,
                           ModalForm::OnSubmitCallback on_submit, ModalForm::OnCloseCallback on_close) {
                 return ModalForm()
                     .setTitle(std::move(title))
                     .setControls(controls.value_or(std::vector<ModalForm::Control>{}))
                     .setSubmitButton(std::move(submit_button))
                     .setIcon(std::move(icon))
                     .setOnSubmit(std::move(on_submit))
                     .setOnClose(std::move(on_close));
             }),
             py::arg("title") = "", py::arg("controls") = py::none(), py::arg("submit_button") = py::none(),
             py::arg("icon") = py::none(), py::arg("on_submit") = ModalForm::OnSubmitCallback{},
             py::arg("on_close") = ModalForm::OnCloseCallback{})
        .def("add_control", &ModalForm::addControl, "Adds a control to the form.", py::arg("control"))
        .def_property("title", &ModalForm::getTitle, &ModalForm::setTitle, "Gets or sets the title of the form.",
                      py::return_value_policy::reference)
        .def_property("on_submit", &ModalForm::getOnSubmit, &ModalForm::setOnSubmit,
                      "Gets or sets the on submit callback.", py::return_value_policy::reference)
        .def_property("on_close", &ModalForm::getOnClose, &ModalForm::setOnClose, "Gets or sets the on close callback.",
                      py::return_value_policy::reference)
        .def_property("controls", &ModalForm::getControls, &ModalForm::setControls,
                      "Gets or sets the controls of the modal form.", py::return_value_policy::reference)
        .def_property("icon", &ModalForm::getIcon, &ModalForm::setIcon, "Gets or sets the icon of the form.",
                      py::return_value_policy::reference)
        .def_property("submit_button", &ModalForm::getSubmitButton, &ModalForm::setSubmitButton,
                      "Gets or sets the submit button message of the form.", py::return_value_policy::reference);
}

}  // namespace endstone::detail
