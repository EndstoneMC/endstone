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

#include <cstddef>
#include <functional>
#include <string>

#include "bedrock/core/math/color.h"
#include "bedrock/core/string/string_hash.h"
#include "bedrock/world/effect/effect_duration.h"

class Actor;
class BaseAttributeMap;

using MobEffectId = unsigned int;

class MobEffect {
public:
    static constexpr std::size_t NUM_EFFECTS = 38;
    static constexpr int UNKNOWN_EFFECT = 0;
    static constexpr int DARKNESS_EFFECT_FACTOR_PADDING_DURATION = 22;
    struct FactorCalculationData {
        int padding_duration;
        float factor_start;
        float factor_target;
        float factor_current;
        std::function<void(FactorCalculationData &, EffectDuration)> update_fn;
        int ticks_active;
        float factor_previous_frame;
        bool had_effect_last_tick;
        bool had_applied;
    };

    // Endstone: vtable reconstruction. Declared in BDS order (~MobEffect, applyEffects, removeEffects) so a virtual
    //           call to removeEffects dispatches to the correct slot. applyEffects is a placeholder occupying its
    //           slot and is never called; a MobEffect is never constructed here (instances come from the registry).
    virtual ~MobEffect() = default;
    virtual void applyEffects(Actor &target, EffectDuration duration, int amplification) const = 0;
    virtual void removeEffects(BaseAttributeMap &attribute_map) = 0;

    static MobEffect *getById(MobEffectId id);
    static MobEffect *getByName(const std::string &name);

    [[nodiscard]] MobEffectId getId() const { return id_; }

    [[nodiscard]] const std::string &getResourceName() const { return resource_name_; }

    [[nodiscard]] bool isVisible() const { return effect_visible_; }

private:
    const MobEffectId id_;
    bool is_harmful_;
    Color color_;
    HashedString particle_effect_id_;
    HashedString particle_effect_ambient_id_;
    std::string description_id_;
    int icon_;
    float duration_modifier_;
    bool is_disabled_;
    std::string resource_name_;
    std::string icon_name_;
    bool effect_visible_;
};
