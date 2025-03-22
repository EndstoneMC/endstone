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

#include <string>

#include "bedrock/core/resource/content_identity.h"
#include "bedrock/core/resource/pack_id_version.h"
#include "bedrock/network/packet.h"

struct PackInfoData {
    PackIdVersion m_pack_id_version;
    std::uint64_t pack_size;
    std::string content_key;
    std::string subpack_name;
    ContentIdentity content_identity;
    bool has_scripts;
    bool is_addon_pack;
    bool is_ray_tracing_capable;
    bool has_exceptions;
    std::string cdn_url;
};

struct PacksInfoData {
    bool resource_pack_required;
    bool has_addon_packs;
    bool has_scripts;
    PackIdVersion world_template_id_and_version;
    std::vector<PackInfoData> resource_packs;
};
static_assert(sizeof(PacksInfoData) == 80);

class ResourcePacksInfoPacket : public Packet {
public:
    PacksInfoData data;
};
static_assert(sizeof(ResourcePacksInfoPacket) == 128);
