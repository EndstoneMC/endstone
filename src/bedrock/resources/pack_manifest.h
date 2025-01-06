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

#include "bedrock/core/resource/content_identity.h"
#include "bedrock/core/resource/pack_id_version.h"
#include "bedrock/core/resource/resource_helper.h"
#include "bedrock/core/utility/enable_non_owner_references.h"
#include "bedrock/resources/base_game_version.h"
#include "bedrock/resources/min_engine_version.h"
#include "bedrock/resources/pack_capabililty.h"

class PackManifest : public Bedrock::EnableNonOwnerReferences {
public:
    [[nodiscard]] PackIdVersion const &getIdentity() const;
    [[nodiscard]] std::string getName() const;

private:
    ResourceLocation location_;
    PackIdVersion identity_;
    ContentIdentity content_identity_;
    MinEngineVersion min_engine_version_;
    BaseGameVersion required_base_game_version_;
    std::unordered_map<std::string, PackCapability::TrustLevel> capabilities_;
    std::string name_;
};
