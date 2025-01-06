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

#include "bedrock/resources/pack.h"

#include "endstone/runtime/symbol.h"

PackManifest const &Pack::getManifest() const
{
    return *manifest_;
}

PackManifest &Pack::getManifest()
{
    return *manifest_;
}

std::unique_ptr<Pack> Pack::createPack(ResourceLocation const &file_location, PackType type, PackOrigin origin,
                                       IPackManifestFactory &manifest_factory,
                                       Bedrock::NotNullNonOwnerPtr<const IContentKeyProvider> const &key_provider,
                                       PackSourceReport *report, Core::Path const &zip_sub_dir)
{
    return ENDSTONE_SYMCALL(&Pack::createPack, file_location, type, origin, manifest_factory, key_provider, report,
                            zip_sub_dir);
}
