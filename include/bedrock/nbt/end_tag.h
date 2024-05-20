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
#include "bedrock/nbt/tag.h"

class EndTag : public Tag {
public:
    void write(IDataOutput &output) const override {}
    Bedrock::Result<void> load(IDataInput &input) override
    {
        return {};
    }
    [[nodiscard]] std::string toString() const override
    {
        return "END";
    }
    [[nodiscard]] Type getId() const override
    {
        return Type::End;
    }
    [[nodiscard]] bool equals(const Tag &other) const override
    {
        return Tag::equals(other);
    }
    [[nodiscard]] std::unique_ptr<Tag> copy() const override
    {
        return std::make_unique<EndTag>();
    }
    [[nodiscard]] std::uint64_t hash() const override
    {
        return 0;
    }
};
BEDROCK_STATIC_ASSERT_SIZE(EndTag, 8, 8);
