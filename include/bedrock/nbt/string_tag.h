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
#include <string>
#include <utility>

#include "bedrock/bedrock.h"
#include "bedrock/nbt/tag.h"

class StringTag : public Tag {
public:
    explicit StringTag(std::string data = "") : data(std::move(data)) {}
    void write(IDataOutput &output) const override
    {
        output.writeString(data);
    }
    Bedrock::Result<void> load(IDataInput &input) override
    {
        auto result = input.readStringResult();
        if (result) {
            data = result.value();
            return {};
        }
        return nonstd::make_unexpected(result.error());
    }
    [[nodiscard]] std::string toString() const override
    {
        return data;
    }
    [[nodiscard]] Type getId() const override
    {
        return Type::String;
    }
    [[nodiscard]] bool equals(const Tag &other) const override
    {
        return Tag::equals(other) && data == static_cast<const StringTag &>(other).data;
    }
    [[nodiscard]] std::unique_ptr<Tag> copy() const override
    {
        return std::make_unique<StringTag>(data);
    }
    [[nodiscard]] std::uint64_t hash() const override
    {
        return std::hash<std::string>{}(data);
    }

    std::string data;
};
BEDROCK_STATIC_ASSERT_SIZE(StringTag, 40, 32);
