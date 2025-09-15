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
#include "bedrock/resources/content_source_repository.h"
#include "bedrock/resources/repository_loading.h"
#include "bedrock/resources/repository_sources.h"
#include "bedrock/resources/resource_pack_repository_interface.h"

class IRepositoryFactory {
public:
    virtual ~IRepositoryFactory();
    virtual std::shared_ptr<RepositorySources> createSources(const IResourcePackRepository &) = 0;
    virtual std::unique_ptr<IPackIOProvider> createIO() = 0;
};

class ResourcePackRepository : public IResourcePackRepository, public IContentSourceRepository {
public:
    ResourcePackRepository(gsl::not_null<std::shared_ptr<RepositoryPacks>> repository_packs,
                           PackManifestFactory &manifest_factory,
                           const Bedrock::NotNullNonOwnerPtr<IContentAccessibilityProvider> &content_accessibility,
                           const Bedrock::NotNullNonOwnerPtr<Core::FilePathManager> &pathManager,
                           Bedrock::NonOwnerPointer<PackCommand::IPackCommandPipeline> commands,
                           PackSourceFactory &pack_source_factory, bool init_async,
                           std::unique_ptr<IRepositoryFactory> factory);

private:
    friend class endstone::core::EndstoneServer;

    ENDSTONE_HOOK void _initializePackSource();  // NOLINT

    Bedrock::NotNullNonOwnerPtr<Core::FilePathManager> file_path_manager_;
    gsl::not_null<std::shared_ptr<RepositoryPacks>> all_resource_packs_;
    gsl::not_null<std::unique_ptr<InvalidPacks>> invalid_packs_;
    gsl::not_null<std::unique_ptr<IRepositoryFactory>> factory_;
    gsl::not_null<std::shared_ptr<RepositorySources>> sources_;
    std::unique_ptr<PackSourceReport> pack_source_report_;
    gsl::not_null<std::unique_ptr<RepositoryLoading::VanillaPacks>> vanilla_packs_;
    std::vector<PackIdVersion> service_packs_;
    PackManifestFactory &manifest_factory_;
    Bedrock::NotNullNonOwnerPtr<IContentAccessibilityProvider> content_accessibility_;
    Core::HeapPathBuffer current_world_path_;
    Core::HeapPathBuffer current_premium_world_template_path_;
    ContentKeyMap temp_cache_content_keys_;
    std::unique_ptr<PackSettingsFactory> pack_settings_factory_;
    PackSourceFactory &pack_source_factory_;
    Bedrock::NonOwnerPointer<PackCommand::IPackCommandPipeline> commands_;
    std::unique_ptr<TaskGroup> task_group_;
    Bedrock::Threading::Mutex initialize_mutex_;
    ContentIdentity current_premium_world_template_identity_;
    gsl::not_null<std::unique_ptr<ResourcePackRepositoryRefreshQueue>> refresher_;
};
// TODO(fixme): check size
// BEDROCK_STATIC_ASSERT_SIZE(ResourcePackRepository, 576, 496);
