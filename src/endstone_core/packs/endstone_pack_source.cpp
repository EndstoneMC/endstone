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

#include "endstone/detail/packs/endstone_pack_source.h"

#include <utility>

#include "endstone/detail/logger_factory.h"

namespace fs = std::filesystem;

namespace endstone::detail {

EndstonePackSource::EndstonePackSource(std::filesystem::path path, PackType pack_type)
    : path_(std::move(path)), pack_type_(pack_type)
{
}

void EndstonePackSource::forEachPackConst(ConstPackCallback callback) const
{
    for (const auto &pack : packs_) {
        callback(*pack);
    }
}

void EndstonePackSource::forEachPack(PackCallback callback)
{
    for (const auto &pack : packs_) {
        callback(*pack);
    }
}

PackOrigin EndstonePackSource::getPackOrigin() const
{
    return PackOrigin::User;
}

PackType EndstonePackSource::getPackType() const
{
    return pack_type_;
}

PackSourceReport EndstonePackSource::load(IPackManifestFactory &manifest_factory,
                                          const Bedrock::NotNullNonOwnerPtr<const IContentKeyProvider> &key_provider)
{
    if (discovered_) {
        return {};
    }

    // Server may not be initialized at this point. Let's get the logger directly from LoggerFactory.
    const auto &logger = LoggerFactory::getLogger("Server");
    logger.info("Loading resource packs...");

    PackSourceReport report;
    for (const auto &entry : fs::directory_iterator(path_)) {
        fs::path file;

        if (!is_regular_file(entry.status())) {
            continue;
        }

        file = entry.path();
        if (file.extension() == ".mcpack" || file.extension() == ".zip") {
            const auto file_location = ResourceLocation(Core::Path(file.string()), ResourceFileSystem::Raw);
            auto pack = Pack::createPack(file_location, getPackType(), getPackOrigin(), manifest_factory, key_provider,
                                         &report, Core::Path::EMPTY);
            packs_.emplace_back(std::move(pack));
        }
    }

    for (const auto &[pack_id, report] : report.getReports()) {
        if (report.hasErrors()) {
            logger.error("Could not load resource pack from '{}':",
                         report.getLocation().getRelativePath().getContainer());
            for (const auto &pack_error : report.getErrors()) {
                logger.error(pack_error->getLocErrorMessage());
            }
        }
    }

    discovered_ = true;
    return report;
}

}  // namespace endstone::detail
