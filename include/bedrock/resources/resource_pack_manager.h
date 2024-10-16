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

#include <unordered_set>

#include "bedrock/core/resource/resource_helper.h"
#include "bedrock/platform/threading/mutex_details.h"
#include "bedrock/resources/resource_pack_stack.h"

class ResourcePackManager : public ResourceLoader {
public:
    [[nodiscard]] ResourcePackStack const &getStack(ResourcePackStackType stack_type) const;

private:
    std::unordered_set<gsl::not_null<ResourcePackListener *>> listeners_set_;      // +88
    std::unique_ptr<ResourcePackStack> addon_stack_;                               // +152
    std::unique_ptr<ResourcePackStack> level_stack_;                               // +160
    std::unique_ptr<ResourcePackStack> global_stack_;                              // +168
    std::unique_ptr<ResourcePackStack> treatment_stack_;                           // +176
    std::unique_ptr<ResourcePackStack> base_game_stack_;                           // +184
    std::unique_ptr<ResourcePackStack> full_stack_;                                // +192
    std::unique_ptr<PackSourceReport> loading_report_;                             // +200
    std::string locale_code_;                                                      // +208
    bool initializing_;                                                            // +240
    bool pending_restack_;                                                         // +241
    bool use_global_pack_stack_;                                                   // +242
    Bedrock::Threading::SharedMutex full_stack_access_;                            // +248
    Bedrock::NotNullNonOwnerPtr<const IContentTierManager> content_tier_manager_;  // +256
    SemVersion full_stack_min_engine_version_DEPRECATED_DONOTUSE_;                 // +272 NOLINT
};
