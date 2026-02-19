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

#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "endstone/form/controls/button.h"
#include "endstone/form/controls/divider.h"
#include "endstone/form/controls/header.h"
#include "endstone/form/controls/label.h"
#include "endstone/form/form.h"

namespace endstone {

/**
 * @brief Represents a form with buttons that let the player take action.
 */
class ActionForm : public Form<ActionForm> {
public:
    using Control = std::variant<Button, Divider, Header, Label>;
    using OnSubmitCallback = std::function<void(Player *, int)>;

    explicit ActionForm() = default;

    /**
     * @brief Get the content of the form.
     *
     * @return The content of the form.
     */
    [[nodiscard]] Message getContent() const { return content_; }

    /**
     * @brief Set the content of the form.
     *
     * @param text The text to set as the content.
     * @return A reference to the current form.
     */
    ActionForm &setContent(Message text)
    {
        content_ = std::move(text);
        return *this;
    }

    /**
     * @brief Adds a button to the form.
     *
     * @param text The text of the button
     * @param icon The path or URL to the icon image
     * @param on_click The on click callback of the button
     * @return A reference to the current form.
     */
    ActionForm &addButton(const Message &text, const std::optional<std::string> &icon = std::nullopt,
                          Button::OnClickCallback on_click = {})
    {
        auto button = Button(text, icon, std::move(on_click));
        controls_.emplace_back(button);
        return *this;
    }

    /**
     * @brief Adds a label to the form.
     *
     * @param text The text of the label
     * @return A reference to the current form.
     */
    ActionForm &addLabel(const Message &text)
    {
        controls_.push_back(Label(text));
        return *this;
    }

    /**
     * @brief Adds a header to the form.
     *
     * @param text The text of the header
     * @return A reference to the current form.
     */
    ActionForm &addHeader(const Message &text)
    {
        controls_.push_back(Header(text));
        return *this;
    }

    /**
     * @brief Adds a divider to the form.
     *
     * @return A reference to the current form.
     */
    ActionForm &addDivider()
    {
        controls_.push_back(Divider());
        return *this;
    }

    /**
     * @brief Get the controls of the action form.
     *
     * @return A list of controls in the action form.
     */
    [[nodiscard]] const std::vector<Control> &getControls() const { return controls_; }

    /**
     * @brief Set the controls of the action form.
     *
     * @param controls The list of controls to set.
     * @return A reference to the current form.
     */
    ActionForm &setControls(const std::vector<Control> &controls)
    {
        controls_ = controls;
        return *this;
    }

    /**
     * @brief Gets the on submit callback of the form.
     *
     * @return The on submit callback of the form.
     */
    [[nodiscard]] OnSubmitCallback getOnSubmit() const { return on_submit_; }

    /**
     * @brief Sets the on submit callback of the form.
     *
     * @param on_submit The callback to be set.
     * @return A reference to the current form.
     */
    ActionForm &setOnSubmit(OnSubmitCallback on_submit)
    {
        on_submit_ = std::move(on_submit);
        return *this;
    }

private:
    Message content_;
    std::vector<Control> controls_;
    OnSubmitCallback on_submit_;
};

}  // namespace endstone
