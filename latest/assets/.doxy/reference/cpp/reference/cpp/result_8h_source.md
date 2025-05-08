

# File result.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**util**](dir_89b85071337bf933dea6c29b4c6a4410.md) **>** [**result.h**](result_8h.md)

[Go to the documentation of this file](result_8h.md)


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

#include <nonstd/expected.hpp>

namespace endstone {

template <typename T>
using Result = nonstd::expected<T, std::string>;

#define ENDSTONE_CHECK(EXPRESSION, ERROR_MSG) \
    if (!(EXPRESSION))                        \
        return nonstd::make_unexpected(ERROR_MSG);

#define ENDSTONE_CHECKF(EXPRESSION, ERROR_MSG, ...) \
    if (!(EXPRESSION))                              \
        return nonstd::make_unexpected(fmt::format(ERROR_MSG, ##__VA_ARGS__));

}  // namespace endstone
```


