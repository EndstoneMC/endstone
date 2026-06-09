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

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "bedrock/world/effect/effect_duration.h"
#include "bedrock/world/effect/mob_effect.h"

class MobEffectInstance {
public:
    static const MobEffectInstance NO_EFFECT;

    MobEffectInstance()
        : id_(0), duration_(EffectDuration{0}), amplifier_(0), display_on_screen_texture_animation_(false),
          is_counter_paused_this_tick_(false), ambient_(false), effect_visible_(false), factor_calculation_data_{}
    {
    }

    MobEffectInstance(std::uint32_t id, EffectDuration duration, int amplifier, bool ambient, bool effect_visible,
                      bool display_on_screen_animation)
        : id_(id), duration_(duration), amplifier_(amplifier),
          display_on_screen_texture_animation_(display_on_screen_animation), is_counter_paused_this_tick_(false),
          ambient_(ambient), effect_visible_(effect_visible), factor_calculation_data_{}
    {
        if (const auto *effect = MobEffect::getById(id); effect) {
            effect_visible_ = effect->isVisible() && effect_visible;
        }
    }

    [[nodiscard]] bool operator==(const MobEffectInstance &other) const
    {
        return id_ == other.id_;
    }

    [[nodiscard]] bool operator!=(const MobEffectInstance &other) const
    {
        return id_ != other.id_;
    }

    [[nodiscard]] std::uint32_t getId() const
    {
        return id_;
    }

    [[nodiscard]] EffectDuration getDuration() const
    {
        return duration_;
    }

    [[nodiscard]] int getAmplifier() const
    {
        return amplifier_;
    }

    [[nodiscard]] bool isAmbient() const
    {
        return ambient_;
    }

    [[nodiscard]] bool isEffectVisible() const
    {
        return effect_visible_;
    }

    [[nodiscard]] bool displaysOnScreenTextureAnimation() const
    {
        return display_on_screen_texture_animation_;
    }

    [[nodiscard]] const std::string &getResourceName() const
    {
        static const std::string empty;
        if (const auto *effect = MobEffect::getById(id_); effect) {
            return effect->getResourceName();
        }
        return empty;
    }

    void removeEffects(BaseAttributeMap &attribute_map) const
    {
        if (auto *effect = MobEffect::getById(id_); effect) {
            effect->removeEffects(attribute_map);
        }
    }

private:
    unsigned int id_;
    EffectDuration duration_;
    std::optional<EffectDuration> duration_easy_;
    std::optional<EffectDuration> duration_normal_;
    std::optional<EffectDuration> duration_hard_;
    int amplifier_;
    bool display_on_screen_texture_animation_;
    bool is_counter_paused_this_tick_;
    bool ambient_;
    bool effect_visible_;
    MobEffect::FactorCalculationData factor_calculation_data_;
};

inline const MobEffectInstance MobEffectInstance::NO_EFFECT;

using MobEffectInstanceList = std::vector<MobEffectInstance>;
