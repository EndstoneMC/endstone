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

#include <cstdint>

#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/resources/pack_error.h"
#include "bedrock/resources/pack_settings.h"
#include "bedrock/resources/pack_source.h"
#include "bedrock/resources/resource_pack.h"

class IResourcePackRepository;

class PackStats {
public:
    uint32_t overridden_entity_count;
    uint32_t custom_entity_count;
    uint32_t custom_animation_count;
    uint32_t custom_effect_count;
};

class PackInstance {
public:
    PackInstance(Bedrock::NotNullNonOwnerPtr<ResourcePack> pack, int subpack_index, bool is_dependent,
                 PackSettings *pack_settings);

    PackReport pack_report;

private:
    PackSettings *pack_settings_;
    Bedrock::NonOwnerPointer<ResourcePack> pack_;
    PackStats stats_{};
    int subpack_index_;
};

class ResourcePackStack {
    using PackInstanceStack = std::vector<PackInstance>;

public:
    virtual ~ResourcePackStack() = 0;

    ENDSTONE_HOOK static std::unique_ptr<ResourcePackStack> deserialize(
        std::istream &file_stream, Bedrock::NotNullNonOwnerPtr<const IResourcePackRepository> const &repo);

    PackInstanceStack stack;

private:
    std::unique_ptr<PackSourceReport> pack_source_report_;
};
