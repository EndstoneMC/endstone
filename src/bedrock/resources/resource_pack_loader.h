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

#include <cstddef>
#include <cstdint>
#include <memory>

#include "bedrock/bedrock.h"
#include "bedrock/core/resource/resource_loader_base.h"
#include "bedrock/resources/resource_pack_stack.h"

class ResourcePackLoader : public ResourceLoaderBase {
private:
    std::unique_ptr<ResourcePackStack> full_stack_;                   // +88
    std::uint64_t weak_this_[2];                                      // +96
    alignas(std::max_align_t) std::byte category_file_mapping_[64];   // +112
    alignas(std::max_align_t) std::byte merge_strategy_factory_[64];  // +176
};
BEDROCK_STATIC_ASSERT_SIZE(ResourcePackLoader, 240, 240);
