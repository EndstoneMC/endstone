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
    Bounce = 10,
    ContainerClose = 11,
    ContainerOpen = 12,
    DispenseFail = 13,
    Drink = 14,
    Eat = 15,
    ElytraGlide = 16,
    EntityDamage = 17,
    EntityDie = 18,
    EntityDismount = 19,
    EntityInteract = 20,
    EntityMount = 21,
    EntityMove = 22,
    EntityPlace = 23,
    EntityRoar = 24,
    EntityAct = 25,
    Equip = 26,
    Explode = 27,
    Flap = 28,
    FluidPickup = 29,
    FluidPlace = 30,
    HitGround = 31,
    ItemInteractFinish = 32,
    ItemInteractStart = 33,
    LightningStrike = 34,
    MultiItemSwap = 35,
    NoteBlockPlay = 36,
    PistonContract = 37,
    PistonExtend = 38,
    PrimeFuse = 39,
    ProjectileLand = 40,
    ProjectileShoot = 41,
    Resonate1 = 42,
    Resonate2 = 43,
    Resonate3 = 44,
    Resonate4 = 45,
    Resonate5 = 46,
    Resonate6 = 47,
    Resonate7 = 48,
    Resonate8 = 49,
    Resonate9 = 50,
    Resonate10 = 51,
    Resonate11 = 52,
    Resonate12 = 53,
    Resonate13 = 54,
    Resonate14 = 55,
    Resonate15 = 56,
    SculkTouch = 57,
    SculkSensorTendrilsClicking = 58,
    Shear = 59,
    Shriek = 60,
    SingleItemSwap = 61,
    Splash = 62,
    Swim = 63,
    Teleport = 64,
    Unequip = 65,
    _Count = 66,
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
}  // namespace GameEventRegistry
