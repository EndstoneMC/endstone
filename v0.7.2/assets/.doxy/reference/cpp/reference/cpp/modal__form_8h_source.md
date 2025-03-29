

# File modal\_form.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**form**](dir_0fd3b458603af3963ebb9c312a9238ec.md) **>** [**modal\_form.h**](modal__form_8h.md)

[Go to the documentation of this file](modal__form_8h.md)


```C++
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

class ModalForm : public Form<ModalForm> {
public:
    using Control = std::variant<Dropdown, Label, Slider, StepSlider, TextInput, Toggle>;
    using OnSubmitCallback = std::function<void(Player *, std::string)>;

    ModalForm &addControl(const Control &control)
    {
        controls_.push_back(control);
        return *this;
    }

    [[nodiscard]] std::vector<Control> getControls() const
    {
        return controls_;
    }

    ModalForm &setControls(std::vector<Control> controls)
    {
        controls_ = std::move(controls);
        return *this;
    }

    [[nodiscard]] std::optional<Message> getSubmitButton() const
    {
        return submit_button_text_;
    }

    ModalForm &setSubmitButton(std::optional<Message> text)
    {
        submit_button_text_ = std::move(text);
        return *this;
    }

    [[nodiscard]] std::optional<std::string> getIcon() const
    {
        return icon_;
    }

    ModalForm &setIcon(std::optional<std::string> icon)
    {
        icon_ = std::move(icon);
        return *this;
    }

    [[nodiscard]] OnSubmitCallback getOnSubmit() const
    {
        return on_submit_;
    }

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
```


