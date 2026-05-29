

# File ip\_ban\_entry.h

[**File List**](files.md) **>** [**ban**](dir_f1b1f2e9abb31749ef58cd98f22bcd78.md) **>** [**ip\_ban\_entry.h**](ip__ban__entry_8h.md)

[Go to the documentation of this file](ip__ban__entry_8h.md)


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

#include <utility>

#include "endstone/ban/ban_entry.h"

namespace endstone {

class IpBanEntry : public BanEntry {
public:
    explicit IpBanEntry(std::string address) : address_(std::move(address)) {}

    [[nodiscard]] std::string getAddress() const
    {
        return address_;
    }

private:
    std::string address_;
};

}  // namespace endstone
```


