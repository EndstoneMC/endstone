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
#include <cstddef>
#include <memory>

#include "bedrock/platform/uuid.h"
#include "bedrock/world/attribute/attribute_buff.h"
#include "bedrock/world/attribute/attribute_modifier.h"

class AttributeInstance;
class BaseAttributeMap;
struct MutableAttributeWithContext;
struct ValidMutableAttributeWithContext;

struct AttributeModificationContext {
    AttributeModificationContext() = default;
    AttributeModificationContext(BaseAttributeMap *attribute_map) : attribute_map(attribute_map) {}
    AttributeModificationContext(const MutableAttributeWithContext);
    AttributeModificationContext(ValidMutableAttributeWithContext);
    BaseAttributeMap *attribute_map;
};
static_assert(sizeof(AttributeModificationContext) == 8);

struct AttributeInstanceForwarder {
    AttributeInstanceForwarder() = default;
    [[nodiscard]] float getCurrentValue() const;
    [[nodiscard]] float getDefaultValue(int operand) const;
    bool hasModifier(const mce::UUID &id) const;
    bool hasModifier(std::shared_ptr<AttributeModifier> modifier) const;
    bool hasModifier(const AttributeModifier &modifier) const;
    [[nodiscard]] bool hasTemporalBuffs() const;
    void setDefaultValue(float default_value, int operand);
    void setDefaultValueOnly(float new_default_value);
    void setRange(float min, float base, float max);
    void setMaxValue(float max);
    void setMinValue(float);
    void setCurrentValue(float value);
    void resetToMaxValue();
    void resetToMinValue();
    void resetToDefaultValue();
    void serializationSetValue(float current_value, int operand, float max_value);
    void serializationSetRange(float, float, float, float, float);
    void addBuff(const AttributeBuff &buff);
    void removeBuffs();
    void addModifiers(gsl::not_null<std::vector<std::shared_ptr<AttributeModifier>> *>);
    void addModifier(std::shared_ptr<AttributeModifier> modifier);
    void addModifier(const AttributeModifier &modifier);
    void updateModifier(const AttributeModifier &);
    bool removeModifier(const mce::UUID &id);
    void removeModifier(std::shared_ptr<AttributeModifier> modifier);
    void removeModifier(const AttributeModifier &modifier);
    void removeModifiers(const std::string &);
    void removeModifiers();
    void recalculateModifiers();
    void notify(AttributeMessageType);
    AttributeInstance *instance;
    AttributeModificationContext context;
};

struct MutableAttributeWithContext {
    MutableAttributeWithContext() = default;
    MutableAttributeWithContext(AttributeInstance *instance, AttributeModificationContext context);
    [[nodiscard]] bool isValid() const;
    explicit operator bool() const;
    bool operator==(std::nullptr_t) const;
    bool operator!=(std::nullptr_t) const;
    AttributeInstanceForwarder *operator->();
    AttributeInstanceForwarder &operator*();
    const AttributeInstanceForwarder *operator->() const;
    const AttributeInstanceForwarder &operator*() const;

    AttributeInstance *instance;
    AttributeModificationContext context;
};
static_assert(sizeof(MutableAttributeWithContext) == 16);
