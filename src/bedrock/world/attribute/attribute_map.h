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
#include "bedrock/platform/brstd/flat_map.h"
#include "bedrock/world/attribute/attribute.h"
#include "bedrock/world/attribute/attribute_instance.h"
#include "bedrock/world/attribute/attribute_instance_handle.h"

class BaseAttributeMap {
    using UnderlyingMapContainer = brstd::flat_map<std::uint32_t, AttributeInstance>;

public:
    [[nodiscard]] const AttributeInstance &getInstance(const Attribute &attribute) const;
    [[nodiscard]] const AttributeInstance &getInstance(std::uint32_t id_value) const;
    void onAttributeModified(AttributeInstance const &instance);

    [[nodiscard]] const AttributeInstance *getInstance(const HashedString &name) const;                 // Endstone
    [[nodiscard]] MutableAttributeWithContext getMutableInstanceWithContext(const HashedString &name);  // Endstone
    [[nodiscard]] std::vector<MutableAttributeWithContext> getAttributes();

private:
    void _onAttributeModified(const AttributeInstance &instance);
    void _onAttributeModifiedDisabled(const AttributeInstance &instance);

    UnderlyingMapContainer instance_map_;
    std::vector<AttributeInstanceHandle> dirty_attributes_;
    void (BaseAttributeMap::*on_attribute_modified_)(const AttributeInstance &) =
        &BaseAttributeMap::_onAttributeModified;
};
