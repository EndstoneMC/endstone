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

class ActorHurtResult {
public:
    static ActorHurtResult createHurt(std::optional<float> damage_dealt);
    static ActorHurtResult createHurtNoDamage();
    static ActorHurtResult createNotHurt();
    ActorHurtResult &setHurt(bool was_hurt);
    ActorHurtResult &setShouldAllowKnockback(bool should_do_knockback);
    bool wasHurt() const;
    bool wasDealtDamage() const;
    bool wasDamageProcessed() const;
    std::optional<float> getDamage() const;
    bool shouldAllowKnockback() const;

private:
    enum class Type : int {
        NotHurt = 0,
        EffectsOnly = 1,
        DamageProcessed = 2,
    };
    ActorHurtResult(Type type, std::optional<float> damage);
    Type type_;
    std::optional<float> damage_amount_;
    bool should_allow_knockback_;
};
