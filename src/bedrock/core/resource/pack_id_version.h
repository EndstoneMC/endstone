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

#include "bedrock/core/resource/pack_type.h"
#include "bedrock/core/sem_ver/sem_version.h"
#include "bedrock/platform/uuid.h"

struct PackIdVersion {
    mce::UUID id;        // +0
    SemVersion version;  // +16
    PackType pack_type;  // +128

    bool operator==(const PackIdVersion &other) const
    {
        return id.data[0] == other.id.data[0] && id.data[1] == other.id.data[1] && version == other.version;
    }
};

template <>
struct std::hash<PackIdVersion> {
    std::size_t operator()(const PackIdVersion &value) const noexcept
    {
        static std::hash<std::string> hasher;
        return hasher(value.id.asString() + value.version.asString());
    }
};
