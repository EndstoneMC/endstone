#pragma once

#include <cstdint>
#include <string>

#include "bedrock/bedrock.h"

class Mob;

class Goal {
public:
    virtual ~Goal() = default;

protected:
    std::string id_;
    std::uint32_t control_flags_;
    std::uint16_t flags_;
};

BEDROCK_STATIC_ASSERT_SIZE(Goal, 48, 40);
