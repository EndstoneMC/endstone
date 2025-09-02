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

namespace GameEventConfig {
using GameEventCategoriesStorage = uint8_t;
enum GameEventCategory : GameEventCategoriesStorage {
    NoCategory = 0,
    VibrationsCategory = 1,
    IgnoreVibrationsIfSneakingCategory = 2,
    IgnoreVibrationsOnDampeningBlockCategory = 4,
    IgnoreVibrationsInDampeningBlockCategory = 8,
    HandleVibrationsFromDampeningActorCategory = 16,
};

enum class GameEventType : int {
    _Invalid = 0,
    BlockActivate = 1,
    BlockAttach = 2,
    BlockChange = 3,
    BlockClose = 4,
    BlockDeactivate = 5,
    BlockDestroy = 6,
    BlockDetach = 7,
    BlockOpen = 8,
    BlockPlace = 9,
    ContainerClose = 10,
    ContainerOpen = 11,
    DispenseFail = 12,
    Drink = 13,
    Eat = 14,
    ElytraGlide = 15,
    EntityDamage = 16,
    EntityDie = 17,
    EntityDismount = 18,
    EntityInteract = 19,
    EntityMount = 20,
    EntityMove = 21,
    EntityPlace = 22,
    EntityRoar = 23,
    EntityAct = 24,
    Equip = 25,
    Explode = 26,
    Flap = 27,
    FluidPickup = 28,
    FluidPlace = 29,
    HitGround = 30,
    ItemInteractFinish = 31,
    ItemInteractStart = 32,
    LightningStrike = 33,
    NoteBlockPlay = 34,
    PistonContract = 35,
    PistonExtend = 36,
    PrimeFuse = 37,
    ProjectileLand = 38,
    ProjectileShoot = 39,
    Resonate1 = 40,
    Resonate2 = 41,
    Resonate3 = 42,
    Resonate4 = 43,
    Resonate5 = 44,
    Resonate6 = 45,
    Resonate7 = 46,
    Resonate8 = 47,
    Resonate9 = 48,
    Resonate10 = 49,
    Resonate11 = 50,
    Resonate12 = 51,
    Resonate13 = 52,
    Resonate14 = 53,
    Resonate15 = 54,
    SculkTouch = 55,
    SculkSensorTendrilsClicking = 56,
    Shear = 57,
    Shriek = 58,
    Splash = 59,
    Swim = 60,
    Teleport = 61,
    Unequip = 62,
    _Count = 63,
};

}  // namespace GameEventConfig

class GameEvent {
public:
    GameEvent(GameEventConfig::GameEventType type, GameEventConfig::GameEventCategoriesStorage categories)
        : type_(type), categories_(categories)
    {
    }

private:
    const GameEventConfig::GameEventType type_;
    const GameEventConfig::GameEventCategoriesStorage categories_;
};

namespace GameEventRegistry {
static const GameEvent blockChange(GameEventConfig::GameEventType::BlockChange, GameEventConfig::VibrationsCategory);
static const GameEvent entityDamage(GameEventConfig::GameEventType::EntityDamage, GameEventConfig::VibrationsCategory);
}
