

# File toggle.h

[**File List**](files.md) **>** [**controls**](dir_035306890ec6a3fa870e30b726ac5ffc.md) **>** [**toggle.h**](toggle_8h.md)

[Go to the documentation of this file](toggle_8h.md)


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

class Toggle {
public:
    Toggle() = default;
    explicit Toggle(Message label, bool default_value = false) : label_(std::move(label)), default_value_(default_value)
    {
    }

    [[nodiscard]] Message getLabel() const
    {
        return label_;
    }

    Toggle &setLabel(Message label)
    {
        label_ = std::move(label);
        return *this;
    }

    [[nodiscard]] bool getDefaultValue() const
    {
        return default_value_;
    }

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
```


