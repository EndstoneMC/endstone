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
#include <utility>

#include "endstone/util/uuid.h"

namespace endstone {
/**
 * @brief Represents an attribute modifier.
 */
class AttributeModifier {
public:
    /**
     * @brief Operation to be applied.
     */
    enum class Operation {
        /**
         * @brief Adds (or subtracts) the specified amount to the base value.
         */
        Add,
        /**
         * @brief Multiplies the current value of the attribute by (1 + x),
         *        where x is the sum of the modifiers' amounts.
         */
        MultiplyBase,
        /**
         * @brief For every modifier, multiplies the current value of the attribute by (1 + x),
         *        where x is the amount of the particular modifier.
         *
         * @note Functions the same as MultiplyBase if there is only a single modifier.
         *       However, for multiple modifiers it multiplies the modifiers rather than adding them.
         */
        Multiply
    };

    AttributeModifier(std::string name, float amount, Operation operation)
        : name_(std::move(name)), amount_(amount), operation_(operation)
    {
    }

    AttributeModifier(std::string name, UUID uuid, float amount, Operation operation)
        : name_(std::move(name)), uuid_(uuid), amount_(amount), operation_(operation)
    {
    }

    /**
     * @brief Get the unique ID for this modifier.
     *
     * @return unique id
     */
    [[nodiscard]] UUID getUniqueId() const { return uuid_; }

    /**
     * @brief Get the name of this modifier.
     *
     * @return name
     */
    [[nodiscard]] std::string getName() const { return name_; }

    /**
     * @brief Get the amount by which this modifier will apply the operation.
     *
     * @return modification amount
     */
    [[nodiscard]] float getAmount() const { return amount_; }

    /**
     * @brief Get the operation this modifier will apply.
     *
     * @return operation
     */
    [[nodiscard]] Operation getOperation() const { return operation_; }

private:
    std::string name_;
    UUID uuid_;
    float amount_;
    Operation operation_;
};
}  // namespace endstone
