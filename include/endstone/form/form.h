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

#include <string>
#include <utility>
#include <variant>

#include "endstone/translatable.h"

namespace endstone {

/**
 * @brief Represents a generic form.
 *
 * The Form class provides a base abstraction for different types of forms.
 * It supports three types of forms: Action, Message, and Modal.
 * Each form has a title, which can be a string or a translatable object.
 */
template <typename T>
class Form {
public:
    using TextType = std::variant<std::string, Translatable>;

    explicit Form(TextType title) : title_(std::move(title)) {}

    /**
     * @brief Gets the title of the form.
     *
     * @return The title of the form.
     */
    [[nodiscard]] TextType getTitle() const
    {
        return title_;
    }

    /**
     * @brief Sets the title of the form.
     *
     * @param title The title of the form to be set.
     * @return A reference to the current form.
     */
    T &setTitle(TextType title)
    {
        title_ = std::move(title);
        return *static_cast<T *>(this);
    }

protected:
    TextType title_;
};

}  // namespace endstone
