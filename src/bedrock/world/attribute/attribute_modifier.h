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

#include "bedrock/platform/uuid.h"
#include "bedrock/world/attribute/attribute.h"

enum class AttributeModifierOperation : int {
    OPERATION_ADDITION = 0,
    OPERATION_MULTIPLY_BASE = 1,
    OPERATION_MULTIPLY_TOTAL = 2,
    OPERATION_CAP = 3,
    TOTAL_OPERATIONS = 4,
    OPERATION_INVALID = 4,
};

class AttributeModifier {
public:
    AttributeModifier(mce::UUID, const std::string &, float, int, int, bool);
    AttributeModifier(mce::UUID, const std::string &, float, AttributeModifierOperation, AttributeOperands, bool);
    AttributeModifier();
    virtual ~AttributeModifier();
    [[nodiscard]] virtual bool isInstantaneous() const;

    [[nodiscard]] const mce::UUID &getId() const;
    [[nodiscard]] const std::string &getName() const;
    [[nodiscard]] AttributeModifierOperation getOperation() const;
    [[nodiscard]] AttributeOperands getOperand() const;
    [[nodiscard]] float getAmount() const;
    [[nodiscard]] bool isSerializable() const;
    void setSerialize(bool);
    bool operator==(const AttributeModifier &rhs) const;
    bool operator!=(const AttributeModifier &) const;
    AttributeModifier &operator=(const AttributeModifier &rhs);

private:
    float amount_;
    AttributeModifierOperation operation_;
    AttributeOperands operands_;
    std::string name_;
    mce::UUID id_;
    bool serialize_;
};
