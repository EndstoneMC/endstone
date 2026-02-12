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

#include "endstone/attribute/attribute_instance.h"

#include "bedrock/world/attribute/attribute_instance.h"
#include "bedrock/world/attribute/mutable_attribute_with_context.h"
#include "endstone/core/util/uuid.h"

namespace endstone {

class EndstoneAttributeInstance : public AttributeInstance {
public:
    explicit EndstoneAttributeInstance(::MutableAttributeWithContext &handle) : handle_(handle) {}

    [[nodiscard]] AttributeId getType() const override
    {
        return handle_.instance->getAttribute()->getName().getString();
    }

    [[nodiscard]] float getBaseValue() const override { return handle_.instance->getDefaultValue(2); }

    void setBaseValue(float value) override { handle_.instance->setDefaultValue(value, 2, handle_->context); }

    [[nodiscard]] float getValue() const override { return handle_.instance->getCurrentValue(); }

    [[nodiscard]] std::vector<AttributeModifier> getModifiers() const override
    {
        std::vector<AttributeModifier> modifiers;
        for (auto &modifier : handle_.instance->modifier_list) {
            AttributeModifier::Operation operation;
            switch (modifier.getOperation()) {
            case AttributeModifierOperation::OPERATION_ADDITION:
                operation = AttributeModifier::Operation::Add;
                break;
            case AttributeModifierOperation::OPERATION_MULTIPLY_BASE:
                operation = AttributeModifier::Operation::MultiplyBase;
                break;
            case AttributeModifierOperation::OPERATION_MULTIPLY_TOTAL:
                operation = AttributeModifier::Operation::Multiply;
                break;
            default:
                break;
            }
            modifiers.emplace_back(modifier.getName(), modifier.getAmount(), operation);
        }
        return modifiers;
    }

    void addModifier(const AttributeModifier &modifier) override
    {

        AttributeModifierOperation operation;
        switch (modifier.getOperation()) {
        case AttributeModifier::Operation::Add:
            operation = AttributeModifierOperation::OPERATION_ADDITION;
            break;
        case AttributeModifier::Operation::MultiplyBase:
            operation = AttributeModifierOperation::OPERATION_MULTIPLY_BASE;
            break;
        case AttributeModifier::Operation::Multiply:
            operation = AttributeModifierOperation::OPERATION_MULTIPLY_TOTAL;
            break;
        }
        ::AttributeModifier vanilla{endstone::core::EndstoneUUID::toMinecraft(modifier.getUniqueId()),
                                    modifier.getName(),
                                    modifier.getAmount(),
                                    operation,
                                    AttributeOperands::OPERAND_CURRENT,
                                    true};
        handle_.instance->addModifier(vanilla, handle_.context);
    }

    void removeModifier(const AttributeModifier &modifier) override
    {
        AttributeModifierOperation operation;
        switch (modifier.getOperation()) {
        case AttributeModifier::Operation::Add:
            operation = AttributeModifierOperation::OPERATION_ADDITION;
            break;
        case AttributeModifier::Operation::MultiplyBase:
            operation = AttributeModifierOperation::OPERATION_MULTIPLY_BASE;
            break;
        case AttributeModifier::Operation::Multiply:
            operation = AttributeModifierOperation::OPERATION_MULTIPLY_TOTAL;
            break;
        }
        ::AttributeModifier vanilla{endstone::core::EndstoneUUID::toMinecraft(modifier.getUniqueId()),
                                    modifier.getName(),
                                    modifier.getAmount(),
                                    operation,
                                    AttributeOperands::OPERAND_CURRENT,
                                    true};
        handle_.instance->removeModifier(vanilla, handle_.context);
    }

private:
    ::MutableAttributeWithContext &handle_;
};

}  // namespace endstone
