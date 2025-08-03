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

#include <memory>
#include <variant>

#include "bedrock/util/molang/complex_expression.h"

class ExpressionNode {
public:
    ExpressionNode();
    ExpressionNode(float value) : impl_(value) {}
    ExpressionNode(const ExpressionNode &other)
    {
        if (std::holds_alternative<std::unique_ptr<Molang::details::IComplexExpression>>(other.impl_)) {
            const auto &expr = std::get<std::unique_ptr<Molang::details::IComplexExpression>>(other.impl_);
            impl_ = expr ? std::make_unique<Molang::details::IComplexExpression>(*expr) : nullptr;
        }
        else if (std::holds_alternative<float>(other.impl_)) {
            impl_ = std::get<float>(other.impl_);
        }
    }

    ExpressionNode &operator=(const ExpressionNode &other)
    {
        if (this == &other) {
            return *this;
        }

        if (std::holds_alternative<std::unique_ptr<Molang::details::IComplexExpression>>(other.impl_)) {
            const auto &expr = std::get<std::unique_ptr<Molang::details::IComplexExpression>>(other.impl_);
            impl_ = expr ? std::make_unique<Molang::details::IComplexExpression>(*expr) : nullptr;
        }
        else if (std::holds_alternative<float>(other.impl_)) {
            impl_ = std::get<float>(other.impl_);
        }
        return *this;
    }

private:
    std::variant<std::unique_ptr<Molang::details::IComplexExpression>, float> impl_;
};
