

# File value.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**nbt**](dir_777e494cd9d9b9aeedcd0556d83eeb02.md) **>** [**value.h**](value_8h.md)

[Go to the documentation of this file](value_8h.md)


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

#include <algorithm>
#include <utility>

#include "endstone/nbt/type.h"

namespace endstone::nbt {
class Tag;
template <typename T>
class ValueTag : public TagBase {
public:
    using value_type = T;

    constexpr ValueTag() = default;
    constexpr explicit ValueTag(const T &v) : value_(v) {}
    constexpr explicit ValueTag(T &&v) : value_(std::move(v)) {}

    // Read-only implicit conversion to payload
    operator const T &() const noexcept { return value_; }

    const T &value() const noexcept { return value_; }

    // Equality with same wrapper
    friend constexpr bool operator==(const ValueTag &a, const ValueTag &b) noexcept { return a.value_ == b.value_; }

    friend constexpr bool operator!=(const ValueTag &a, const ValueTag &b) noexcept { return !(a == b); }

    // Equality with underlying payload
    friend constexpr bool operator==(const ValueTag &a, const T &b) noexcept { return a.value_ == b; }
    friend constexpr bool operator==(const T &a, const ValueTag &b) noexcept { return a == b.value_; }
    friend constexpr bool operator!=(const ValueTag &a, const T &b) noexcept { return !(a == b); }
    friend constexpr bool operator!=(const T &a, const ValueTag &b) noexcept { return !(a == b); }

private:
    T value_;
};
}  // namespace endstone::nbt
```


