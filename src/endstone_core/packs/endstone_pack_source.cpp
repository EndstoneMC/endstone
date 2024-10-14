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

#include <spdlog/spdlog.h>

namespace endstone::detail {

EndstonePackSource::EndstonePackSource(PackType pack_type) : pack_type_(pack_type) {}

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

PackSourceReport EndstonePackSource::load(IPackManifestFactory &factory,
                                          const Bedrock::NotNullNonOwnerPtr<const IContentKeyProvider> &)
{
    // TODO:
    return {};
}

}  // namespace endstone::detail
