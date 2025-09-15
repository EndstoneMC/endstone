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
#include "pack_source_factory.h"

struct RepositorySourceOptions;

class RepositorySources {
public:
    PackSource *getPackSource();
    PackSource *getPremiumWorldTemplatePackSource();
    const Core::Path &getOnDiskScratchPath() const;
    const Core::Path &getDevelopmentResourcePacksPath() const;
    const Core::Path &getDevelopmentBehaviorPacksPath() const;
    const Core::Path &getDevelopmentSkinPacksPath() const;
    const Core::Path &getTreatmentPacksPath() const;
    const Core::Path &getResourcePacksPath() const;
    const Core::Path &getBehaviorPacksPath() const;
    const Core::Path &getPremiumWorldTemplatePath() const;
    const Core::Path &getPremiumBehaviorPackPath() const;
    const Core::Path &getPremiumResourcePackPath() const;
    const Core::Path &getPremiumPackPath() const;
    const Core::Path &getSystemServicePackPath() const;
    const Core::Path &getTemporaryPremiumResourcePacksPath() const;
    const Core::Path &getTemporaryPremiumBehaviorPacksPath() const;
    const Core::Path &getCachedResourcePacksPath() const;
    const Core::Path &getCachedBehaviorPacksPath() const;
    bool saveEncryptedWorldTemplatePacksAsZips() const;
    std::unique_ptr<const RepositorySourceOptions> options;
    std::unique_ptr<CompositePackSource> pack_source;
    std::unique_ptr<CompositePackSource> cache_pack_source;
    std::unique_ptr<CompositePackSource> world_pack_source;
    std::unique_ptr<CompositePackSource> premium_world_template_pack_source;
    std::unique_ptr<CompositePackSource> temp_world_template_pack_source;
};
