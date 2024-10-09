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

#include "bedrock/forward.h"
#include "bedrock/resources/pack_access_strategy.h"

class Pack : public Bedrock::EnableNonOwnerReferences {
public:
private:
    std::unique_ptr<PackManifest> manifest_;
    std::unique_ptr<PackAccessStrategy> access_strategy_;
    std::unique_ptr<SubpackInfoCollection> subpack_info_pack_;
    std::unique_ptr<PackMetadata> metadata_;
    std::map<void *, std::function<void(Pack &)>> pack_updated_callbacks_;
    std::map<void *, std::function<void(Pack &)>> pack_deleted_callbacks_;
};
