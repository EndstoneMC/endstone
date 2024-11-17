

# File ban\_entry.h

[**File List**](files.md) **>** [**ban**](dir_f1b1f2e9abb31749ef58cd98f22bcd78.md) **>** [**ban\_entry.h**](ban__entry_8h.md)

[Go to the documentation of this file](ban__entry_8h.md)


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

#include <string>
#include <memory>
#include <optional>
#include <chrono>

namespace endstone{
template<typename T>
class BanEntry {
public:
    // Ensure derived class has virtual destructor
    virtual ~BanEntry() = default;

    virtual T& getTarget() const = 0;

    virtual std::chrono::system_clock::time_point getCreated() const = 0;

    virtual void setCreated(std::chrono::system_clock::time_point created) = 0;

    virtual std::string getSource() const = 0;

    virtual void setSource(std::string source) = 0;

    virtual std::optional<std::chrono::system_clock::time_point> getExpiration() const = 0;

    virtual void setExpiration(std::optional<std::chrono::system_clock::time_point> expiration) = 0;

    virtual std::optional<std::string> getReason() const = 0;

    virtual void setReason(std::optional<std::string> reason) = 0;

    virtual void save() = 0;

    virtual void remove() = 0;
};
}
```


