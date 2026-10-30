#pragma once

#include <cstddef>
#include <cstdint>

#include "bedrock/world/actor/ai/goal/goal.h"

class MoveToBlockGoal : public Goal {
protected:
    std::int32_t attempts_;
    float unknown_52_;
    std::int32_t limit_;
    std::uint32_t unknown_60_;
    std::uint32_t unknown_64_;
    std::uint8_t reached_;
    std::byte padding_[3];
    float unknown_72_;
    float unknown_76_;
    std::uint64_t failure_;
    std::byte remaining_[104];
};

BEDROCK_STATIC_ASSERT_SIZE(MoveToBlockGoal, 192, 184);
