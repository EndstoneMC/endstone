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

#include <fmt/format.h>

#include "bedrock/bedrock.h"
#include "bedrock/nbt/tag.h"

class ListTag : public Tag {
    using List = std::vector<std::unique_ptr<Tag>>;

public:
    ListTag() = default;

    void deleteChildren() override
    {
        while (!list_.empty()) {
            list_.back()->deleteChildren();
            list_.pop_back();
        }
    }

    void write(IDataOutput &output) const override
    {
        output.writeByte(static_cast<std::uint8_t>(type_));
        output.writeInt(static_cast<std::int32_t>(list_.size()));
        for (const auto &data : list_) {
            data->write(output);
        }
    }

    Bedrock::Result<void> load(IDataInput &input) override
    {
        auto byte_result = input.readByteResult();
        if (!byte_result) {
            return nonstd::make_unexpected(byte_result.error());
        }
        type_ = static_cast<Type>(byte_result.value());

        auto int_result = input.readIntResult();
        if (!int_result) {
            return nonstd::make_unexpected(int_result.error());
        }

        const auto size = int_result.value();
        list_.clear();
        list_.reserve(size);
        for (int i = 0; i < size; ++i) {
            auto tag_result = newTag(type_);
            if (!tag_result) {
                return nonstd::make_unexpected(tag_result.error());
            }
            auto tag = std::move(tag_result.value());
            tag->load(input);
            list_.push_back(std::move(tag));
        }
        return {};
    }

    [[nodiscard]] std::string toString() const override
    {
        return fmt::format("{} entries of type {}", list_.size(), Tag::getTagName(type_));
    }

    [[nodiscard]] Type getId() const override
    {
        return Type::List;
    }

    [[nodiscard]] bool equals(const Tag &other) const override
    {
        return Tag::equals(other) && list_ == static_cast<const ListTag &>(other).list_ &&
               type_ == static_cast<const ListTag &>(other).type_;
    }

    [[nodiscard]] std::unique_ptr<Tag> copy() const override
    {
        return copyList();
    }

    [[nodiscard]] std::size_t hash() const override
    {
        return boost::hash_range(list_.begin(), list_.end());
    }

    void print(const std::string &string, PrintStream &stream) const override
    {
        Tag::print(string, stream);
        stream.print(string);
        stream.print("{\n");
        stream.print("   ");
        for (const auto &data : list_) {
            data->print(string, stream);
        }
        stream.print(string);
        stream.print("}\n");
    }

    [[nodiscard]] std::size_t size() const
    {
        return list_.size();
    }

    [[nodiscard]] Tag *get(std::size_t index) const
    {
        return list_[index].get();
    }

    void add(std::unique_ptr<Tag> tag)
    {
        type_ = tag->getId();
        list_.push_back(std::move(tag));
    }

    [[nodiscard]] std::unique_ptr<ListTag> copyList() const
    {
        auto copy = std::make_unique<ListTag>();
        copy->type_ = type_;
        for (const auto &data : list_) {
            copy->list_.push_back(data->copy());
        }
        return copy;
    }

private:
    List list_;  // +8
    Type type_;  // +32
};
BEDROCK_STATIC_ASSERT_SIZE(ListTag, 40, 40);
