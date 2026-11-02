#pragma once

#include <cstddef>

#include "bedrock/bedrock.h"

class ActorFilterGroup {
    std::byte data_[64];  // +0x00; opaque target-filter storage
};

BEDROCK_STATIC_ASSERT_SIZE(ActorFilterGroup, 64, 64);
