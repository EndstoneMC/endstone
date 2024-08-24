

# File event.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**event**](dir_f1d783c0ad83ee143d16e768ebca51c8.md) **>** [**event.h**](event_8h.md)

[Go to the documentation of this file](event_8h.md)


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

namespace endstone {

class Event {
public:
    explicit Event(bool async = false) : async_(async){};
    Event(const Event &) = delete;             // deleted copy constructor
    Event &operator=(const Event &) = delete;  // deleted copy assignment operator

    virtual ~Event() = default;

    [[nodiscard]] virtual std::string getEventName() const = 0;

    [[nodiscard]] virtual bool isCancellable() const = 0;

    [[nodiscard]] bool isCancelled() const
    {
        if (!isCancellable()) {
            return false;
        }
        return cancelled_;
    };

    void setCancelled(bool cancel)
    {
        if (isCancellable()) {
            cancelled_ = cancel;
        }
    }

    [[nodiscard]] bool isAsynchronous() const
    {
        return async_;
    }

private:
    bool async_;
    bool cancelled_{false};
};

}  // namespace endstone
```


