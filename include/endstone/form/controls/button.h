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

#include "endstone/message.h"

/**
 * @brief Represents a button with text and an optional icon.
 */
namespace endstone {

class Player;

/**
 * @brief Represents a button with text and an optional icon.
 */
class Button {
public:
    using OnClickCallback = std::function<void(Player *)>;

    Button() = default;
    explicit Button(Message text, std::optional<std::string> icon = std::nullopt, OnClickCallback on_click = {})
        : text_(std::move(text)), icon_(std::move(icon)), on_click_(std::move(on_click))
    {
    }

    /**
     * @brief Gets the text of the button.
     *
     * @return The text.
     */
    [[nodiscard]] Message getText() const { return text_; }

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
    [[nodiscard]] std::optional<std::string> getIcon() const { return icon_; }

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
     * @brief Gets the on click callback of the button.
     *
     * @return The on click callback of the button.
     */
    [[nodiscard]] OnClickCallback getOnClick() const { return on_click_; }

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

}  // namespace endstone
