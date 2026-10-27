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
#include <unordered_set>

#include "bedrock/platform/threading/mutex_details.h"
#include "bedrock/resources/resource_pack_loader.h"
#include "bedrock/resources/resource_pack_stack.h"

class ResourcePackManager : public ResourcePackLoader {
public:
    [[nodiscard]] ResourcePackStack const &getStack(ResourcePackStackType stack_type) const;
    bool setStack(std::unique_ptr<ResourcePackStack> stack, ResourcePackStackType stack_type);

private:
    std::unordered_set<gsl::not_null<ResourcePackListener *>> listeners_set_;      // +240
    std::unique_ptr<ResourcePackStack> level_stack_;                               // +304
    std::unique_ptr<ResourcePackStack> global_stack_;                              // +312
    std::unique_ptr<ResourcePackStack> treatment_stack_;                           // +320
    std::unique_ptr<ResourcePackStack> base_game_stack_;                           // +328
    std::unique_ptr<PackSourceReport> loading_report_;                             // +336
    std::string locale_code_;                                                      // +344
    bool initializing_;                                                            // +376
    bool pending_restack_;                                                         // +377
    bool use_global_pack_stack_;                                                   // +378
    bool gameplay_resources_loaded_;                                               // +379
    Bedrock::Threading::SharedMutex full_stack_access_;                            // +384
    Bedrock::NotNullNonOwnerPtr<const IContentTierManager> content_tier_manager_;  // +392
    SemVersion full_stack_min_engine_version_DEPRECATED_DONOTUSE_;                 // +416 NOLINT
};
BEDROCK_STATIC_ASSERT_SIZE(ResourcePackManager, 440, 544);
