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

#include <nlohmann/json.hpp>

template <typename ValueType>
struct NewType {
    using Raw = ValueType;
    Raw value;

    operator const Raw &() const
    {
        return value;
    }

    NewType &operator=(const Raw &rhs)
    {
        value = rhs;
        return *this;
    }

    bool operator==(const Raw &rhs) const
    {
        return value == rhs;
    }

    bool operator!=(const Raw &rhs) const
    {
        return !(*this == rhs);
    }
};

template <typename ValueType>
void to_json(nlohmann::json &j, const NewType<ValueType> &nt)
{
    j = nt.value;
}

template <typename ValueType>
void from_json(const nlohmann::json &j, NewType<ValueType> &nt)
{
    j.get_to(nt.value);
}
