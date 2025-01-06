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
#include <system_error>
#include <utility>
#include <vector>

#include <boost/functional/hash.hpp>
#include <fmt/format.h>

#include "bedrock/bedrock.h"
#include "bedrock/nbt/tag.h"

class ByteArrayTag : public Tag {
public:
    using ArrayData = std::vector<unsigned char>;

    explicit ByteArrayTag(ArrayData data = {}) : data(std::move(data)) {}

    void write(IDataOutput &output) const override
    {
        output.writeInt(static_cast<std::int32_t>(data.size()));
        output.writeBytes(data.data(), data.size());
    }

    Bedrock::Result<void> load(IDataInput &input) override
    {
        auto int_result = input.readIntResult();
        if (!int_result) {
            return nonstd::make_unexpected(int_result.error());
        }
        const auto size = int_result.value();
        if (size > input.numBytesLeft()) {
            return nonstd::make_unexpected(
                Bedrock::ErrorInfo<std::error_code>{std::make_error_code(std::errc::bad_message)});
        }

        data.resize(size);
        if (auto bytes_result = input.readBytesResult(data.data(), size); !bytes_result) {
            return nonstd::make_unexpected(bytes_result.error());
        }
        return {};
    }

    [[nodiscard]] std::string toString() const override
    {
        return fmt::format("[{} bytes]", data.size());
    }

    [[nodiscard]] Type getId() const override
    {
        return Type::ByteArray;
    }

    [[nodiscard]] bool equals(const Tag &other) const override
    {
        return Tag::equals(other) && data == static_cast<const ByteArrayTag &>(other).data;
    }

    [[nodiscard]] std::unique_ptr<Tag> copy() const override
    {
        return std::make_unique<ByteArrayTag>(data);
    }

    [[nodiscard]] std::size_t hash() const override
    {
        return boost::hash_range(data.begin(), data.end());
    }

    ArrayData data;
};
BEDROCK_STATIC_ASSERT_SIZE(ByteArrayTag, 32, 32);
