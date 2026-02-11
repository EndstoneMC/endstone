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

#include "bedrock/core/threading/async.h"
#include "bedrock/resources/invalid_packs_filter_group.h"
#include "bedrock/resources/pack_source.h"
#include "bedrock/resources/pack_source_factory.h"
#include "bedrock/resources/resource_pack_stack.h"

class IResourcePackRepository : public Bedrock::EnableNonOwnerReferences {
public:
    virtual void getResourcePacksByPackId(std::vector<PackInstanceId> const &, std::vector<PackInstance> &) const = 0;
    [[nodiscard]] virtual std::shared_ptr<ResourcePack> getResourcePackForPackId(PackIdVersion const &) const = 0;
    [[nodiscard]] virtual ResourcePack *getResourcePackOfDifferentVersionForPackId(const PackIdVersion &) const = 0;
    [[nodiscard]] virtual ResourcePack *getResourcePackForPackIdInPath(PackIdVersion const &,
                                                                       Core::Path const &) const = 0;
    [[nodiscard]] virtual ResourcePack *getResourcePackByUUID(mce::UUID const &) const = 0;
    [[nodiscard]] virtual ResourcePack *getResourcePackForPackIdOwned(PackIdVersion const &) const = 0;
    [[nodiscard]] virtual std::shared_ptr<ResourcePack> getResourcePackSatisfiesPackId(PackIdVersion const &,
                                                                                       bool) const = 0;
    [[nodiscard]] virtual ResourcePack *getResourcePackContainingModule(PackIdVersion const &) const = 0;
    [[nodiscard]] virtual Bedrock::Threading::Async<std::shared_ptr<ResourcePack>> getResourcePackInPath(
        Core::Path const &) const = 0;
    virtual bool isResourcePackLoaded(PackIdVersion const &, PackOrigin const &) = 0;
    [[nodiscard]] virtual PackSourceReport const *getPackLoadingReport() const = 0;
    [[nodiscard]] virtual std::shared_ptr<ResourcePack> getEditorPack() const = 0;
    [[nodiscard]] virtual std::shared_ptr<ResourcePack> getVanillaPack() const = 0;
    virtual bool setServicePacks(std::vector<PackIdVersion>) = 0;
    [[nodiscard]] virtual bool hasServicePacks(std::vector<PackIdVersion> const &) const = 0;
    [[nodiscard]] virtual std::vector<PackIdVersion> const &getServicePacks() const = 0;
    virtual void addServicePacksToStack(ResourcePackStack &) const = 0;
    virtual void addCachedResourcePacks(ContentKeyMap const *) = 0;
    virtual void addWorldResourcePacks(Core::Path const &) = 0;
    virtual void addPremiumWorldTemplateResourcePacks(Core::Path const &, ContentIdentity const &) = 0;
    virtual void addWorldPackSource(const Core::Path &) = 0;
    virtual void addPremiumWorldTemplatePackSource(const Core::Path &, const ContentIdentity &) = 0;
    virtual void addTempWorldTemplateResourcePacks(mce::UUID const &) = 0;
    virtual void removePacksLoadedFromCache() = 0;
    virtual void removePacksLoadedFromWorld() = 0;
    [[nodiscard]] virtual Core::HeapPathBuffer getResourcePacksPath() const = 0;
    [[nodiscard]] virtual Core::HeapPathBuffer getBehaviorPacksPath() const = 0;
    [[nodiscard]] virtual Core::HeapPathBuffer getSkinPacksPath() const = 0;
    [[nodiscard]] virtual Core::HeapPathBuffer getCustomSkinDirectoryPath() const = 0;
    [[nodiscard]] virtual Core::HeapPathBuffer getDevelopmentResourcePacksPath() const = 0;
    [[nodiscard]] virtual Core::HeapPathBuffer getDevelopmentBehaviorPacksPath() const = 0;
    [[nodiscard]] virtual Core::HeapPathBuffer getDevelopmentSkinPacksPath() const = 0;
    [[nodiscard]] virtual Core::HeapPathBuffer getTreatmentPacksPath() const = 0;
    virtual void refreshPacks() = 0;
    virtual Bedrock::Threading::Async<void> refreshPacksAsync() = 0;
    virtual void requestReloadUserPacks() = 0;
    [[nodiscard]] virtual Bedrock::NotNullNonOwnerPtr<const IContentKeyProvider> getKeyProvider() const = 0;
    virtual PackManifestFactory &getPackManifestFactory() = 0;
    [[nodiscard]] virtual PackSettingsFactory &getPackSettingsFactory() const = 0;
    virtual PackSourceFactory &getPackSourceFactory() = 0;
    // [[nodiscard]] virtual CompositePackSource const *getWorldPackSource() const = 0;
    [[nodiscard]] virtual std::vector<ResourcePack *> getPacksByResourceLocation(PackOrigin) const = 0;
    [[nodiscard]] virtual std::vector<ResourcePack *> getPacksByType(PackType) const = 0;
    [[nodiscard]] virtual std::vector<gsl::not_null<std::shared_ptr<ResourcePack>>> getPacksByCategory(
        PackCategory) = 0;
    virtual void forEachPack(const std::function<void(const ResourcePack &)> &) const = 0;
    [[nodiscard]] virtual std::vector<ResourceLocation> const &getInvalidPacks(PackType) const = 0;
    [[nodiscard]] virtual std::vector<ResourceLocation> getInvalidPacks(InvalidPacksFilterGroup const &) const = 0;
    virtual void deletePack(ResourceLocation const &) = 0;
    virtual void deletePackFiles(ResourceLocation const &) = 0;
    virtual void postDeletePack(ResourceLocation const &) = 0;
    virtual void untrackInvalidPack(ResourceLocation const &) = 0;
    [[nodiscard]] virtual bool isInitialized() const = 0;
};
