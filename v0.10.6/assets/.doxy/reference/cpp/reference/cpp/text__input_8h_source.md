

# File text\_input.h

[**File List**](files.md) **>** [**controls**](dir_035306890ec6a3fa870e30b726ac5ffc.md) **>** [**text\_input.h**](text__input_8h.md)

[Go to the documentation of this file](text__input_8h.md)


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

class TextInput {
public:
    TextInput() = default;
    explicit TextInput(Message label, Message placeholder, std::optional<std::string> default_text = std::nullopt)
        : label_(std::move(label)), placeholder_(std::move(placeholder)), default_text_(std::move(default_text))
    {
    }

    [[nodiscard]] Message getLabel() const
    {
        return label_;
    }

    TextInput &setLabel(Message label)
    {
        label_ = std::move(label);
        return *this;
    }

    [[nodiscard]] Message getPlaceholder() const
    {
        return placeholder_;
    }

    TextInput &setPlaceholder(Message placeholder)
    {
        placeholder_ = std::move(placeholder);
        return *this;
    }

    [[nodiscard]] std::optional<std::string> getDefaultValue() const
    {
        return default_text_;
    }

    TextInput &setDefaultValue(std::optional<std::string> text)
    {
        default_text_ = std::move(text);
        return *this;
    }

private:
    Message label_;
    Message placeholder_;
    std::optional<std::string> default_text_;
};

}  // namespace endstone
```


