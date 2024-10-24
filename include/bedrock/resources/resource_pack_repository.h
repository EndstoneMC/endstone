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

    Bedrock::NotNullNonOwnerPtr<Core::FilePathManager> file_path_manager_;                 // +24
    std::vector<std::unique_ptr<ResourcePack>> all_resource_packs_;                        // +48
    std::unique_ptr<CompositePackSource> pack_source_;                                     // +72
    std::unique_ptr<CompositePackSource> cache_pack_source_;                               // +80
    std::unique_ptr<CompositePackSource> world_pack_source_;                               // +88
    std::unique_ptr<CompositePackSource> premium_world_template_pack_source_;              // +96
    std::unique_ptr<CompositePackSource> dynamic_package_source_;                          // +104
    std::unique_ptr<TestPackSource> test_behavior_pack_source_;                            // +112
    std::unique_ptr<TestPackSource> test_resource_pack_source_;                            // +120
    std::unique_ptr<PackSourceReport> pack_source_report_;                                 // +128
    ResourcePack *editor_pack_;                                                            // +136
    ResourcePack *vanilla_pack_;                                                           // +144
    std::vector<PackIdVersion> service_packs_;                                             // +152
    std::vector<ResourceLocation> invalid_pack_location_;                                  // +176
    std::vector<ResourceLocation> invalid_behavior_pack_location_;                         // +200
    std::vector<ResourceLocation> invalid_resource_pack_location_;                         // +224
    std::vector<ResourceLocation> invalid_template_pack_location_;                         // +248
    IMinecraftEventing *eventing_;                                                         // +272
    PackManifestFactory *manifest_factory_;                                                // +280
    Bedrock::NotNullNonOwnerPtr<IContentAccessibilityProvider> content_accessibility_;     // +288
    Core::HeapPathBuffer current_world_path_;                                              // +312
    Core::HeapPathBuffer current_premium_world_template_path_;                             // +344
    ContentKeyMap temp_cache_content_keys_;                                                // +376
    std::unique_ptr<PackSettingsFactory> pack_settings_factory_;                           // +440
    PackSourceFactory *pack_source_factory_;                                               // +448
    std::map<void *, std::function<void(ResourcePack *)>> remove_resource_pack_callback_;  // +456
    std::unique_ptr<TaskGroup> init_task_group_;                                           // +472
    Bedrock::Threading::Mutex initialize_mutex_;                                           // +480
    bool cancel_initialization_;                                                           // +560
    bool initialized_;                                                                     // +561
    bool reload_user_packs_requested_;                                                     // +562
    bool reload_dynamic_package_packs_requested_;                                          // +563
    bool refresh_packs_requested_;                                                         // +564
    ContentIdentity current_premium_world_template_identity_;                              // +568 (+480)
};
BEDROCK_STATIC_ASSERT_SIZE(ResourcePackRepository, 592, 504);
