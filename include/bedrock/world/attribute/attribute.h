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

#include <cstdint>

#include "bedrock/core/hashed_string.h"
#include "bedrock/world/attribute/attribute_collection.h"
#include "bedrock/world/attribute/redefinition_mode.h"

class Attribute {
public:
    [[nodiscard]] RedefinitionMode getRedefinitionMode() const
    {
        return redefinition_mode_;
    }

    [[nodiscard]] bool isClientSyncable() const
    {
        return syncable_;
    }

    [[nodiscard]] std::uint32_t getIDValue() const
    {
        return id_value_;
    }

    [[nodiscard]] const HashedString &getName() const
    {
        return name_;
    }

private:
    RedefinitionMode redefinition_mode_;  // +0
    bool syncable_;                       // +1
    std::uint32_t id_value_;              // +4
    HashedString name_;                   // +8
};
