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

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "bedrock/core/utility/enable_non_owner_references.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/resources/pack.h"
#include "bedrock/resources/pack_access_strategy.h"
#include "bedrock/resources/pack_error.h"
#include "bedrock/resources/resource_signature.h"

class ResourcePack {
public:
    [[nodiscard]] PackManifest &getManifest() const;

private:
    struct Impl {
        bool hidden_;
        bool error_;
        NotNullPack pack_;
        std::unique_ptr<PackAccessStrategy> subpack_access_strategy_;
        std::vector<std::shared_ptr<Pack>> sub_packs_;
        std::vector<std::shared_ptr<ResourcePack>> sub_resource_packs_;
        const std::string name_with_version_for_telemetry_;
        Core::HeapPathBuffer icon_path_;
        double load_time_;
        bool is_base_game_pack_;
        bool is_slice_pack_;
        ResourceSignature resource_signature_;
        bool is_marked_for_removal_;
        std::atomic<double> asset_read_ms_;
        std::atomic<std::uint64_t> asset_read_bytes_;
    };

    gsl::not_null<std::unique_ptr<Impl>> impl_;
};

using NotNullResourcePack = gsl::not_null<std::shared_ptr<ResourcePack>>;
