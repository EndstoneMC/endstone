#pragma once

#include <cstddef>

#include "bedrock/bedrock.h"

class ActorFilterGroup {
#ifdef _WIN32
    std::byte data_[64];  // +0x00; opaque target-filter storage
#else
    std::byte data_[40];  // +0x00; opaque target-filter storage
#endif
};

BEDROCK_STATIC_ASSERT_SIZE(ActorFilterGroup, 64, 40);
