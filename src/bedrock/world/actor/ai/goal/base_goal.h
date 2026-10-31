#pragma once

#include <cstdint>
#include <string>

#include "bedrock/bedrock.h"

class Mob;

class BaseGoal {
public:
    virtual ~BaseGoal() = default;

protected:
    std::string name_;
    std::uint32_t required_control_flags_;
    std::uint16_t type_id_;
};

BEDROCK_STATIC_ASSERT_SIZE(BaseGoal, 48, 40);
