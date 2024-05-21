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

#include <entt/entt.hpp>

/**
 * From entityCategoryFromString
 */
enum class ActorCategory {
    None = 0,
    Player = 1 << 0,
    Mob = 1 << 1,
    Monster = 1 << 2,
    Humanoid = 1 << 3,
    Animal = 1 << 4,
    Water = 1 << 5,
    Pathable = 1 << 6,
    Tamable = 1 << 7,
    Ridable = 1 << 8,
    Item = 1 << 10,
    Ambient = 1 << 11,
    Villager = 1 << 12,
    Arthropod = 1 << 13,
    Undead = 1 << 14,
    Zombie = 1 << 15,
    Minecart = 1 << 16,
    Boat = 1 << 17,

    BoatRideable = Boat | Ridable,
    MinecartRidable = Minecart | Ridable,
    HumanoidMonster = Humanoid | Monster,
    WaterAnimal = Water | Animal,
    TamableAnimal = Tamable | Animal,
    UndeadMob = Undead | Monster,  // wtf mojang
    ZombieMonster = Zombie | Monster
};

template <>
struct entt::enum_as_bitmask<ActorCategory> : std::true_type {};
