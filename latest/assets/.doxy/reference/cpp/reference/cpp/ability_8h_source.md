

# File ability.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**ability.h**](ability_8h.md)

[Go to the documentation of this file](ability_8h.md)


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

#include <type_traits>
#include <variant>

#include "endstone/registry.h"

namespace endstone {

using AbilityValue = std::variant<bool, float>;

class Ability;

template <typename T>
class AbilityId : public Identifier<Ability> {
public:
    using Identifier::Identifier;

    static constexpr AbilityId minecraft(const std::string_view key) noexcept { return {Minecraft, key}; }
};

class Ability : public Registry<Ability>::Type {
public:
    static constexpr auto AttackMobs = AbilityId<bool>::minecraft("attackmobs");
    static constexpr auto AttackPlayers = AbilityId<bool>::minecraft("attackplayers");
    static constexpr auto Build = AbilityId<bool>::minecraft("build");
    static constexpr auto DoorsAndSwitches = AbilityId<bool>::minecraft("doorsandswitches");
    static constexpr auto Flying = AbilityId<bool>::minecraft("flying");
    static constexpr auto FlySpeed = AbilityId<float>::minecraft("flyspeed");
    static constexpr auto Instabuild = AbilityId<bool>::minecraft("instabuild");
    static constexpr auto Invulnerable = AbilityId<bool>::minecraft("invulnerable");
    static constexpr auto Lightning = AbilityId<bool>::minecraft("lightning");
    static constexpr auto MayFly = AbilityId<bool>::minecraft("mayfly");
    static constexpr auto Mine = AbilityId<bool>::minecraft("mine");
    static constexpr auto Muted = AbilityId<bool>::minecraft("mute");
    static constexpr auto NoClip = AbilityId<bool>::minecraft("noclip");
    static constexpr auto OpenContainers = AbilityId<bool>::minecraft("opencontainers");
    static constexpr auto OperatorCommands = AbilityId<bool>::minecraft("op");
    static constexpr auto PrivilegedBuilder = AbilityId<bool>::minecraft("privilegedbuilder");
    static constexpr auto Teleport = AbilityId<bool>::minecraft("teleport");
    static constexpr auto VerticalFlySpeed = AbilityId<float>::minecraft("verticalflyspeed");
    static constexpr auto WalkSpeed = AbilityId<float>::minecraft("walkspeed");
    static constexpr auto WorldBuilder = AbilityId<bool>::minecraft("worldbuilder");
};

static_assert(std::is_same_v<decltype(Ability::NoClip), const AbilityId<bool>>);
static_assert(std::is_same_v<decltype(Ability::FlySpeed), const AbilityId<float>>);
static_assert(std::is_same_v<decltype(Ability::VerticalFlySpeed), const AbilityId<float>>);
static_assert(std::is_same_v<decltype(Ability::WalkSpeed), const AbilityId<float>>);

}  // namespace endstone
```


