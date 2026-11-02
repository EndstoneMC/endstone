#pragma once

#include <cstddef>
#include <cstdint>

#include "bedrock/world/actor/ai/goal/base_goal.h"
#include "bedrock/world/level/block_pos.h"

class BaseMoveToGoal : public BaseGoal {
protected:
    std::int32_t travel_ticks_;
    std::int32_t next_start_tick_;
    std::int32_t give_up_ticks_;
    std::int32_t stay_ticks_;
    std::int32_t max_stay_ticks_;
    bool reached_target_;
    std::byte padding_[3];
    float speed_multiplier_;
    float goal_radius_sq_;
    std::uint64_t cooldown_counter_;
    std::uint64_t cooldown_timeout_time_;
    BlockPos start_pos_;
    Vec3 block_pos_;
    Vec3 target_position_offset_;
    float chance_to_start_;
    Mob *mob_;
    std::int32_t interval_;
};

BEDROCK_STATIC_ASSERT_SIZE(BaseMoveToGoal, 152, 144);
