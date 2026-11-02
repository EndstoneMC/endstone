#pragma once

#include <vector>

#include "bedrock/world/actor/ai/goal/base_goal.h"
#include "bedrock/world/actor/ai/goal/mob_descriptor.h"

class TargetGoal : public BaseGoal {
protected:
    Mob *mob_;
    bool attack_owner_;
    bool must_reach_;
    bool global_must_see_;
    bool target_must_see_;
    std::int32_t target_must_see_forget_ticks_;
    bool filter_failure_;
    std::int32_t filter_failure_ticks_;
    std::int32_t global_must_see_forget_ticks_;
    std::int32_t persist_target_ticks_;
    std::int32_t reach_cache_;
    std::int32_t reach_cache_time_;
    std::int32_t unseen_ticks_;
    float within_default_;
    bool reevaluate_target_;
    std::vector<MobDescriptor> target_types_;
};

BEDROCK_STATIC_ASSERT_SIZE(TargetGoal, 128, 120);
