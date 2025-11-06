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

#include "bedrock/world/attribute/attribute_instance.h"

#include "bedrock/world/attribute/attribute_map.h"

const Attribute *AttributeInstance::getAttribute() const
{
    return attribute_;
}

float AttributeInstance::getMaxValue() const
{
    return current_max_value_;
}

float AttributeInstance::getMinValue() const
{
    return current_min_value_;
}

float AttributeInstance::getDefaultValue(int operand) const
{
    if (operand <= 2) {
        return default_values_[operand];
    }
    return 0.0;
}

void AttributeInstance::setDefaultValue(float default_value, int operand, AttributeModificationContext context)
{
    if (operand <= 2 && default_values_[operand] != default_value) {
        default_values_[operand] = default_value;
        current_values_[operand] = default_value;
        _setDirty(context);
    }
}

void AttributeInstance::setDefaultValueOnly(float new_default_value, AttributeModificationContext context)
{
    if (default_value_ != new_default_value) {
        default_value_ = new_default_value;
        _setDirty(context);
    }
}

float AttributeInstance::getCurrentValue() const
{
    return current_value_;
}

void AttributeInstance::setCurrentValue(float value, AttributeModificationContext context)
{
    current_value_ = value;
    _setDirty(context);
}

void AttributeInstance::_setDirty(AttributeModificationContext context)
{
    if (context.attribute_map) {
        context.attribute_map->onAttributeModified(*this);
    }
}
