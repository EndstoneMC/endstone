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
    ResourcePackRepository(IMinecraftEventing &, PackManifestFactory &,
                           const Bedrock::NotNullNonOwnerPtr<IContentAccessibilityProvider> &,
                           const Bedrock::NotNullNonOwnerPtr<Core::FilePathManager> &,
                           Bedrock::NonOwnerPointer<PackCommand::IPackCommandPipeline>, PackSourceFactory &, bool);

private:
    friend class endstone::core::EndstoneServer;

    ENDSTONE_HOOK void _initializePackSource();  // NOLINT

    Bedrock::NotNullNonOwnerPtr<Core::FilePathManager> file_path_manager_;
    gsl::not_null<std::shared_ptr<RepositoryPacks>> all_resource_packs_;
    std::unique_ptr<CompositePackSource> pack_source_;
    std::unique_ptr<CompositePackSource> cache_pack_source_;
    std::unique_ptr<CompositePackSource> world_pack_source_;
    std::unique_ptr<CompositePackSource> premium_world_template_pack_source_;
    std::unique_ptr<CompositePackSource> temp_world_template_pack_source_;
    std::unique_ptr<PackSourceReport> pack_source_report_;
    ResourcePack *editor_pack_;
    ResourcePack *vanilla_pack_;
    std::vector<PackIdVersion> service_packs_;
    std::vector<ResourceLocation> invalid_pack_location_;
    std::vector<ResourceLocation> invalid_behavior_pack_location_;
    std::vector<ResourceLocation> invalid_resource_pack_location_;
    std::vector<ResourceLocation> invalid_template_pack_location_;
    IMinecraftEventing &eventing_;
    PackManifestFactory &manifest_factory_;
    Bedrock::NotNullNonOwnerPtr<IContentAccessibilityProvider> content_accessibility_;
    Core::HeapPathBuffer current_world_path_;
    Core::HeapPathBuffer current_premium_world_template_path_;
    ContentKeyMap temp_cache_content_keys_;
    std::unique_ptr<PackSettingsFactory> pack_settings_factory_;
    PackSourceFactory &pack_source_factory_;
    Bedrock::NonOwnerPointer<PackCommand::IPackCommandPipeline> commands_;
    std::unique_ptr<TaskGroup> init_task_group_;
    Bedrock::Threading::Mutex initialize_mutex_;
    bool cancel_initialization_;
    bool initialized_;
    bool reload_user_packs_requested_;
    bool refresh_packs_requested_;
    ContentIdentity current_premium_world_template_identity_;
};
BEDROCK_STATIC_ASSERT_SIZE(ResourcePackRepository, 576, 496);
