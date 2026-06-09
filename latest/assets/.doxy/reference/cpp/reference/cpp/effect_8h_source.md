

# File effect.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**potion**](dir_3aeea1eb013bc4968668ceaff3f9e2f0.md) **>** [**effect.h**](effect_8h.md)

[Go to the documentation of this file](effect_8h.md)


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

#include <optional>

#include "endstone/potion/effect_type.h"

namespace endstone {
class Effect {
public:
    constexpr Effect(EffectId type, std::optional<int> duration, int amplifier, bool ambient = false,
                     bool particles = true, bool icon = true)
        : type_(type), duration_(duration), amplifier_(amplifier), ambient_(ambient), particles_(particles),
          icon_(icon)
    {
    }

    constexpr bool operator==(const Effect &other) const noexcept
    {
        return type_ == other.type_ && duration_ == other.duration_ && amplifier_ == other.amplifier_ &&
               ambient_ == other.ambient_ && particles_ == other.particles_ && icon_ == other.icon_;
    }

    constexpr bool operator!=(const Effect &other) const noexcept { return !(*this == other); }

    [[nodiscard]] constexpr EffectId getType() const noexcept { return type_; }

    [[nodiscard]] constexpr std::optional<int> getDuration() const noexcept { return duration_; }

    [[nodiscard]] constexpr bool isInfinite() const noexcept { return !duration_.has_value(); }

    [[nodiscard]] constexpr int getAmplifier() const noexcept { return amplifier_; }

    [[nodiscard]] constexpr bool isAmbient() const noexcept { return ambient_; }

    [[nodiscard]] constexpr bool hasParticles() const noexcept { return particles_; }

    [[nodiscard]] constexpr bool hasIcon() const noexcept { return icon_; }

private:
    EffectId type_;
    std::optional<int> duration_;
    int amplifier_;
    bool ambient_;
    bool particles_;
    bool icon_;
};
}  // namespace endstone
```


