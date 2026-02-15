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

#include <vector>

#include "endstone/attribute/attribute.h"
#include "endstone/attribute/attribute_modifier.h"

namespace endstone {
/**
 * @brief Represents a mutable instance of an attribute and its associated modifiers and values.
 */
class AttributeInstance {
public:
    virtual ~AttributeInstance() = default;

    /**
     * @brief The attribute type pertaining to this instance.
     *
     * @return the attribute type
     */
    [[nodiscard]] virtual AttributeId getType() const = 0;

    /**
     * @brief Base value of this instance before modifiers are applied.
     *
     * @return base value
     */
    [[nodiscard]] virtual float getBaseValue() const = 0;

    /**
     * @brief Set the base value of this instance.
     *
     * @param value new base value
     */
    virtual void setBaseValue(float value) = 0;

    /**
     * @brief Get the value of this instance after all associated modifiers have been applied.
     *
     * @return the total attribute value
     */
    [[nodiscard]] virtual float getValue() const = 0;

    /**
     * @brief Get all modifiers present on this instance.
     *
     * @return a copied collection of all modifiers
     */
    [[nodiscard]] virtual std::vector<AttributeModifier> getModifiers() const = 0;

    /**
     * @brief Add a modifier to this instance.
     *
     * @param modifier to add
     */
    virtual void addModifier(const AttributeModifier &modifier) = 0;

    /**
     * @brief Remove a modifier from this instance.
     *
     * @param modifier to remove
     */
    virtual void removeModifier(const AttributeModifier &modifier) = 0;
};
}  // namespace endstone
