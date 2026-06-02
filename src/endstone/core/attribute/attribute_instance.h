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

#include <cstdint>
#include <cstring>
#include <string>

#include <boost/uuid/name_generator_sha1.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>

#include "bedrock/platform/uuid.h"
#include "bedrock/world/attribute/attribute_instance.h"
#include "bedrock/world/attribute/mutable_attribute_with_context.h"
#include "endstone/attribute/attribute_instance.h"

namespace endstone::core {

class EndstoneAttributeInstance : public AttributeInstance {
public:
    explicit EndstoneAttributeInstance(::MutableAttributeWithContext handle) : handle_(handle) {}

    [[nodiscard]] AttributeId getType() const override
    {
        return handle_.instance->getAttribute()->getName().getString();
    }

    [[nodiscard]] float getBaseValue() const override
    {
        return handle_.instance->getDefaultValue(static_cast<int>(AttributeOperands::OPERAND_CURRENT));
    }

    void setBaseValue(float value) override
    {
        handle_.instance->setDefaultValue(value, static_cast<int>(AttributeOperands::OPERAND_CURRENT), handle_.context);
    }

    [[nodiscard]] float getMinValue() const override { return handle_.instance->getMinValue(); }

    [[nodiscard]] float getMaxValue() const override { return handle_.instance->getMaxValue(); }

    [[nodiscard]] float getValue() const override { return handle_.instance->getCurrentValue(); }

    [[nodiscard]] std::vector<AttributeModifier> getModifiers() const override
    {
        std::vector<AttributeModifier> modifiers;
        for (const auto &modifier : handle_.instance->getModifiers()) {
            modifiers.push_back(fromMinecraft(modifier));
        }
        return modifiers;
    }

    [[nodiscard]] std::optional<AttributeModifier> getModifier(AttributeModifierId id) const override
    {
        for (const auto &modifier : handle_.instance->getModifiers()) {
            if (AttributeModifierId{modifier.getName()} == id) {
                return fromMinecraft(modifier);
            }
        }
        return std::nullopt;
    }

    void addModifier(const AttributeModifier &modifier) override
    {
        handle_.instance->addModifier(toMinecraft(modifier, /*serializable=*/true), handle_.context);
    }

    void addTransientModifier(const AttributeModifier &modifier) override
    {
        handle_.instance->addModifier(toMinecraft(modifier, /*serializable=*/false), handle_.context);
    }

    void removeModifier(const AttributeModifier &modifier) override { removeModifier(modifier.getId()); }

    void removeModifier(AttributeModifierId id) override
    {
        handle_.instance->removeModifier(idToMinecraftUuid(id), handle_.context);
    }

private:
    static AttributeModifier::Operation fromMinecraft(AttributeModifierOperation operation)
    {
        switch (operation) {
        case AttributeModifierOperation::OPERATION_ADDITION:
            return AttributeModifier::Operation::Add;
        case AttributeModifierOperation::OPERATION_MULTIPLY_BASE:
            return AttributeModifier::Operation::MultiplyBase;
        case AttributeModifierOperation::OPERATION_MULTIPLY_TOTAL:
            return AttributeModifier::Operation::Multiply;
        case AttributeModifierOperation::OPERATION_CAP:
            return AttributeModifier::Operation::Cap;
        default:
            return AttributeModifier::Operation::Add;
        }
    }

    static AttributeModifierOperation toMinecraft(AttributeModifier::Operation operation)
    {
        switch (operation) {
        case AttributeModifier::Operation::Add:
            return AttributeModifierOperation::OPERATION_ADDITION;
        case AttributeModifier::Operation::MultiplyBase:
            return AttributeModifierOperation::OPERATION_MULTIPLY_BASE;
        case AttributeModifier::Operation::Multiply:
            return AttributeModifierOperation::OPERATION_MULTIPLY_TOTAL;
        case AttributeModifier::Operation::Cap:
            return AttributeModifierOperation::OPERATION_CAP;
        }
        return AttributeModifierOperation::OPERATION_ADDITION;
    }

    static AttributeModifier fromMinecraft(const ::AttributeModifier &modifier)
    {
        return {AttributeModifierId{modifier.getName()}, modifier.getAmount(), fromMinecraft(modifier.getOperation())};
    }

    // The modifier id is the public identity (Bukkit-style key). Bedrock identifies modifiers by UUID, so derive a
    // deterministic UUID from the id; the id string is also stored as the modifier name so it round-trips. Modifiers
    // created through the public API always target the current value (OPERAND_CURRENT) — the operand is a
    // Bedrock-internal addressing detail not exposed by the Bukkit-derived API. Persistence is chosen by the caller
    // (addModifier vs addTransientModifier), mirroring Bukkit.
    static ::AttributeModifier toMinecraft(const AttributeModifier &modifier, bool serializable)
    {
        const auto id = modifier.getId();
        return {idToMinecraftUuid(id),
                std::string(id),
                modifier.getAmount(),
                toMinecraft(modifier.getOperation()),
                AttributeOperands::OPERAND_CURRENT,
                serializable};
    }

    static ::mce::UUID idToMinecraftUuid(const AttributeModifierId &id)
    {
        // A fixed namespace keeps the derivation deterministic: the same id always maps to the same UUID.
        static const boost::uuids::uuid name_space =
            boost::uuids::string_generator{}("9b6c7f2e-3a4d-5e6f-8a1b-2c3d4e5f6a7b");
        static const boost::uuids::name_generator_sha1 generator{name_space};
        const boost::uuids::uuid uuid = generator(std::string(id));
        std::uint64_t high = 0;
        std::uint64_t low = 0;
        std::memcpy(&high, uuid.data, sizeof(high));
        std::memcpy(&low, uuid.data + sizeof(high), sizeof(low));
        return ::mce::UUID{high, low};
    }

    ::MutableAttributeWithContext handle_;
};

}  // namespace endstone::core
