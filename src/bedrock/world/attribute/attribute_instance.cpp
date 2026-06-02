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

#include <algorithm>
#include <cmath>

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

std::vector<AttributeModifier> AttributeInstance::getModifiers() const
{
    // Returns every modifier, grouped by operation in ascending order (addition, multiply-base, multiply-total, cap).
    std::vector<AttributeModifier> modifiers;
    for (int operation = 0; operation < static_cast<int>(AttributeModifierOperation::TOTAL_OPERATIONS); ++operation) {
        for (const auto &modifier : modifier_list_) {
            if (static_cast<int>(modifier.getOperation()) == operation) {
                modifiers.push_back(modifier);
            }
        }
    }
    return modifiers;
}

float AttributeInstance::_calculateValue()
{
    // Start from the base (default) values; additions accumulate onto this base, which the multiply-base operation
    // reads from before any multiplications are applied.
    float base[3] = {default_values_[0], default_values_[1], default_values_[2]};
    for (const auto &modifier : modifier_list_) {
        if (modifier.getOperation() == AttributeModifierOperation::OPERATION_ADDITION) {
            base[static_cast<int>(modifier.getOperand())] += modifier.getAmount();
        }
    }

    float values[3] = {base[0], base[1], base[2]};
    for (const auto &modifier : modifier_list_) {
        if (modifier.getOperation() == AttributeModifierOperation::OPERATION_MULTIPLY_BASE) {
            const auto operand = static_cast<int>(modifier.getOperand());
            values[operand] += base[operand] * modifier.getAmount();
        }
    }
    for (const auto &modifier : modifier_list_) {
        if (modifier.getOperation() == AttributeModifierOperation::OPERATION_MULTIPLY_TOTAL) {
            const auto operand = static_cast<int>(modifier.getOperand());
            values[operand] *= 1.0F + modifier.getAmount();
        }
    }

    float result = values[2];
    if (modifier_list_.empty()) {
        if (attribute_->getRedefinitionMode() == RedefinitionMode::KeepCurrent) {
            result = current_value_;
        }
    }
    else if (values[2] == default_value_) {
        result = current_value_;
    }

    current_values_[0] = values[0];
    current_values_[1] = values[1];
    current_values_[2] = values[2];
    return _sanitizeValue(result);
}

float AttributeInstance::_sanitizeValue(float value)
{
    float cap = current_max_value_;
    for (const auto &modifier : modifier_list_) {
        if (modifier.getOperation() == AttributeModifierOperation::OPERATION_CAP) {
            cap = std::fmin(modifier.getAmount(), cap);
        }
    }
    if (value <= cap) {
        return std::fmax(current_min_value_, value);
    }
    return cap;
}

void AttributeInstance::addModifier(const AttributeModifier &modifier, AttributeModificationContext context)
{
    if (!modifier.isInstantaneous()) {
        // Skip if an equal modifier (same id, name, operation and operand) is already present.
        for (const auto &existing : modifier_list_) {
            if (existing == modifier) {
                return;
            }
        }
        modifier_list_.push_back(modifier);
    }
    current_value_ = _calculateValue();
    if (context.attribute_map) {
        context.attribute_map->onAttributeModified(*this);
    }
}

void AttributeInstance::removeModifier(const AttributeModifier &modifier, AttributeModificationContext context)
{
    std::erase_if(modifier_list_, [&](const AttributeModifier &existing) { return existing == modifier; });
    current_value_ = _calculateValue();
    if (context.attribute_map) {
        context.attribute_map->onAttributeModified(*this);
    }
}

bool AttributeInstance::removeModifier(const mce::UUID &id, AttributeModificationContext context)
{
    const auto it = std::ranges::find_if(modifier_list_,
                                         [&](const AttributeModifier &existing) { return existing.getId() == id; });
    if (it == modifier_list_.end()) {
        return false;
    }
    modifier_list_.erase(it);
    current_value_ = _calculateValue();
    if (context.attribute_map) {
        context.attribute_map->onAttributeModified(*this);
    }
    return true;
}
