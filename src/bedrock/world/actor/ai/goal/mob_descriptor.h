#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>

#include "bedrock/world/actor/actor_filter_group.h"

struct MobDescriptor {
    ActorFilterGroup target_filter_;
    float max_distance_;
    float max_height_;
    float max_flee_;
    float walk_speed_modifier_;
    float sprint_speed_modifier_;
    std::optional<bool> must_see_;
    bool check_if_outnumbered_;
    bool reevaluate_description_;
    std::int32_t must_see_forget_ticks_;
    std::int32_t priority_;
    std::int32_t cooldown_in_ticks_;
#ifdef _WIN32
    std::uint32_t unknown_100_;  // +0x64; trailing field in the 104-byte descriptor
#else
    std::byte unknown_76_[28];   // +0x4c; trailing storage in the 104-byte descriptor
#endif
};

BEDROCK_STATIC_ASSERT_SIZE(MobDescriptor, 104, 104);
