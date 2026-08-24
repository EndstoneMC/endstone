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

/**
 * Represents the identifier of a player ability, carrying the type of the ability's value.
 */
template <typename T>
class AbilityId : public Identifier<Ability> {
public:
    using Identifier::Identifier;

    static constexpr AbilityId minecraft(const std::string_view key) noexcept { return {Minecraft, key}; }
};

/**
 * All player abilities.
 *
 * AttackMobs, AttackPlayers, Build, DoorsAndSwitches, Mine, OpenContainers, OperatorCommands and Teleport are the
 * eight member permissions the client shows in its pause menu, and the server enforces every one of them.
 */
class Ability : public Registry<Ability>::Type {
public:
    /** Whether the player can attack mobs. */
    static constexpr auto AttackMobs = AbilityId<bool>::minecraft("attackmobs");
    /** Whether the player can attack other players. */
    static constexpr auto AttackPlayers = AbilityId<bool>::minecraft("attackplayers");
    /** Whether the player can place blocks. */
    static constexpr auto Build = AbilityId<bool>::minecraft("build");
    /** Whether the player can use doors, trapdoors, buttons, levers and other redstone components. */
    static constexpr auto DoorsAndSwitches = AbilityId<bool>::minecraft("doorsandswitches");
    /** Whether the player is currently flying. */
    static constexpr auto Flying = AbilityId<bool>::minecraft("flying");
    /** The speed at which the player flies, default is `0.05`. */
    static constexpr auto FlySpeed = AbilityId<float>::minecraft("flyspeed");
    /** Whether the player destroys blocks instantly. */
    static constexpr auto Instabuild = AbilityId<bool>::minecraft("instabuild");
    /** Whether the player is immune to all damage. */
    static constexpr auto Invulnerable = AbilityId<bool>::minecraft("invulnerable");
    /** Whether the player was struck by lightning. */
    static constexpr auto Lightning = AbilityId<bool>::minecraft("lightning");
    /** Whether the player is allowed to fly. */
    static constexpr auto MayFly = AbilityId<bool>::minecraft("mayfly");
    /** Whether the player can destroy blocks. */
    static constexpr auto Mine = AbilityId<bool>::minecraft("mine");
    /** Whether the player's chat messages are hidden from other players. */
    static constexpr auto Muted = AbilityId<bool>::minecraft("mute");
    /** Whether the player can move through blocks. */
    static constexpr auto NoClip = AbilityId<bool>::minecraft("noclip");
    /** Whether the player can open containers. */
    static constexpr auto OpenContainers = AbilityId<bool>::minecraft("opencontainers");
    /** Whether the player can use operator commands. */
    static constexpr auto OperatorCommands = AbilityId<bool>::minecraft("op");
    /** Whether the player is a privileged builder. */
    static constexpr auto PrivilegedBuilder = AbilityId<bool>::minecraft("privilegedbuilder");
    /** Whether the player can teleport. */
    static constexpr auto Teleport = AbilityId<bool>::minecraft("teleport");
    /** The speed at which the player flies up and down, default is `1`. */
    static constexpr auto VerticalFlySpeed = AbilityId<float>::minecraft("verticalflyspeed");
    /** The speed at which the player walks, default is `0.1`. */
    static constexpr auto WalkSpeed = AbilityId<float>::minecraft("walkspeed");
    /** Whether the player is a world builder. */
    static constexpr auto WorldBuilder = AbilityId<bool>::minecraft("worldbuilder");
};

static_assert(std::is_same_v<decltype(Ability::NoClip), const AbilityId<bool>>);
static_assert(std::is_same_v<decltype(Ability::FlySpeed), const AbilityId<float>>);
static_assert(std::is_same_v<decltype(Ability::VerticalFlySpeed), const AbilityId<float>>);
static_assert(std::is_same_v<decltype(Ability::WalkSpeed), const AbilityId<float>>);

}  // namespace endstone
