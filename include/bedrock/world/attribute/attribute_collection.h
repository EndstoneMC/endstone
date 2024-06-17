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

#include <unordered_map>

#include "bedrock/bedrock.h"
#include "bedrock/core/hashed_string.h"

class Attribute;
class AttributeCollection {
public:
    static Attribute *getAttribute(const HashedString &name)
    {
        auto &inst = instance();
        auto it = inst.attributes_map_.find(name);
        if (it == inst.attributes_map_.end()) {
            return nullptr;
        }
        return it->second;
    }

    static bool hasAttribute(const HashedString &name)
    {
        auto &inst = instance();
        return inst.attributes_map_.find(name) != inst.attributes_map_.end();
    }

private:
    friend class Attribute;
    ENDSTONE_HOOK static AttributeCollection &instance();

    std::unordered_map<HashedString, Attribute *> attributes_map_;
    std::uint32_t id_value_index_;
};
