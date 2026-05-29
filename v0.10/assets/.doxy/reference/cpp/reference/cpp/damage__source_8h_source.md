

# File damage\_source.h

[**File List**](files.md) **>** [**damage**](dir_d35032eb9ca22acb8eface5f2c8b6a35.md) **>** [**damage\_source.h**](damage__source_8h.md)

[Go to the documentation of this file](damage__source_8h.md)


```C++
// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include "endstone/actor/actor.h"

namespace endstone {

class DamageSource {
public:
    virtual ~DamageSource() = default;

    [[nodiscard]] virtual std::string_view getType() const = 0;

    [[nodiscard]] virtual Actor *getActor() const = 0;

    [[nodiscard]] virtual Actor *getDamagingActor() const = 0;

    [[nodiscard]] virtual bool isIndirect() const = 0;
};

}  // namespace endstone

namespace fmt {
template <>
struct formatter<endstone::DamageSource> : formatter<string_view> {
    using Type = endstone::DamageSource;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        auto it = ctx.out();
        it = fmt::format_to(it, "DamageSource(type={}", val.getType());
        if (auto *actor = val.getActor()) {
            it = fmt::format_to(it, ", actor={}", *actor);
        }
        else {
            it = fmt::format_to(it, ", actor=None");
        }
        if (auto *damaging_actor = val.getDamagingActor()) {
            it = fmt::format_to(it, ", damaging_actor={}", *damaging_actor);
        }
        else {
            it = fmt::format_to(it, ", damaging_actor=None");
        }
        it = fmt::format_to(it, ")");
        return it;
    }
};
}  // namespace fmt
```


