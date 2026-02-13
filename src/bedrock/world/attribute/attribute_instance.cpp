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

#include "bedrock/world/attribute/attribute_instance_delegate.h"
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

void AttributeInstance::setMaxValue(float max, AttributeModificationContext context)
{
    current_max_value_ = max;
    _setDirty(context);
}

void AttributeInstance::setMinValue(float min, AttributeModificationContext context)
{
    current_min_value_ = min;
    _setDirty(context);
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

float AttributeInstance::_calculateValue()
{
    for (const auto &modifier : modifier_list) {
        if (modifier.getOperation() == AttributeModifierOperation::OPERATION_ADDITION) {
            int operand = static_cast<int>(modifier.getOperand());
            current_values_[operand] += modifier.getAmount();
        }
    }
    for (const auto &modifier : modifier_list) {
        if (modifier.getOperation() == AttributeModifierOperation::OPERATION_MULTIPLY_BASE) {
            int operand = static_cast<int>(modifier.getOperand());
            current_values_[operand] *= modifier.getAmount();
        }
    }
    for (const auto &modifier : modifier_list) {
        if (modifier.getOperation() == AttributeModifierOperation::OPERATION_MULTIPLY_TOTAL) {
            int operand = static_cast<int>(modifier.getOperand());
            current_values_[operand] *= (1.0F + modifier.getAmount());
        }
    }
    // todo(attribute): check logic
    // if ((current_value_ == default_value_ && !modifier_list.empty()) ||
    //     (modifier_list.empty() && !attribute_->isClientSyncable())) {
    //     current_value_ = default_value_;
    // }
    return _sanitizeValue(current_value_);
}

float AttributeInstance::_sanitizeValue(float value)
{
    float cap_limit = current_max_value_;
    for (const auto &modifier : modifier_list) {
        if (modifier.getOperation() == AttributeModifierOperation::OPERATION_CAP) {
            float amount = modifier.getAmount();
            cap_limit = std::min(cap_limit, amount);
        }
    }
    float result = std::max(value, current_min_value_);
    result = std::min(result, cap_limit);
    return result;
}

void AttributeInstance::addModifier(const AttributeModifier &modifier, AttributeModificationContext context)
{
    if (modifier.isInstantaneous()) {
        return;
    }
    for (const auto &existing_modifier : modifier_list) {
        if (existing_modifier == modifier) {
            return;
        }
    }
    modifier_list.push_back(modifier);
    float new_value = _calculateValue();
    current_value_ = new_value;
    _setDirty(context);
}

void AttributeInstance::removeModifier(const AttributeModifier &modifier, AttributeModificationContext context)
{
    auto begin = modifier_list.begin();
    auto end = modifier_list.end();
    for (auto it = begin; it != end; ++it) {
        if (*it != modifier) {
            continue;
        }
        modifier_list.erase(it);
        float new_value = _calculateValue();
        current_value_ = new_value;
        _setDirty(context);
        if (delegate_) {
            // todo(attribute): check logic
            delegate_->notify(0, context);
        }
        break;
    }
}
