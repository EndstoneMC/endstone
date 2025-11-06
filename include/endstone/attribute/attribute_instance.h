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
    [[nodiscard]] virtual std::string getType() const = 0;

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
     * @brief Base max value of this instance before modifiers are applied.
     *
     * @return base max value
     */
    [[nodiscard]] virtual float getBaseMaxValue() const = 0;

    /**
     * @brief Set the base max value of this instance.
     *
     * @param value new base max value
     */
    virtual void setBaseMaxValue(float value) = 0;

    /**
     * @brief Base min value of this instance before modifiers are applied.
     *
     * @return base max value
     */
    [[nodiscard]] virtual float getBaseMinValue() const = 0;

    /**
     * @brief Set the base min value of this instance.
     *
     * @param value new base min value
     */
    virtual void setBaseMinValue(float value) = 0;

    /**
     * Get the value of this instance after all associated modifiers have been applied.
     *
     * @return the total attribute value
     */
    [[nodiscard]] virtual float getValue() const = 0;

    /**
     * Get the max value of this instance after all associated modifiers have been applied.
     *
     * @return the max attribute value
     */
    [[nodiscard]] virtual float getMaxValue() const = 0;

    /**
     * Get the min value of this instance after all associated modifiers have been applied.
     *
     * @return the min attribute value
     */
    [[nodiscard]] virtual float getMinValue() const = 0;

    // TODO(attribute): add modifiers
};
}  // namespace endstone
