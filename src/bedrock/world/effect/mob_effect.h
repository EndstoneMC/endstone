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
};
