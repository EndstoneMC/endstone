// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "bedrock/core/container/blob.h"

namespace mce {
enum class ImageFormat : uint32_t {
    Unknown = 0,
    R8Unorm = 1,
    RG8Unorm = 2,
    RGB8Unorm = 3,
    RGBA8Unorm = 4,
    RGBA16Float = 5,
};

enum class ImageUsage : uint8_t {
    Unknown = 0,
    sRGB = 1,
    Data = 2,
};

struct Image {
    using Storage = Blob;
    ImageFormat image_format;
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    ImageUsage usage;
    Storage image_bytes;
};
static_assert(sizeof(Image) == 48);
}  // namespace mce
