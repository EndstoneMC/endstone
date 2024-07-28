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

#include "endstone/form/form.h"

namespace endstone {

/**
 * @brief Represents a form with buttons that let the player take action.
 */
class ActionForm : public Form<ActionForm> {
public:
    /**
     * @brief Represents a button with text and an optional icon.
     */
    class Button {
    public:
        using OnClickCallback = std::function<void(Player *)>;

        Button() = default;
        explicit Button(Message text, std::optional<std::string> icon = std::nullopt,
                        ActionForm::Button::OnClickCallback on_click = {})
            : text_(std::move(text)), icon_(std::move(icon)), on_click_(std::move(on_click))
        {
        }

        /**
         * @brief Gets the text of the button.
         *
         * @return The text.
         */
        [[nodiscard]] Message getText() const
        {
            return text_;
        }

        /**
         * @brief Sets the text of the button.
         *
         * @param text The new text for the button.
         * @return A reference to the current button.
         */
        Button &setText(Message text)
        {
            text_ = std::move(text);
            return *this;
        }

        /**
         * @brief Get the icon of the button.
         *
         * @return The path or URL to the icon file
         */
        [[nodiscard]] std::optional<std::string> getIcon() const
        {
            return icon_;
        }

        /**
         * @brief Sets the icon for the button.
         *
         * @param icon The path or URL to the icon file.
         * @return A reference to the current button.
         */
        Button &setIcon(std::string icon)
        {
            icon_ = std::move(icon);
            return *this;
        }

        /**
         * @brief Gets the on submit callback of the button.
         *
         * @return The on submit callback of the button.
         */
        [[nodiscard]] OnClickCallback getOnClick() const
        {
            return on_click_;
        }

        /**
         * @brief Sets the on click callback of the button.
         *
         * @param on_click The callback to be set.
         * @return A reference to the current button.
         */
        Button &setOnClick(OnClickCallback on_click)
        {
            on_click_ = std::move(on_click);
            return *this;
        }

    private:
        Message text_;
        std::optional<std::string> icon_;
        OnClickCallback on_click_;
    };

    using OnSubmitCallback = std::function<void(Player *, int)>;

    explicit ActionForm()
    {
        on_submit_ = [this](Player *player, int selection) {
            if (selection < 0 || selection >= buttons_.size()) {
                return;
            }
            auto &button = buttons_[selection];
            auto on_click = button.getOnClick();
            if (on_click) {
                on_click(player);
            }
        };
    }

    /**
     * @brief Get the content of the form.
     *
     * @return The content of the form.
     */
    [[nodiscard]] Message getContent() const
    {
        return content_;
    }

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
     * @return A reference to the current form.
     */
    ActionForm &addButton(const Message &text, const std::optional<std::string> &icon = std::nullopt)
    {
        buttons_.emplace_back(text, icon);
        return *this;
    }

    /**
     * @brief Get the buttons of the action form.
     *
     * @return A list of buttons in the action form.
     */
    [[nodiscard]] const std::vector<Button> &getButtons() const
    {
        return buttons_;
    }

    /**
     * @brief Set the buttons of the action form.
     *
     * @param buttons The list of buttons to set.
     * @return A reference to the current form.
     */
    ActionForm &setButtons(const std::vector<Button> &buttons)
    {
        buttons_ = buttons;
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
    ActionForm &setOnSubmit(OnSubmitCallback on_submit)
    {
        on_submit_ = std::move(on_submit);
        return *this;
    }

private:
    Message content_;
    std::vector<Button> buttons_;
    OnSubmitCallback on_submit_;
};

}  // namespace endstone
