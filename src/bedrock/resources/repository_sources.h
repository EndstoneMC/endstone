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

#include <memory>

#include "bedrock/resources/pack_source.h"
#include "bedrock/resources/pack_source_factory.h"

struct RepositorySourceOptions;

class RepositorySources {
public:
    RepositorySources(RepositorySourceOptions &&options);
    ENDSTONE_HOOK void initializePackSource(PackSourceFactory &pack_source_factory);

private:
    std::unique_ptr<const RepositorySourceOptions> options_;
    std::unique_ptr<CompositePackSource> pack_source_;
    std::unique_ptr<CompositePackSource> cache_pack_source_;
    std::unique_ptr<CompositePackSource> world_pack_source_;
    std::unique_ptr<CompositePackSource> premium_world_template_pack_source_;
    std::unique_ptr<CompositePackSource> temp_world_template_pack_source_;
};
