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

#include "bedrock/resources/resource_pack_repository_interface.h"

class ResourcePackRepository : public IResourcePackRepository {
public:
private:
    ENDSTONE_HOOK void _initializePackSource();  // NOLINT

    Bedrock::NotNullNonOwnerPtr<Core::FilePathManager> file_path_manager_;  // +24
    std::vector<std::unique_ptr<ResourcePack>> all_resource_packs_;         // +40
    std::unique_ptr<CompositePackSource> pack_source_;                      // +64
};
