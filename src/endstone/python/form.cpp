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

void init_form(py::module_ &m)
{
    py::class_<Dropdown>(m, "Dropdown", "Represents a dropdown with a set of predefined options.")
        .def(py::init<>([](Message label, const std::optional<std::vector<std::string>> &options,
                           std::optional<int> default_index) {
                 return Dropdown(std::move(label), options.value_or(std::vector<std::string>{}), default_index);
             }),
             py::arg("label") = "", py::arg("options") = py::none(), py::arg("default_index") = py::none())
        .def_property("label", &Dropdown::getLabel, &Dropdown::setLabel, "The label of the dropdown.")
        .def_property("options", &Dropdown::getOptions, &Dropdown::setOptions, "The options of the dropdown.")
        .def_property("default_index", &Dropdown::getDefaultIndex, &Dropdown::setDefaultIndex,
                      "The optional default index of the dropdown.")
        .def("add_option", &Dropdown::addOption, py::arg("option"), "Adds a new option to the dropdown.");

    py::class_<Label>(m, "Label", "Represents a text label.")
        .def(py::init<Message>(), py::arg("text") = "")
        .def_property("text", &Label::getText, &Label::setText, "The text of the label.",
                      py::return_value_policy::reference);

    py::class_<Slider>(m, "Slider", "Represents a slider with a label.")
        .def(py::init<Message, float, float, float, std::optional<float>>(), py::arg("label") = "", py::arg("min") = 0,
             py::arg("max") = 100, py::arg("step") = 20, py::arg("default_value") = py::none())
        .def_property("label", &Slider::getLabel, &Slider::setLabel, "The label of the slider.")
        .def_property("min", &Slider::getMin, &Slider::setMin, "The minimum value of the slider.")
        .def_property("max", &Slider::getMax, &Slider::setMax, "The maximum value of the slider.")
        .def_property("step", &Slider::getStep, &Slider::setStep, "The step size of the slider.")
        .def_property("default_value", &Slider::getDefaultValue, &Slider::setDefaultValue,
                      "The optional default value of the slider.");

    py::class_<StepSlider>(m, "StepSlider", "Represents a step slider with a set of predefined options.")
        .def(py::init<>([](Message label, const std::optional<std::vector<std::string>> &options,
                           std::optional<int> default_index) {
                 return StepSlider(std::move(label), options.value_or(std::vector<std::string>{}), default_index);
             }),
             py::arg("label") = "", py::arg("options") = py::none(), py::arg("default_index") = py::none())
        .def_property("label", &StepSlider::getLabel, &StepSlider::setLabel, "The label of the step slider.")
        .def_property("options", &StepSlider::getOptions, &StepSlider::setOptions, "The options of the step slider.")
        .def_property("default_index", &StepSlider::getDefaultIndex, &StepSlider::setDefaultIndex,
                      "The optional default index of the step slider.")
        .def("add_option", &StepSlider::addOption, py::arg("option"), "Adds a new option to the step slider.");

    py::class_<TextInput>(m, "TextInput", "Represents a text input field.")
        .def(py::init<Message, Message, std::optional<std::string>>(), py::arg("label") = "",
             py::arg("placeholder") = "", py::arg("default_value") = py::none())
        .def_property("label", &TextInput::getLabel, &TextInput::setLabel, "The label of the text input field.")
        .def_property("placeholder", &TextInput::getPlaceholder, &TextInput::setPlaceholder,
                      "The placeholder of the text input field.")
        .def_property("default_value", &TextInput::getDefaultValue, &TextInput::setDefaultValue,
                      "The optional default text of the text input field.");

    py::class_<Toggle>(m, "Toggle", "Represents a toggle button with a label.")
        .def(py::init<Message, bool>(), py::arg("label") = "", py::arg("default_value") = false)
        .def_property("label", &Toggle::getLabel, &Toggle::setLabel, "The label of the toggle.",
                      py::return_value_policy::reference)
        .def_property("default_value", &Toggle::getDefaultValue, &Toggle::setDefaultValue,
                      "The default value of the toggle.", py::return_value_policy::reference);

    py::class_<Divider>(m, "Divider", "Represents a divider.").def(py::init<>());

    py::class_<Header>(m, "Header", "Represents a header with a label.")
        .def(py::init<Message>(), py::arg("label") = "")
        .def_property("label", &Header::getLabel, &Header::setLabel, "The label of the header.",
                      py::return_value_policy::reference);

    py::class_<Button>(m, "Button", "Represents a button with text and an optional icon.")
        .def(py::init<Message, std::optional<std::string>, Button::OnClickCallback>(), py::arg("text") = "",
             py::arg("icon") = py::none(), py::arg("on_click") = Button::OnClickCallback{})
        .def_property("text", &Button::getText, &Button::setText, "The text of the button.",
                      py::return_value_policy::reference)
        .def_property("icon", &Button::getIcon, &Button::setIcon, "The path or URL to the icon file.",
                      py::return_value_policy::reference)
        .def_property("on_click", &Button::getOnClick, &Button::setOnClick, "The on click callback of the button.",
                      py::return_value_policy::reference);

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
        .def_property("title", &MessageForm::getTitle, &MessageForm::setTitle, "The title of the form.",
                      py::return_value_policy::reference)
        .def_property("on_submit", &MessageForm::getOnSubmit, &MessageForm::setOnSubmit,
                      "The on submit callback of the form.", py::return_value_policy::reference)
        .def_property("on_close", &MessageForm::getOnClose, &MessageForm::setOnClose,
                      "The on close callback of the form.", py::return_value_policy::reference)
        .def_property("content", &MessageForm::getContent, &MessageForm::setContent, "The content of the form.",
                      py::return_value_policy::reference)
        .def_property("button1", &MessageForm::getButton1, &MessageForm::setButton1, "The text of button1.",
                      py::return_value_policy::reference)
        .def_property("button2", &MessageForm::getButton2, &MessageForm::setButton2, "The text of button2.",
                      py::return_value_policy::reference);

    auto action_form =
        py::class_<ActionForm>(m, "ActionForm", "Represents a form with buttons that let the player take action.")
            .def(py::init<>([](Message title, Message content,
                               const std::optional<std::vector<ActionForm::Control>> &controls,
                               ActionForm::OnSubmitCallback on_submit, ActionForm::OnCloseCallback on_close) {
                     return ActionForm()
                         .setTitle(std::move(title))
                         .setContent(std::move(content))
                         .setControls(controls.value_or(std::vector<ActionForm::Control>{}))
                         .setOnSubmit(std::move(on_submit))
                         .setOnClose(std::move(on_close));
                 }),
                 py::arg("title") = "", py::arg("content") = "", py::arg("buttons") = py::none(),
                 py::arg("on_submit") = ActionForm::OnSubmitCallback{},
                 py::arg("on_close") = ActionForm::OnCloseCallback{})
            .def_property("title", &ActionForm::getTitle, &ActionForm::setTitle, "The title of the form.",
                          py::return_value_policy::reference)
            .def_property("on_submit", &ActionForm::getOnSubmit, &ActionForm::setOnSubmit,
                          "The on submit callback of the form.", py::return_value_policy::reference)
            .def_property("on_close", &ActionForm::getOnClose, &ActionForm::setOnClose,
                          "The on close callback of the form.", py::return_value_policy::reference)
            .def_property("content", &ActionForm::getContent, &ActionForm::setContent, "The content of the form.",
                          py::return_value_policy::reference)
            .def("add_button", &ActionForm::addButton, py::arg("text"), py::arg("icon") = py::none(),
                 py::arg("on_click") = Button::OnClickCallback{}, py::return_value_policy::reference, R"doc(
    Adds a button to the form.

    Args:
        text: The text of the button.
        icon: The path or URL to the icon image.
        on_click: The on click callback of the button.

    Returns:
        A reference to the current form.
)doc")
            .def("add_label", &ActionForm::addLabel, py::arg("text"), py::return_value_policy::reference, R"doc(
    Adds a label to the form.

    Args:
        text: The text of the label.

    Returns:
        A reference to the current form.
)doc")
            .def("add_header", &ActionForm::addHeader, py::arg("text"), py::return_value_policy::reference, R"doc(
    Adds a header to the form.

    Args:
        text: The text of the header.

    Returns:
        A reference to the current form.
)doc")
            .def("add_divider", &ActionForm::addDivider, py::return_value_policy::reference, R"doc(
    Adds a divider to the form.

    Returns:
        A reference to the current form.
)doc")
            .def_property("controls", &ActionForm::getControls, &ActionForm::setControls,
                          "The controls of the action form.", py::return_value_policy::reference);

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
        .def("add_control", &ModalForm::addControl, py::arg("control"), "Adds a control to the form.")
        .def_property("title", &ModalForm::getTitle, &ModalForm::setTitle, "The title of the form.",
                      py::return_value_policy::reference)
        .def_property("on_submit", &ModalForm::getOnSubmit, &ModalForm::setOnSubmit,
                      "The on submit callback of the form.", py::return_value_policy::reference)
        .def_property("on_close", &ModalForm::getOnClose, &ModalForm::setOnClose, "The on close callback of the form.",
                      py::return_value_policy::reference)
        .def_property("controls", &ModalForm::getControls, &ModalForm::setControls, "The controls of the modal form.",
                      py::return_value_policy::reference)
        .def_property("icon", &ModalForm::getIcon, &ModalForm::setIcon, "The path or URL to the icon file.",
                      py::return_value_policy::reference)
        .def_property("submit_button", &ModalForm::getSubmitButton, &ModalForm::setSubmitButton,
                      "The submit button text of the form.", py::return_value_policy::reference);
}

}  // namespace endstone::python
