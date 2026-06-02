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

#include "bedrock/world/attribute/attribute_map.h"

#include <stdexcept>

const AttributeInstance &BaseAttributeMap::getInstance(const Attribute &attribute) const
{
    return getInstance(attribute.getIDValue());
}

const AttributeInstance &BaseAttributeMap::getInstance(std::uint32_t id_value) const
{
    const auto it = instance_map_.find(id_value);
    if (it == instance_map_.end()) {
        throw std::runtime_error("Invalid attribute id.");
    }
    return (*it).second;
}

void BaseAttributeMap::onAttributeModified(AttributeInstance const &instance)
{
    std::invoke(on_attribute_modified_, this, instance);
}

const AttributeInstance *BaseAttributeMap::getInstance(const HashedString &name) const
{
    for (const auto [id, instance] : instance_map_) {
        if (instance.attribute_->getName() == name) {
            return &instance;
        }
    }
    throw std::runtime_error("Attribute not found by name: " + name.getString());
}

MutableAttributeWithContext BaseAttributeMap::getMutableInstanceWithContext(const HashedString &name)
{
    for (auto [id, instance] : instance_map_) {
        if (instance.attribute_->getName() == name) {
            return {&instance, this};
        }
    }
    throw std::runtime_error("Attribute not found by name: " + name.getString());
}

void BaseAttributeMap::_onAttributeModified(AttributeInstance const &instance)
{
    const auto *attribute = instance.getAttribute();
    if (!attribute || !attribute->isClientSyncable()) {
        return;
    }
    dirty_attributes_.push_back({attribute->getIDValue()});
}

void BaseAttributeMap::_onAttributeModifiedDisabled(const AttributeInstance &instance) {}

std::vector<MutableAttributeWithContext> BaseAttributeMap::getAttributes()
{
    std::vector<MutableAttributeWithContext> attributes;
    for (auto [id, instance] : instance_map_) {
        attributes.emplace_back(&instance, this);
    }
    return attributes;
}
