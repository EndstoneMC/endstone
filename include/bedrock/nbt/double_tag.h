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

#include "bedrock/bedrock.h"
#include "bedrock/nbt/tag.h"

class DoubleTag : public Tag {
public:
    explicit DoubleTag(double data = 0) : data(data) {}
    void write(IDataOutput &output) const override
    {
        output.writeDouble(data);
    }
    Bedrock::Result<void> load(IDataInput &input) override
    {
        auto result = input.readDoubleResult();
        if (result) {
            data = result.value();
            return {};
        }
        return nonstd::make_unexpected(result.error());
    }
    [[nodiscard]] std::string toString() const override
    {
        return std::to_string(data);
    }
    [[nodiscard]] Type getId() const override
    {
        return Type::Double;
    }
    [[nodiscard]] bool equals(const Tag &other) const override
    {
        return Tag::equals(other) && data == static_cast<const DoubleTag &>(other).data;
    }
    [[nodiscard]] std::unique_ptr<Tag> copy() const override
    {
        return std::make_unique<DoubleTag>(data);
    }
    [[nodiscard]] std::uint64_t hash() const override
    {
        return std::hash<double>{}(data);
    }

    double data;
};
BEDROCK_STATIC_ASSERT_SIZE(DoubleTag, 16, 16);
