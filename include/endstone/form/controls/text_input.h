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

#include "endstone/message.h"

namespace endstone {

/**
 * @brief Represents a text input field.
 */
class TextInput {
public:
    TextInput() = default;
    explicit TextInput(Message label, Message placeholder, std::optional<std::string> default_text = std::nullopt)
        : label_(std::move(label)), placeholder_(std::move(placeholder)), default_text_(std::move(default_text))
    {
    }

    /**
     * @brief Gets the label of the text input field.
     *
     * @return The label of the text input field.
     */
    [[nodiscard]] Message getLabel() const
    {
        return label_;
    }

    /**
     * @brief Sets the label of the text input field.
     *
     * @param label The new label for the text input field.
     * @return A reference to the text input field itself.
     */
    TextInput &setLabel(Message label)
    {
        label_ = std::move(label);
        return *this;
    }

    /**
     * @brief Gets the placeholder of the text input field.
     *
     * @return The placeholder of the text input field.
     */
    [[nodiscard]] Message getPlaceholder() const
    {
        return placeholder_;
    }

    /**
     * @brief Sets the placeholder of the text input field.
     *
     * @param placeholder The new placeholder for the text input field.
     * @return A reference to the text input field itself.
     */
    TextInput &setPlaceholder(Message placeholder)
    {
        placeholder_ = std::move(placeholder);
        return *this;
    }

    /**
     * @brief Gets the default text of the text input field.
     *
     * @return The default text of the text input field.
     */
    [[nodiscard]] std::optional<std::string> getDefaultValue() const
    {
        return default_text_;
    }

    /**
     * @brief Sets the default text of the text input field.
     *
     * @param text The new default text for the text input field.
     * @return A reference to the text input field itself.
     */
    TextInput &setDefaultValue(std::optional<std::string> text)
    {
        default_text_ = std::move(text);
        return *this;
    }

private:
    Message label_;
    Message placeholder_;
    std::optional<std::string> default_text_;
};

}  // namespace endstone
