#pragma once

#include "bedrock/world/actor/ai/goal/move_to_poi_goal.h"
#include "bedrock/world/actor/ai/util/bed_helper.h"
#include "bedrock/world/level/tick.h"

class SleepGoal : public MoveToPOIGoal {
public:
    ENDSTONE_HOOK bool canUse();
    ENDSTONE_HOOK void stop();

    [[nodiscard]] Mob *tryGetActor() const { return mob_; }
    [[nodiscard]] bool hasFailedNavigation() const
    {
        return !reached_target_ && travel_ticks_ > 0 &&
               (cooldown_counter_ != 0 || travel_ticks_ > give_up_ticks_);
    }

private:
    Mob *mob_;
    BedHelper bed_helper_;
    BlockPos bed_pos_;
    Vec3 sleep_pos_;
    Vec3 bed_offset_;
    Vec3 enter_pos_;
    Vec3 exit_pos_;
    std::int32_t bed_dir_;
    float body_rot_;
    float sleep_y_offset_;
    float sleep_collider_height_;
    float sleep_collider_width_;
    bool can_sleep_while_riding_;
    Tick goal_cooldown_max_;
    Tick cooldown_tick_;
    bool woken_;
    bool goal_ended_early_;
};

BEDROCK_STATIC_ASSERT_SIZE(SleepGoal, 352, 344);
