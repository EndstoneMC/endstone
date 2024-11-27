

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

#include <chrono>
#include <optional>
#include <string>
#include <utility>

namespace endstone {
class BanEntry {

public:
    using Date = std::chrono::system_clock::time_point;

    inline static const std::string DateFormat = "%FT%T%Ez";

    [[nodiscard]] Date getCreated() const
    {
        return created_;
    }

    void setCreated(Date created)
    {
        created_ = created;
    }

    [[nodiscard]] std::string getSource() const
    {
        return source_;
    }

    void setSource(std::string source)
    {
        source_ = std::move(source);
    }

    [[nodiscard]] std::optional<Date> getExpiration() const
    {
        return expiration_;
    }

    void setExpiration(std::optional<Date> expiration)
    {
        expiration_ = expiration;
    }

    [[nodiscard]] std::string getReason() const
    {
        return reason_;
    }

    void setReason(std::string reason)
    {
        reason_ = std::move(reason);
    }

private:
    Date created_ = std::chrono::system_clock::now();
    std::string source_ = "(Unknown)";
    std::optional<Date> expiration_ = std::nullopt;
    std::string reason_ = "Banned by an operator.";
};

}  // namespace endstone
```


