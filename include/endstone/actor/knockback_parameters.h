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

namespace endstone {

/**
 * Describes how knockback is calculated for a mob.
 */
class KnockbackParameters {
public:
    /**
     * Defines how extra knockback from sprinting or enchantments is combined with the base knockback.
     */
    enum class ExtraKnockbackApproach : std::uint8_t {
        /** Reapply knockback using the default parameters for values not represented by the extra power. */
        ReapplyDefault,
        /** Multiply the extra power by the base knockback and a reduction factor. */
        MultiplyReduced,
    };

    KnockbackParameters(float horizontal_power, float vertical_power, float vertical_velocity_cap, float slowdown_scale,
                        bool scale_with_damage, bool slow_down_attacker, bool check_legacy_knockback,
                        float extra_knockback_power, ExtraKnockbackApproach extra_knockback_approach)
        : horizontal_power_(horizontal_power), vertical_power_(vertical_power),
          vertical_velocity_cap_(vertical_velocity_cap), slowdown_scale_(slowdown_scale),
          scale_with_damage_(scale_with_damage), slow_down_attacker_(slow_down_attacker),
          check_legacy_knockback_(check_legacy_knockback), extra_knockback_power_(extra_knockback_power),
          extra_knockback_approach_(extra_knockback_approach)
    {
    }

    /**
     * Gets the power with which the target is knocked backwards.
     *
     * @return the horizontal power
     */
    [[nodiscard]] float getHorizontalPower() const { return horizontal_power_; }

    /**
     * Sets the power with which the target is knocked backwards.
     *
     * @param horizontal_power the horizontal power
     */
    void setHorizontalPower(float horizontal_power) { horizontal_power_ = horizontal_power; }

    /**
     * Gets the power with which the target is knocked upwards.
     *
     * @return the vertical power
     */
    [[nodiscard]] float getVerticalPower() const { return vertical_power_; }

    /**
     * Sets the power with which the target is knocked upwards.
     *
     * @param vertical_power the vertical power
     */
    void setVerticalPower(float vertical_power) { vertical_power_ = vertical_power; }

    /**
     * Gets the maximum Y velocity after the knockback rules are evaluated.
     *
     * @return the vertical velocity cap
     */
    [[nodiscard]] float getVerticalVelocityCap() const { return vertical_velocity_cap_; }

    /**
     * Sets the maximum Y velocity after the knockback rules are evaluated.
     *
     * @param vertical_velocity_cap the vertical velocity cap
     */
    void setVerticalVelocityCap(float vertical_velocity_cap) { vertical_velocity_cap_ = vertical_velocity_cap; }

    /**
     * Gets the scale applied to the target's existing velocity before knockback is added.
     *
     * Bedrock clamps this value to the range from `0.0` to `1.0`.
     *
     * @return the slowdown scale
     */
    [[nodiscard]] float getSlowdownScale() const { return slowdown_scale_; }

    /**
     * Sets the scale applied to the target's existing velocity before knockback is added.
     *
     * Bedrock clamps this value to the range from `0.0` to `1.0`.
     *
     * @param slowdown_scale the slowdown scale
     */
    void setSlowdownScale(float slowdown_scale) { slowdown_scale_ = slowdown_scale; }

    /**
     * Gets whether the knockback power is scaled using the damage.
     *
     * @return `true` if the power is scaled using the damage
     */
    [[nodiscard]] bool isScaleWithDamage() const { return scale_with_damage_; }

    /**
     * Sets whether the knockback power is scaled using the damage.
     *
     * @param scale_with_damage `true` to scale the power using the damage
     */
    void setScaleWithDamage(bool scale_with_damage) { scale_with_damage_ = scale_with_damage; }

    /**
     * Gets whether the source's horizontal velocity is slowed when knockback is applied.
     *
     * @return `true` if the source is slowed
     */
    [[nodiscard]] bool isSlowDownAttacker() const { return slow_down_attacker_; }

    /**
     * Sets whether the source's horizontal velocity is slowed when knockback is applied.
     *
     * @param slow_down_attacker `true` to slow the source
     */
    void setSlowDownAttacker(bool slow_down_attacker) { slow_down_attacker_ = slow_down_attacker; }

    /**
     * Gets whether Bedrock checks the legacy pre-Nether Update knockback rules.
     *
     * @return `true` if the legacy rules are checked
     */
    [[nodiscard]] bool isCheckLegacyKnockback() const { return check_legacy_knockback_; }

    /**
     * Sets whether Bedrock checks the legacy pre-Nether Update knockback rules.
     *
     * @param check_legacy_knockback `true` to check the legacy rules
     */
    void setCheckLegacyKnockback(bool check_legacy_knockback) { check_legacy_knockback_ = check_legacy_knockback; }

    /**
     * Gets the extra knockback power supplied by sprinting or enchantments.
     *
     * @return the extra knockback power
     */
    [[nodiscard]] float getExtraKnockbackPower() const { return extra_knockback_power_; }

    /**
     * Sets the extra knockback power supplied by sprinting or enchantments.
     *
     * @param extra_knockback_power the extra knockback power
     */
    void setExtraKnockbackPower(float extra_knockback_power) { extra_knockback_power_ = extra_knockback_power; }

    /**
     * Gets how extra knockback is combined with the base knockback.
     *
     * @return the extra knockback approach
     */
    [[nodiscard]] ExtraKnockbackApproach getExtraKnockbackApproach() const { return extra_knockback_approach_; }

    /**
     * Sets how extra knockback is combined with the base knockback.
     *
     * @param extra_knockback_approach the extra knockback approach
     */
    void setExtraKnockbackApproach(ExtraKnockbackApproach extra_knockback_approach)
    {
        extra_knockback_approach_ = extra_knockback_approach;
    }

private:
    float horizontal_power_;
    float vertical_power_;
    float vertical_velocity_cap_;
    float slowdown_scale_;
    bool scale_with_damage_;
    bool slow_down_attacker_;
    bool check_legacy_knockback_;
    float extra_knockback_power_;
    ExtraKnockbackApproach extra_knockback_approach_;
};

}  // namespace endstone
