#pragma once

#include <cstdint>
#include <memory>

#include "bedrock/world/actor/ai/goal/base_move_to_goal.h"
#include "bedrock/world/level/pathfinder/path.h"
#include "bedrock/world/phys/aabb.h"

class MoveToPOIGoal : public BaseMoveToGoal {
protected:
    std::uint32_t poi_type_;
    AABB poi_bounding_box_;
    bool using_bounding_box_;
    bool require_same_y_;
    std::unique_ptr<Path> path_;
};

BEDROCK_STATIC_ASSERT_SIZE(MoveToPOIGoal, 192, 184);
