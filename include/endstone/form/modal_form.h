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

#pragma once

#include <optional>
#include <string>
#include <utility>

#include "endstone/form/controls/dropdown.h"
#include "endstone/form/controls/label.h"
#include "endstone/form/controls/slider.h"
#include "endstone/form/controls/step_slider.h"
#include "endstone/form/controls/text_input.h"
#include "endstone/form/controls/toggle.h"
#include "endstone/form/form.h"

namespace endstone {

/**
 * @brief Represents a modal form with controls.
 */
class ModalForm : public Form<ModalForm> {
public:
    using Control = std::variant<Dropdown, Label, Slider, StepSlider, TextInput, Toggle>;
    using OnSubmitCallback = std::function<void(Player *, std::string)>;

    /**
     * @brief Adds a control to the form.
     *
     * @param control The control to add to the form.
     * @return A reference to the current form.
     */
    ModalForm &addControl(const Control &control)
    {
        controls_.push_back(control);
        return *this;
    }

    /**
     * @brief Gets the controls of the modal form.
     *
     * @return A list of controls in the modal form.
     */
    [[nodiscard]] std::vector<Control> getControls() const
    {
        return controls_;
    }

    /**
     * @brief Sets the controls of the modal form.
     *
     * @param controls The list of controls to set.
     * @return A reference to the current form.
     */
    ModalForm &setControls(std::vector<Control> controls)
    {
        controls_ = std::move(controls);
        return *this;
    }

    /**
     * @brief Gets the submit button text of the form.
     *
     * @return The submit button text of the form.
     */
    [[nodiscard]] std::optional<Message> getSubmitButton() const
    {
        return submit_button_text_;
    }

    /**
     * @brief Sets the submit button text of the form.
     *
     * @param text The submit button text to set.
     * @return A reference to the current form.
     */
    ModalForm &setSubmitButton(std::optional<Message> text)
    {
        submit_button_text_ = std::move(text);
        return *this;
    }

    /**
     * @brief Get the icon of the form.
     *
     * @return The path or URL to the icon file
     */
    [[nodiscard]] std::optional<std::string> getIcon() const
    {
        return icon_;
    }

    /**
     * @brief Sets the icon of the form.
     *
     * @param icon The path or URL to the icon file.
     * @return A reference to the current form.
     */
    ModalForm &setIcon(std::optional<std::string> icon)
    {
        icon_ = std::move(icon);
        return *this;
    }

    /**
     * @brief Gets the on submit callback of the form.
     *
     * @return The on submit callback of the form.
     */
    [[nodiscard]] OnSubmitCallback getOnSubmit() const
    {
        return on_submit_;
    }

    /**
     * @brief Sets the on submit callback of the form.
     *
     * @param on_submit The callback to be set.
     * @return A reference to the current form.
     */
    ModalForm &setOnSubmit(OnSubmitCallback on_submit)
    {
        on_submit_ = std::move(on_submit);
        return *this;
    }

private:
    std::vector<Control> controls_;
    std::optional<Message> submit_button_text_;
    std::optional<std::string> icon_;
    OnSubmitCallback on_submit_;
};

}  // namespace endstone
