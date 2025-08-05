

# File slider.h

[**File List**](files.md) **>** [**controls**](dir_035306890ec6a3fa870e30b726ac5ffc.md) **>** [**slider.h**](slider_8h.md)

[Go to the documentation of this file](slider_8h.md)


```C++
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

class Slider {
public:
    Slider() = default;
    explicit Slider(Message label, float min, float max, float step, std::optional<float> default_value = std::nullopt)
        : label_(std::move(label)), min_(min), max_(max), step_(step), default_value_(default_value)
    {
    }

    [[nodiscard]] Message getLabel() const
    {
        return label_;
    }

    Slider &setLabel(Message label)
    {
        label_ = std::move(label);
        return *this;
    }

    [[nodiscard]] float getMin() const
    {
        return min_;
    }

    Slider &setMin(float min)
    {
        min_ = min;
        return *this;
    }

    [[nodiscard]] float getMax() const
    {
        return max_;
    }

    Slider &setMax(float max)
    {
        max_ = max;
        return *this;
    }

    [[nodiscard]] float getStep() const
    {
        return step_;
    }

    Slider &setStep(float step)
    {
        step_ = step;
        return *this;
    }

    [[nodiscard]] std::optional<float> getDefaultValue() const
    {
        return default_value_;
    }

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
```


