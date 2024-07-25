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
 * @brief Represents a text label.
 */
class Label {
public:
    Label() = default;
    explicit Label(Message text) : text_(std::move(text)) {}

    /**
     * @brief Gets the text of the label.
     *
     * @return The text of the label.
     */
    [[nodiscard]] Message getText() const
    {
        return text_;
    }

    /**
     * @brief Sets the text of the label.
     *
     * @param text The new text for the label.
     * @return A reference to the current label.
     */
    Label &setText(Message text)
    {
        text_ = std::move(text);
        return *this;
    }

private:
    Message text_;
};

}  // namespace endstone
