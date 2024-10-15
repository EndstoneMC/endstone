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

#include "bedrock/forward.h"
#include "bedrock/platform/threading/mutex_details.h"
#include "bedrock/resources/resource_pack_repository_interface.h"

class ResourcePackRepository : public IResourcePackRepository {
public:
private:
    ENDSTONE_HOOK void _initializePackSource();  // NOLINT

    Bedrock::NotNullNonOwnerPtr<Core::FilePathManager> file_path_manager_;     // +24
    std::vector<std::unique_ptr<ResourcePack>> all_resource_packs_;            // +40
    std::unique_ptr<CompositePackSource> pack_source_;                         // +64
    std::unique_ptr<CompositePackSource> cache_pack_source_;                   // +72
    std::unique_ptr<CompositePackSource> world_pack_source_;                   // +80
    std::unique_ptr<CompositePackSource> premium_world_template_pack_source_;  // +88
    std::unique_ptr<CompositePackSource> dynamic_package_source_;              // +96
    // std::unique_ptr<TestPackSource> test_behavior_pack_source_;
    // std::unique_ptr<TestPackSource> test_resource_pack_source_;
    std::unique_ptr<PackSourceReport> pack_source_report_;                                 // +104
    ResourcePack *editor_pack_;                                                            // +112
    ResourcePack *vanilla_pack_;                                                           // +120
    std::vector<PackIdVersion> service_packs_;                                             // +128
    std::vector<ResourceLocation> invalid_pack_location_;                                  // +152
    std::vector<ResourceLocation> invalid_behavior_pack_location_;                         // +176
    std::vector<ResourceLocation> invalid_resource_pack_location_;                         // +200
    std::vector<ResourceLocation> invalid_template_pack_location_;                         // +224
    IMinecraftEventing &eventing_;                                                         // +248
    PackManifestFactory &manifest_factory_;                                                // +256
    Bedrock::NotNullNonOwnerPtr<IContentAccessibilityProvider> content_accessibility_;     // +264
    Core::HeapPathBuffer current_world_path_;                                              // +280
    Core::HeapPathBuffer current_premium_world_template_path_;                             // +312
    ContentKeyMap temp_cache_content_keys_;                                                // +344
    std::unique_ptr<PackSettingsFactory> pack_settings_factory_;                           // +408
    PackSourceFactory &pack_source_factory_;                                               // +416
    std::map<void *, std::function<void(ResourcePack *)>> remove_resource_pack_callback_;  // +424
    std::unique_ptr<TaskGroup> init_task_group_;                                           // +440
    Bedrock::Threading::Mutex initialize_mutex_;                                           // +448
    bool cancel_initialization_;                                                           // +528
    bool initialized_;                                                                     // +529
    bool reload_user_packs_requested_;                                                     // +530
    bool reload_dynamic_package_packs_requested_;                                          // +531
    bool refresh_packs_requested_;                                                         // +532
    ContentIdentity current_premium_world_template_identity_;                              // +536
};
