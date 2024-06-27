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
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/nbt/tag.h"

class IntArrayTag : public Tag {
public:
    explicit IntArrayTag(std::vector<std::int32_t> data = {}) : data(std::move(data)) {}
    void write(IDataOutput &output) const override
    {
        output.writeInt(static_cast<std::int32_t>(data.size()));
        for (int i : data) {
            output.writeInt(i);
        }
    }
    Bedrock::Result<void> load(IDataInput &input) override
    {
        auto result = input.readIntResult();
        if (!result) {
            return nonstd::make_unexpected(result.error());
        }

        auto size = result.value();
        data.clear();
        data.reserve(size);
        for (int i = 0; i < size; ++i) {
            auto result2 = input.readIntResult();
            if (!result2) {
                return nonstd::make_unexpected(result2.error());
            }
            data.push_back(result2.value());
        }
        return {};
    }
    [[nodiscard]] std::string toString() const override
    {
        return fmt::format("[{} ints]", data.size());
    }
    [[nodiscard]] Type getId() const override
    {
        return Type::IntArray;
    }
    [[nodiscard]] bool equals(const Tag &other) const override
    {
        return Tag::equals(other) && data == static_cast<const IntArrayTag &>(other).data;
    }
    [[nodiscard]] std::unique_ptr<Tag> copy() const override
    {
        return std::make_unique<IntArrayTag>(data);
    }
    [[nodiscard]] std::uint64_t hash() const override
    {
        return boost::hash_range(data.begin(), data.end());
    }

    std::vector<std::int32_t> data;
};
BEDROCK_STATIC_ASSERT_SIZE(IntArrayTag, 32, 32);
