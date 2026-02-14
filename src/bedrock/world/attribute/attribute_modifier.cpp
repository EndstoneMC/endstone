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

#include "bedrock/world/attribute/attribute_modifier.h"

AttributeModifier::AttributeModifier(mce::UUID id, const std::string &name, float amount, int operation, int operands,
                                     bool serialize)
    : id_(id), name_(name), amount_(amount), operation_(static_cast<AttributeModifierOperation>(operation)),
      operands_(static_cast<AttributeOperands>(operands)), serialize_(serialize)
{
}

AttributeModifier::AttributeModifier(mce::UUID id, const std::string &name, float amount,
                                     AttributeModifierOperation operation, AttributeOperands operands, bool serialize)
    : id_(id), name_(name), amount_(amount), operation_(operation), operands_(operands), serialize_(serialize)
{
}

AttributeModifier::AttributeModifier() = default;

AttributeModifier::~AttributeModifier() = default;

bool AttributeModifier::isInstantaneous() const
{
    return false;
}

const mce::UUID &AttributeModifier::getId() const
{
    return id_;
}

const std::string &AttributeModifier::getName() const
{
    return name_;
}

AttributeModifierOperation AttributeModifier::getOperation() const
{
    return operation_;
}

AttributeOperands AttributeModifier::getOperand() const
{
    return operands_;
}

float AttributeModifier::getAmount() const
{
    return amount_;
}

bool AttributeModifier::isSerializable() const
{
    return serialize_;
}

void AttributeModifier::setSerialize(bool serialize)
{
    serialize_ = serialize;
}

bool AttributeModifier::operator==(const AttributeModifier &rhs) const
{
    return id_ == rhs.id_ && name_ == rhs.name_ && operation_ == rhs.operation_ && operands_ == rhs.operands_;
}

bool AttributeModifier::operator!=(const AttributeModifier &rhs) const
{
    return !(*this == rhs);
}

AttributeModifier &AttributeModifier::operator=(const AttributeModifier &rhs) = default;
