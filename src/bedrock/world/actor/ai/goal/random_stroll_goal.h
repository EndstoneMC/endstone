#pragma once

#include "bedrock/world/actor/ai/goal/base_goal.h"
#include "bedrock/world/level/block_pos.h"

class RandomStrollGoal : public BaseGoal {
public:
    ENDSTONE_HOOK bool canUse();
    ENDSTONE_HOOK bool canContinue();

    [[nodiscard]] Mob *tryGetActor() const { return mob_; }

private:
    Mob *mob_;
    float speed_;
    std::int32_t xz_dist_;
    std::int32_t y_dist_;
    std::int32_t interval_;
    Vec3 wanted_position_;
    BlockPos chosen_end_pos_;
    bool reached_target_;
};

BEDROCK_STATIC_ASSERT_SIZE(RandomStrollGoal, 104, 96);
