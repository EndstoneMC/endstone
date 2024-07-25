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
 * @brief Represents a slider with a label.
 */
class Slider {
public:
    Slider() = default;
    explicit Slider(Message label, float min, float max, float step, std::optional<float> default_value = std::nullopt)
        : label_(std::move(label)), min_(min), max_(max), step_(step), default_value_(default_value)
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
    Slider &setLabel(Message label)
    {
        label_ = std::move(label);
        return *this;
    }

    /**
     * @brief Gets the minimum value of the slider.
     *
     * @return The minimum value of the slider.
     */
    [[nodiscard]] float getMin() const
    {
        return min_;
    }

    /**
     * @brief Sets the minimum value of the slider.
     *
     * @param min The new minimum value for the slider.
     * @return A reference to the current slider, for function chaining.
     */
    Slider &setMin(float min)
    {
        min_ = min;
        return *this;
    }

    /**
     * @brief Gets the maximum value of the slider.
     *
     * @return The maximum value of the slider.
     */
    [[nodiscard]] float getMax() const
    {
        return max_;
    }

    /**
     * @brief Sets the maximum value of the slider.
     *
     * @param max The new maximum value for the slider.
     * @return A reference to the current slider, for function chaining.
     */
    Slider &setMax(float max)
    {
        max_ = max;
        return *this;
    }

    /**
     * @brief Gets the step size of the slider.
     *
     * @return The step size of the slider.
     */
    [[nodiscard]] float getStep() const
    {
        return step_;
    }

    /**
     * @brief Sets the step size of the slider.
     *
     * @param step The new step size for the slider.
     * @return A reference to the current slider, for function chaining.
     */
    Slider &setStep(float step)
    {
        step_ = step;
        return *this;
    }

    /**
     * @brief Gets the default value of the slider.
     *
     * @return The default value of the slider.
     */
    [[nodiscard]] std::optional<float> getDefaultValue() const
    {
        return default_value_;
    }

    /**
     * @brief Sets the default value of the slider.
     *
     * @param default_value The new default value for the slider.
     * @return A reference to the current slider, for function chaining.
     */
    Slider &setDefaultValue(std::optional<float> default_value)
    {
        default_value_ = default_value;
        return *this;
    }

private:
    Message label_;
    float min_;
    float max_;
    float step_;
    std::optional<float> default_value_;
};

}  // namespace endstone
