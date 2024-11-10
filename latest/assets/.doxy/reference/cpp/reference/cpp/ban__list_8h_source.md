

# File ban\_list.h

[**File List**](files.md) **>** [**ban**](dir_f1b1f2e9abb31749ef58cd98f22bcd78.md) **>** [**ban\_list.h**](ban__list_8h.md)

[Go to the documentation of this file](ban__list_8h.md)


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

#include <vector>
#include <string>
#include <optional>
#include <chrono>

namespace endstone {

template <typename T>
class BanList {
public:
    virtual ~BanList() = default;

    virtual BanEntry<T>* getBanEntry(T target) const = 0;

    virtual BanEntry<T>& addBan(T target,
                                std::optional<std::string> reason,
                                std::optional<std::chrono::system_clock::time_point> expires,
                                std::optional<std::string> source) = 0;

    virtual BanEntry<T>& addBan(T target,
                                std::optional<std::string> reason,
                                std::optional<std::chrono::seconds> duration,
                                std::optional<std::string> source) = 0;

    virtual std::vector<BanEntry<T>*> getEntries() const = 0;

    virtual bool isBanned(T target) const = 0;

    virtual void removeBan(T target) = 0;
};

}  // namespace endstone

```


