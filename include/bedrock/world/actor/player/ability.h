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

#include "bedrock/bedrock.h"

class Ability {
public:
    enum class Type {
        Invalid = 0,
        Unset = 1,
        Bool = 2,
        Float = 3,
    };

    enum class Options {
        None = 0
    };

    union Value {
        bool bool_val;
        float float_val;
    };

    Ability() = default;

    [[nodiscard]] Type getType() const
    {
        return type_;
    }

    [[nodiscard]] bool getBool() const
    {
        return type_ == Type::Bool ? value_.bool_val : false;
    }

    [[nodiscard]] float getFloat() const
    {
        return type_ == Type::Float ? value_.float_val : 0.0F;
    }

private:
    Type type_;
    Value value_;
    Options options_;
};
BEDROCK_STATIC_ASSERT_SIZE(Ability, 12, 12);
