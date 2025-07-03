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

#include "endstone/debugshape/debug_shape.h"

namespace endstone {

/**
 * @brief Represents a debug text.
 */
class DebugText : public DebugShape<DebugText> {
public:
    /**
     * @brief Gets the text of the debug text.
     *
     * @return The the text of the debug text.
     */
    [[nodiscard]] std::string getText() const
    {
        return text_;
    }

    /**
     * @brief Sets the the text of the debug text.
     *
     * @param text The desired the text of the debug text.
     * @return A reference to the current debug shape.
     */
    DebugText &setText(const std::string &text)
    {
        text_ = text;
        return *this;
    }

private:
    std::string text_;
};

}  // namespace endstone