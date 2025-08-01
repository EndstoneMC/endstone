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

#include <functional>
#include <map>

#include "bedrock/core/utility/enable_non_owner_references.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/forward.h"
#include "bedrock/resources/content_key_provider.h"
#include "bedrock/resources/pack_access_strategy.h"
#include "bedrock/resources/pack_manifest.h"
#include "bedrock/resources/pack_manifest_factory.h"

class PackSourceReport;

class IPackIOProvider {
public:
    virtual ~IPackIOProvider();
    [[nodiscard]] virtual Bedrock::NotNullNonOwnerPtr<IFileAccess> getFileAccess(ResourceFileSystem) const = 0;
    [[nodiscard]] virtual std::function<std::string(const Core::Path &)> getAssetReader() const = 0;
    [[nodiscard]] virtual Core::Path getOnDiskScratchPath() const = 0;
    [[nodiscard]] virtual std::unique_ptr<IPackIOProvider> clone() const = 0;
};

struct LegacyDependenciesUpgrade {
    std::vector<PackIdVersion> new_dependencies;
};

class Pack : public Bedrock::EnableNonOwnerReferences {
public:
    [[nodiscard]] PackManifest const &getManifest() const;
    PackManifest &getManifest();

    static std::unique_ptr<Pack> createPack(const IPackIOProvider &io, const ResourceLocation &file_location,
                                            PackType type, PackOrigin origin, IPackManifestFactory &manifest_factory,
                                            Bedrock::NonOwnerPointer<const IContentKeyProvider> key_provider,
                                            PackSourceReport *report, const Core::Path &zip_sub_dir);

private:
    std::unique_ptr<PackManifest> manifest_;
    std::unique_ptr<PackAccessStrategy> access_strategy_;
    std::unique_ptr<void *> subpack_info_pack_;  // SubpackInfoCollection
    std::unique_ptr<void *> metadata_;           // PackMetadata
    std::unique_ptr<const LegacyDependenciesUpgrade> dependencies_upgrade_;
    // std::map<void *, std::function<void(Pack &)>> pack_updated_callbacks_;
    // std::map<void *, std::function<void(Pack &)>> pack_deleted_callbacks_;
};
