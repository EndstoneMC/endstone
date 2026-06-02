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

#include <memory>
#include <vector>

#include "bedrock/world/attribute/attribute.h"
#include "bedrock/world/attribute/attribute_buff.h"
#include "bedrock/world/attribute/attribute_modifier.h"
#include "bedrock/world/attribute/mutable_attribute_with_context.h"

class AttributeInstanceDelegate;
class BaseAttributeMap;

class AttributeInstance {
public:
    virtual ~AttributeInstance() = default;
    // virtual void tick() = 0;

    [[nodiscard]] const Attribute *getAttribute() const;
    [[nodiscard]] float getMaxValue() const;
    [[nodiscard]] float getMinValue() const;
    [[nodiscard]] float getDefaultValue(int operand) const;
    void setDefaultValue(float default_value, int operand, AttributeModificationContext context);
    void setDefaultValueOnly(float new_default_value, AttributeModificationContext context);
    [[nodiscard]] float getCurrentValue() const;
    void setMaxValue(float max, AttributeModificationContext context);
    void setMinValue(float min, AttributeModificationContext context);
    void setCurrentValue(float value, AttributeModificationContext context);
    void addBuff(const AttributeBuff &, AttributeModificationContext);
    void removeBuff(const AttributeBuff &buff);
    void addModifier(const AttributeModifier &modifier, AttributeModificationContext context);
    void removeModifier(const AttributeModifier &modifier, AttributeModificationContext context);  // todo

private:
    friend class BaseAttributeMap;

    void _setDirty(AttributeModificationContext context);
    float _calculateValue();
    float _sanitizeValue(float);

    Attribute *attribute_;

public:
    std::vector<AttributeModifier> modifier_list;

private:
    std::vector<void *> temporal_buffs_;
    std::vector<void *> listeners_;
    std::shared_ptr<AttributeInstanceDelegate> delegate_;
    union {
        float default_values_[3];
        struct {
            float default_min_value_;
            float default_max_value_;
            float default_value_;
        };
    };
    union {
        float current_values_[3];
        struct {
            float current_min_value_;
            float current_max_value_;
            float current_value_;
        };
    };
};
static_assert(sizeof(AttributeInstance) == 128);
