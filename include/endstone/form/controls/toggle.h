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
 * @brief Represents a toggle button with a label.
 */
class Toggle {
public:
    Toggle() = default;
    explicit Toggle(Message label, bool default_value = false) : label_(std::move(label)), default_value_(default_value)
    {
    }

    /**
     * @brief Gets the label of the toggle.
     *
     * @return The label of the toggle.
     */
    [[nodiscard]] Message getLabel() const
    {
        return label_;
    }

    /**
     * @brief Sets the label of the toggle.
     *
     * @param label The new label for the toggle.
     * @return A reference to the current toggle.
     */
    Toggle &setLabel(Message label)
    {
        label_ = std::move(label);
        return *this;
    }

    /**
     * @brief Gets the default value of the toggle.
     *
     * @return The default value of the toggle.
     */
    [[nodiscard]] bool getDefaultValue() const
    {
        return default_value_;
    }

    /**
     * @brief Sets the default value of the toggle.
     *
     * @param value The new default value for the toggle.
     * @return A reference to the current toggle.
     */
    Toggle &setDefaultValue(bool value)
    {
        default_value_ = value;
        return *this;
    }

private:
    Message label_;
    bool default_value_;
};

}  // namespace endstone
