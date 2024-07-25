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

#include <utility>

#include "endstone/form/form.h"

namespace endstone {

/**
 * @brief Represents a form with two buttons.
 */
class MessageForm : public Form<MessageForm> {
public:
    using OnSubmitCallback = std::function<void(Player *, int)>;

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
    MessageForm &setContent(Message text)
    {
        content_ = std::move(text);
        return *this;
    }

    /**
     * @brief Get the text of button1.
     *
     * @return The text of button1.
     */
    [[nodiscard]] Message getButton1() const
    {
        return button1_text_;
    }

    /**
     * @brief Set the text of button1.
     *
     * @param text The text to set as the button1 text.
     * @return A reference to the current form.
     */
    MessageForm &setButton1(Message text)
    {
        button1_text_ = std::move(text);
        return *this;
    }

    /**
     * @brief Get the text of button2.
     *
     * @return The text of button2.
     */
    [[nodiscard]] Message getButton2() const
    {
        return button2_text_;
    }

    /**
     * @brief Set the text of button2.
     *
     * @param text The text to set as the button2 text.
     * @return A reference to the current form.
     */
    MessageForm &setButton2(Message text)
    {
        button2_text_ = std::move(text);
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
    MessageForm &setOnSubmit(OnSubmitCallback on_submit)
    {
        on_submit_ = std::move(on_submit);
        return *this;
    }

private:
    Message content_;
    Message button1_text_;
    Message button2_text_;
    OnSubmitCallback on_submit_;
};

}  // namespace endstone
