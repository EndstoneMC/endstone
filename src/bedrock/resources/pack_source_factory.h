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

#include "bedrock/core/utility/pub_sub/subscription.h"
#include "bedrock/platform/threading/mutex_details.h"
#include "bedrock/resources/pack_command_pipeline.h"
#include "bedrock/resources/pack_source.h"

class IPackSourceFactory : public Bedrock::EnableNonOwnerReferences {
public:
    ~IPackSourceFactory() override = default;
};

class PackSourceFactory : public IPackSourceFactory {
public:
    struct RealmsUnknownPackSources {
        std::unique_ptr<RealmsUnknownPackSource> realms_unknown_resource_pack_source;
        std::unique_ptr<RealmsUnknownPackSource> realms_unknown_behavior_pack_source;
    };

    [[nodiscard]] std::unique_ptr<IPackIOProvider> createPackIOProvider() const  // Endstone
    {
        return io->clone();
    }

private:
    template <typename PackSourceT>
    struct SourcesList {
        std::vector<PackSourceT> sources;
        Bedrock::Threading::RecursiveMutex mutex;
    };

    std::vector<Bedrock::PubSub::Subscription> subscriptions_;
    SourcesList<std::shared_ptr<ContentCatalogPackSource>> content_catalog_pack_sources_;
    SourcesList<std::unique_ptr<TreatmentPackSource>> treatment_pack_sources_;
    SourcesList<std::unique_ptr<DirectoryPackSource>> directory_pack_sources_;
    SourcesList<std::unique_ptr<InPackagePackSource>> in_package_pack_sources_;
    SourcesList<std::unique_ptr<WorldHistoryPackSource>> world_history_pack_sources_;
    SourcesList<std::unique_ptr<WorldTemplatePackSource>> world_template_pack_sources_;
    RealmsUnknownPackSources realms_unknown_pack_sources_;
    std::shared_ptr<IInPackagePacks> in_package_packs_provider_;
    const std::unique_ptr<IPackIOProvider> io;
    Bedrock::NonOwnerPointer<PackCommand::IPackCommandPipeline> commands_;
};
