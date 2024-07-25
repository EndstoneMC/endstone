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

#include "endstone/message.h"

namespace endstone {

class Player;

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
    using OnCloseCallback = std::function<void(Player *)>;
    explicit Form() = default;

    /**
     * @brief Gets the title of the form.
     *
     * @return The title of the form.
     */
    [[nodiscard]] Message getTitle() const
    {
        return title_;
    }

    /**
     * @brief Sets the title of the form.
     *
     * @param title The title of the form to be set.
     * @return A reference to the current form.
     */
    T &setTitle(Message title)
    {
        title_ = std::move(title);
        return *static_cast<T *>(this);
    }

    /**
     * @brief Sets the on close callback of the form.
     *
     * @param on_close The callback to be set.
     * @return A reference to the current form.
     */
    T &setOnClose(OnCloseCallback on_close)
    {
        on_close_ = std::move(on_close);
        return *static_cast<T *>(this);
    }

    /**
     * @brief Gets the on close callback of the form.
     *
     * @return The on close callback of the form.
     */
    [[nodiscard]] OnCloseCallback getOnClose() const
    {
        return on_close_;
    }

protected:
    Message title_;
    OnCloseCallback on_close_;
};

}  // namespace endstone
