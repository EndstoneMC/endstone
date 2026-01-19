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
#include <vector>

namespace endstone {

/**
 * @brief Represents an object with a text representation that can be translated by the Minecraft client.
 */
class Translatable {
public:
    explicit Translatable(std::string text, std::vector<std::string> params = {})
        : text_(std::move(text)), params_(std::move(params))
    {
    }

    /**
     * @brief Get the text to be translated.
     *
     * @return the text to be translated
     */
    [[nodiscard]] const std::string &getText() const { return text_; }

    /**
     * @brief Get the translation parameters.
     *
     * @return the translation parameters
     */
    [[nodiscard]] const std::vector<std::string> &getParameters() const { return params_; }

    /**
     * @brief Check if the message to be translated is empty
     *
     * @return true if the message to be translated is empty, false otherwise
     */
    [[nodiscard]] bool empty() const { return text_.empty(); }

private:
    std::string text_;
    std::vector<std::string> params_;
};

}  // namespace endstone
