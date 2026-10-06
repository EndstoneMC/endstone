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
#include <vector>

#include "bedrock/core/resource/pack_id_version.h"
#include "bedrock/core/utility/enable_non_owner_references.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/forward.h"
#include "bedrock/resources/pack_access_strategy.h"
#include "bedrock/resources/pack_manifest.h"
#include "bedrock/resources/subpack_info_collection.h"

class IPackIOProvider {
public:
    virtual ~IPackIOProvider();
    [[nodiscard]] virtual Bedrock::NotNullNonOwnerPtr<IFileAccess> getFileAccess(ResourceFileSystem) const = 0;
    [[nodiscard]] virtual std::function<std::string(const Core::Path &)> getAssetReader() const = 0;
    [[nodiscard]] virtual Core::Path getOnDiskScratchPath() const = 0;
    [[nodiscard]] virtual std::unique_ptr<IPackIOProvider> clone() const = 0;
    [[nodiscard]] virtual std::shared_ptr<Core::FileHandlePool> getArchiveHandlePool() const = 0;
    [[nodiscard]] virtual I18n &getLoc() const = 0;
};

class Pack {
public:
    using UpgradeLegacyDependencies = std::vector<PackIdVersion>;
    ~Pack();
    [[nodiscard]] PackManifest const &getManifest() const;
    PackManifest &getManifest();

private:
    std::unique_ptr<PackManifest> manifest_;
    std::unique_ptr<PackAccessStrategy> access_strategy_;
    SubpackInfoCollection subpack_info_stack_;
    std::unique_ptr<PackMetadata> metadata_;
    std::unique_ptr<const std::vector<PackIdVersion>> dependencies_upgrade_;
    std::uint8_t revision_;
};

using NotNullPack = gsl::not_null<std::shared_ptr<Pack>>;
