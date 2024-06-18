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

#include <system_error>
#include <unordered_map>

#include "bedrock/bedrock.h"
#include "bedrock/world/attribute/attribute.h"
#include "bedrock/world/attribute/attribute_instance.h"

class BaseAttributeMap {
public:
    [[nodiscard]] const AttributeInstance &getInstance(const Attribute &attribute) const
    {
        return getInstance(attribute.getIDValue());
    }

    [[nodiscard]] const AttributeInstance &getInstance(std::uint32_t id_value) const
    {
        auto it = instance_map_.find(id_value);
        if (it == instance_map_.end()) {
            throw std::runtime_error("Invalid attribute id.");
        }
        return it->second;
    }

private:
    std::unordered_map<std::uint32_t, AttributeInstance> instance_map_;  // +0
    std::vector<void *> dirty_attributes_;  //+64 (+40) std::vector<AttributeInstanceHandle>
};
BEDROCK_STATIC_ASSERT_SIZE(BaseAttributeMap, 88, 64);
