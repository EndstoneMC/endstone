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

#include <string>

#include "endstone/identifier.h"

namespace endstone {
using AttributeModifierId = Identifier<class AttributeModifier>;

/**
 * Concrete implementation of an attribute modifier.
 */
class AttributeModifier {
public:
    /**
     * Operation to be applied.
     */
    enum class Operation {
        /**
         * Adds (or subtracts) the specified amount to the base value.
         */
        Add,
        /**
         * Multiplies the current value of the attribute by (1 + x),
         * where x is the sum of the modifiers' amounts.
         */
        MultiplyBase,
        /**
         * For every modifier, multiplies the current value of the attribute by (1 + x),
         * where x is the amount of the particular modifier.
         *
         * @note Functions the same as MultiplyBase if there is only a single modifier.
         *       However, for multiple modifiers it multiplies the modifiers rather than adding them.
         */
        Multiply,
        /**
         * Caps the value of the attribute at the modifier's amount.
         *
         * @note Bedrock-specific operation with no Bukkit equivalent.
         */
        Cap
    };

    /**
     * Construct a new attribute modifier.
     *
     * @param key the key of the modifier
     * @param amount the amount by which the modifier applies its operation
     * @param operation the operation to apply
     */
    AttributeModifier(AttributeModifierId key, float amount, Operation operation)
        : key_(std::string(key)), amount_(amount), operation_(operation)
    {
    }

    /**
     * Get the key of this modifier.
     *
     * @return key
     */
    [[nodiscard]] AttributeModifierId getKey() const { return AttributeModifierId{key_}; }

    /**
     * Get the amount by which this modifier will apply its operation.
     *
     * @return modification amount
     */
    [[nodiscard]] float getAmount() const { return amount_; }

    /**
     * Get the operation this modifier will apply.
     *
     * @return operation
     */
    [[nodiscard]] Operation getOperation() const { return operation_; }

private:
    std::string key_;
    float amount_;
    Operation operation_;
};
}  // namespace endstone
