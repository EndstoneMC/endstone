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

#include "endstone/potion/effect_type.h"

namespace endstone {
/**
 * Represents an effect that can be added to a Mob.
 *
 * An effect is an immutable description of an effect type together with a duration, amplifier and a set of display
 * options. Applying it to a Mob (e.g. via Mob::addEffect) installs the effect; querying a Mob's active effects yields
 * Effect values describing what is currently in force.
 */
class Effect {
public:
    /**
     * Creates an effect.
     *
     * @param type effect type
     * @param duration measured in ticks, see Effect::getDuration
     * @param amplifier the amplifier for the effect, see Effect::getAmplifier
     * @param ambient the ambient status, see Effect::isAmbient
     * @param particles the particle status, see Effect::hasParticles
     * @param icon the icon status, see Effect::hasIcon
     */
    constexpr Effect(EffectId type, int duration, int amplifier, bool ambient = false, bool particles = true,
                     bool icon = true)
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

    /**
     * Gets the type of this effect.
     *
     * @return the type
     */
    [[nodiscard]] constexpr EffectId getType() const noexcept { return type_; }

    /**
     * Gets the duration of this effect, in ticks.
     *
     * @return the duration
     */
    [[nodiscard]] constexpr int getDuration() const noexcept { return duration_; }

    /**
     * Gets the amplifier of this effect.
     *
     * A higher amplifier means the effect happens more often over its duration and in some cases has more effect on
     * its target.
     *
     * @return the amplifier
     */
    [[nodiscard]] constexpr int getAmplifier() const noexcept { return amplifier_; }

    /**
     * Makes the effect produce more, translucent, particles.
     *
     * @return whether this effect is ambient
     */
    [[nodiscard]] constexpr bool isAmbient() const noexcept { return ambient_; }

    /**
     * Gets whether this effect has particles.
     *
     * @return whether this effect has particles
     */
    [[nodiscard]] constexpr bool hasParticles() const noexcept { return particles_; }

    /**
     * Gets whether this effect has an icon.
     *
     * @return whether this effect has an icon
     */
    [[nodiscard]] constexpr bool hasIcon() const noexcept { return icon_; }

private:
    EffectId type_;
    int duration_;
    int amplifier_;
    bool ambient_;
    bool particles_;
    bool icon_;
};
}  // namespace endstone
