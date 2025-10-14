

# File form.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**form**](dir_0fd3b458603af3963ebb9c312a9238ec.md) **>** [**form.h**](form_8h.md)

[Go to the documentation of this file](form_8h.md)


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

#include <functional>

#include "endstone/message.h"

namespace endstone {

class Player;

template <typename T>
class Form {
public:
    using OnCloseCallback = std::function<void(Player *)>;
    explicit Form() = default;

    [[nodiscard]] Message getTitle() const
    {
        return title_;
    }

    T &setTitle(Message title)
    {
        title_ = std::move(title);
        return *static_cast<T *>(this);
    }

    T &setOnClose(OnCloseCallback on_close)
    {
        on_close_ = std::move(on_close);
        return *static_cast<T *>(this);
    }

    [[nodiscard]] OnCloseCallback getOnClose() const
    {
        return on_close_;
    }

protected:
    Message title_;
    OnCloseCallback on_close_;
};

}  // namespace endstone
```


