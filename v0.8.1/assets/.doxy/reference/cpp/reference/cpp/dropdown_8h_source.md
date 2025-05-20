

# File dropdown.h

[**File List**](files.md) **>** [**controls**](dir_035306890ec6a3fa870e30b726ac5ffc.md) **>** [**dropdown.h**](dropdown_8h.md)

[Go to the documentation of this file](dropdown_8h.md)


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
#include <vector>

#include "endstone/message.h"

namespace endstone {

class Dropdown {
public:
    Dropdown() = default;
    explicit Dropdown(Message label, std::vector<std::string> options, std::optional<int> default_index = std::nullopt)
        : label_(std::move(label)), options_(std::move(options)), default_index_(default_index)
    {
    }

    [[nodiscard]] Message getLabel() const
    {
        return label_;
    }

    Dropdown &setLabel(Message label)
    {
        label_ = std::move(label);
        return *this;
    }

    Dropdown &addOption(const std::string &option)
    {
        options_.push_back(option);
        return *this;
    }

    [[nodiscard]] std::vector<std::string> getOptions() const
    {
        return options_;
    }

    Dropdown &setOptions(std::vector<std::string> options)
    {
        options_ = std::move(options);
        return *this;
    }

    [[nodiscard]] std::optional<int> getDefaultIndex() const
    {
        return default_index_;
    }

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
```


