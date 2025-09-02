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

enum class ManifestOrigin : char {
    Directory = 0,
    Archive = 1,
    Realms = 2,
    Catalog = 3,
    WorldHistory = 4,
    Invalid = 5,
};

enum class ManifestType : char {
    Pack = 0,
    WorldTemplate = 1,
    Catalog = 2,
};

enum class PackScope : uint8_t {
    Global = 1,
    AddOn = 2,
    Level = 4,
    System = 8,
    World = 6,
    Any = 7,
    Default = 7,
};

enum class PackManifestFormat : uint8_t {
    V0 = 0,
    V1 = 1,
    V2 = 2,
    V3 = 3,
};
class PackManifest : public Bedrock::EnableNonOwnerReferences {
public:
    PackManifest(ManifestType type) : manifest_type_(type) {}

    [[nodiscard]] PackIdVersion const &getIdentity() const;
    [[nodiscard]] std::string getName() const;

private:
    ManifestType manifest_type_;
    ManifestOrigin manifest_origin_;
    ResourceLocation location_;
    ResourceLocation pack_icon_location_;
    ContentIdentity content_identity_;
    ContentIdentity source_identity_;
    PackCategory pack_category_;
    PackOrigin pack_origin_;
    bool is_title_locked_;
    bool expired_;
    uint64_t size_;
    std::string last_modified_date_;
    std::vector<std::string> language_codes_for_pack_keywords_;
    PackManifestFormat format_version_;
    PackManifestFormat original_format_version_;
    std::string name_;
    std::string description_;
    PackIdVersion identity_;
    PackScope pack_scope_;
    PackType pack_type_;
    BaseGameVersion required_base_game_version_;
    MinEngineVersion min_engine_version_;
    // ...
};
