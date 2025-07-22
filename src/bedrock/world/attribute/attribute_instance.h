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

#include "bedrock/forward.h"
#include "bedrock/world/attribute/attribute.h"
#include "bedrock/world/attribute/attribute_buff.h"
#include "bedrock/world/attribute/mutable_attribute_with_context.h"

class BaseAttributeMap;

class AttributeInstance {
public:
    virtual ~AttributeInstance() = default;
    // virtual void tick() = 0;

    [[nodiscard]] float getCurrentValue() const;
    [[nodiscard]] float getMaxValue() const;
    void setCurrentValue(float value, AttributeModificationContext ctx);
    [[nodiscard]] Attribute *getAttribute() const;
    void addBuff(const AttributeBuff &, AttributeModificationContext);

private:
    friend class BaseAttributeMap;

    void _setDirty(AttributeModificationContext ctx);

    Attribute *attribute_;
    std::vector<void *> modifier_list_;
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
