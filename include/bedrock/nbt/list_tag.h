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
#include <memory>
#include <vector>

#include <boost/container_hash/detail/hash_range.hpp>
#include <fmt/format.h>

#include "bedrock/bedrock.h"
#include "bedrock/nbt/tag.h"

class ListTag : public Tag {
public:
    ListTag() = default;
    void deleteChildren() override
    {
        while (!data_.empty()) {
            data_.back()->deleteChildren();
            data_.pop_back();
        }
    }
    void write(IDataOutput &output) const override
    {
        output.writeByte(static_cast<std::uint8_t>(type_));
        output.writeInt(static_cast<std::int32_t>(data_.size()));
        for (const auto &data : data_) {
            data->write(output);
        }
    }
    Bedrock::Result<void> load(IDataInput &input) override
    {
        auto result = input.readByteResult();
        if (!result) {
            return nonstd::make_unexpected(result.error());
        }
        type_ = static_cast<Type>(result.value());

        auto result2 = input.readIntResult();
        if (!result2) {
            return nonstd::make_unexpected(result2.error());
        }

        auto size = result2.value();
        data_.clear();
        data_.reserve(size);
        for (int i = 0; i < size; ++i) {
            auto result3 = Tag::newTag(type_);
            if (!result3) {
                return nonstd::make_unexpected(result3.error());
            }
            auto tag = std::move(result3.value());
            tag->load(input);
            data_.push_back(std::move(tag));
        }
        return {};
    }
    [[nodiscard]] std::string toString() const override
    {
        return fmt::format("{} entries of type {}", data_.size(), Tag::getTagName(type_));
    }
    [[nodiscard]] Type getId() const override
    {
        return Type::List;
    }
    [[nodiscard]] bool equals(const Tag &other) const override
    {
        return Tag::equals(other) && data_ == static_cast<const ListTag &>(other).data_ &&
               type_ == static_cast<const ListTag &>(other).type_;
    }
    [[nodiscard]] std::unique_ptr<Tag> copy() const override
    {
        return copyList();
    }
    [[nodiscard]] std::uint64_t hash() const override
    {
        return boost::hash_range(data_.begin(), data_.end());
    }
    void print(const std::string &string, PrintStream &stream) const override
    {
        Tag::print(string, stream);
        stream.print(string);
        stream.print("{\n");
        stream.print("   ");
        for (const auto &data : data_) {
            data->print(string, stream);
        }
        stream.print(string);
        stream.print("}\n");
    }
    [[nodiscard]] std::size_t size() const
    {
        return data_.size();
    }
    [[nodiscard]] Tag *get(std::size_t index) const
    {
        return data_[index].get();
    }
    void add(std::unique_ptr<Tag> tag)
    {
        type_ = tag->getId();
        data_.push_back(std::move(tag));
    }
    [[nodiscard]] std::unique_ptr<ListTag> copyList() const
    {
        auto copy = std::make_unique<ListTag>();
        copy->type_ = type_;
        for (const auto &data : data_) {
            copy->data_.push_back(data->copy());
        }
        return copy;
    }

private:
    std::vector<std::unique_ptr<Tag>> data_;  // +8
    Tag::Type type_;                          // +32
};
BEDROCK_STATIC_ASSERT_SIZE(ListTag, 40, 40);
