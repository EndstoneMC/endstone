#pragma once

#include "bedrock/world/actor/ai/goal/base_goal.h"

class RandomLookAroundGoal : public BaseGoal {
public:
    ENDSTONE_HOOK bool canUse();
    ENDSTONE_HOOK bool canContinue();

    [[nodiscard]] Mob *tryGetActor() const { return mob_; }

private:
    float min_angle_of_view_;
    float max_angle_of_view_;
    std::int32_t min_look_time_;
    std::int32_t max_look_time_;
    float probability_;
    Mob *mob_;
    std::int32_t total_look_time_;
    float initial_rotation_;
    float target_rotation_;
};

BEDROCK_STATIC_ASSERT_SIZE(RandomLookAroundGoal, 96, 88);
