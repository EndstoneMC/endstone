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
#include <vector>

#include "endstone/message.h"

namespace endstone {

/**
 * @brief Represents a dropdown with a set of predefined options.
 */
class Dropdown {
public:
    Dropdown() = default;
    explicit Dropdown(Message label, std::vector<std::string> options, std::optional<int> default_index = std::nullopt)
        : label_(std::move(label)), options_(std::move(options)), default_index_(default_index)
    {
    }

    /**
     * @brief Gets the label of the dropdown.
     *
     * @return The label of the dropdown.
     */
    [[nodiscard]] Message getLabel() const
    {
        return label_;
    }

    /**
     * @brief Sets the label of the dropdown.
     *
     * @param label The new label for the dropdown.
     * @return A reference to the dropdown itself.
     */
    Dropdown &setLabel(Message label)
    {
        label_ = std::move(label);
        return *this;
    }

    /**
     * @brief Adds a new option to the dropdown.
     *
     * @param option The new option for the dropdown.
     * @return A reference to the dropdown itself.
     */
    Dropdown &addOption(const std::string &option)
    {
        options_.push_back(option);
        return *this;
    }

    /**
     * @brief Gets the options of the dropdown.
     *
     * @return The options of the dropdown.
     */
    [[nodiscard]] std::vector<std::string> getOptions() const
    {
        return options_;
    }

    /**
     * @brief Sets the options of the dropdown.
     *
     * @param options The new options for the dropdown.
     * @return A reference to the dropdown itself.
     */
    Dropdown &setOptions(std::vector<std::string> options)
    {
        options_ = std::move(options);
        return *this;
    }

    /**
     * @brief Gets the default index of the dropdown.
     *
     * @return The default index of the dropdown.
     */
    [[nodiscard]] std::optional<int> getDefaultIndex() const
    {
        return default_index_;
    }

    /**
     * @brief Sets the default index of the dropdown.
     *
     * @param default_index The new default index for the dropdown.
     * @return A reference to the dropdown itself.
     */
    Dropdown &setDefaultIndex(std::optional<int> default_index)
    {
        default_index_ = default_index;
        return *this;
    }

private:
    Message label_;
    std::vector<std::string> options_;
    std::optional<int> default_index_;
};

}  // namespace endstone
