#pragma once

#include <unordered_map>

#include "bedrock/util/int_range.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/actor/ai/goal/target/target_goal.h"
#include "bedrock/world/level/tick.h"

class NearestAttackableTargetGoal : public TargetGoal {
public:
    ENDSTONE_HOOK ActorUniqueID _findTarget(const MobDescriptor **matched_definition);

    [[nodiscard]] Mob *tryGetActor() const { return mob_; }

private:
    ActorUniqueID target_id_;
    const MobDescriptor *target_descriptor_;
    bool reselect_targets_;
    std::unordered_map<const MobDescriptor *, Tick> target_cooldowns_;
    IntRange attack_interval_ticks_;
    std::int32_t scan_interval_;
    bool set_persistent_;
    float target_search_height_;
    float target_invisible_multiplier_;
    float target_sneak_visibility_multiplier_;
    float target_acquisition_probability_;
};

BEDROCK_STATIC_ASSERT_SIZE(NearestAttackableTargetGoal, 248, 216);
