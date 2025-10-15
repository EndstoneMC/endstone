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

#include "bedrock/core/utility/serialize_simple_types.h"
#include "bedrock/platform/brstd/function_ref.h"
#include "bedrock/platform/result.h"

class ReadOnlyBinaryStream {
public:
    explicit ReadOnlyBinaryStream(std::string_view buffer, bool copy_buffer);
    virtual ~ReadOnlyBinaryStream() = default;

private:
    virtual Bedrock::Result<void> read(void *target, std::uint64_t num);

public:
    void setReadPointer(size_t);
    [[nodiscard]] size_t getReadPointer() const;
    [[nodiscard]] size_t getUnreadLength() const;
    [[nodiscard]] size_t getLength() const;
    Bedrock::Result<unsigned char> getByte();
    Bedrock::Result<unsigned int> getUnsignedVarInt();
    [[nodiscard]] std::string_view getView() const;
    [[nodiscard]] bool hasOverflowed() const;

protected:
    std::string owned_buffer_;  // +8
    std::string_view view_;     // +40

private:
    std::size_t read_pointer_{0};  // +56
    bool has_overflowed_{false};   // +64
};
BEDROCK_STATIC_ASSERT_SIZE(ReadOnlyBinaryStream, 72, 64);

class BinaryStream : public ReadOnlyBinaryStream {
public:
    struct ConditionBlock {
        bool control_value;
        brstd::function_ref<void(BinaryStream &)> write_condition;
        char const *doc_field_name;
    };
    static_assert(sizeof(ConditionBlock) == 32);

    using ReadOnlyBinaryStream::ReadOnlyBinaryStream;
    BinaryStream();
    [[nodiscard]] const std::string &getBuffer() const;
    void reset();

    ~BinaryStream() override = default;
    virtual void writeBool(bool value, char const *doc_field_name, char const *doc_field_notes);
    virtual void writeByte(uint8_t value, char const *doc_field_name, char const *doc_field_notes);
    virtual void writeUnsignedShort(uint16_t value, char const *doc_field_name, char const *doc_field_notes);
    virtual void writeSignedShort(int16_t value, char const *doc_field_name, char const *doc_field_notes);
    virtual void writeUnsignedInt(uint32_t value, char const *doc_field_name, char const *doc_field_notes);
    virtual void writeSignedBigEndianInt(int32_t value, char const *doc_field_name, char const *doc_field_notes);
    virtual void writeSignedInt(int32_t value, char const *doc_field_name, char const *doc_field_notes);
    virtual void writeUnsignedInt64(uint64_t value, char const *doc_field_name, char const *doc_field_notes);
    virtual void writeSignedInt64(int64_t value, char const *doc_field_name, char const *doc_field_notes);
    virtual void writeUnsignedVarInt(uint32_t value, char const *doc_field_name, char const *doc_field_notes);
    virtual void writeUnsignedVarInt64(uint64_t value, char const *doc_field_name, char const *doc_field_notes);
    virtual void writeVarInt(int32_t value, char const *doc_field_name, char const *doc_field_notes);
    virtual void writeVarInt64(int64_t value, char const *doc_field_name, char const *doc_field_notes);
    virtual void writeDouble(double value, char const *doc_field_name, char const *doc_field_notes);
    virtual void writeFloat(float value, char const *doc_field_name, char const *doc_field_notes);
    virtual void writeFixedFloat(float value, char const *doc_field_name, char const *doc_field_notes, double size);
    virtual void writeNormalizedFloat(float value, char const *doc_field_name, char const *doc_field_notes);
    virtual void writeString(std::string_view value, char const *doc_field_name, char const *doc_field_notes);
    virtual void writeIf(bool control_value, char const *doc_field_name,
                         brstd::function_ref<void(BinaryStream &)> const &write_if_true,
                         std::optional<brstd::function_ref<void(BinaryStream &)>> const &write_if_false);
    virtual void writeConditional(char const *doc_field_name, std::initializer_list<ConditionBlock> &&conditions,
                                  std::optional<brstd::function_ref<void(BinaryStream &)>> const &write_default);
    virtual void branchingWrite_DEPRECATED(std::function<void(BinaryStream &, int)> &&branch_writer, int control_value,
                                           int doc_range_begin, int doc_range_end, char const *control_doc_field_name);
    virtual void branchingWrite_DEPRECATED(std::function<void(BinaryStream &, int)> &&branch_writer, int control_value,
                                           std::vector<int> const &doc_control_set, char const *control_doc_field_name);

    template <typename Type>
    void writeVectorList(std::vector<Type> list, const char *entry_doc_field_name, const char *entry_doc_field_notes,
                         const char *list_doc_field_name, const char *list_doc_field_notes)
    {
        _writeArray([&](BinaryStream &stream) { stream.writeUnsignedVarInt(list.size(), "List Size", nullptr); },
                    [&](BinaryStream &stream) {
                        for (const auto &entry : list) {
                            serialize<Type>::write(entry, stream);
                        }
                    },
                    list_doc_field_name, list_doc_field_notes);
    }

private:
    virtual void _writeArray(std::function<void(BinaryStream &)> &&size_writer,
                             std::function<void(BinaryStream &)> &&writer, char const *doc_field_name,
                             char const *doc_field_notes);

public:
    void writeRawBytes(std::string_view span);
    void writeStream(BinaryStream &);

private:
    void write(const void *data, std::size_t size);
    std::string *buffer_;  // +72
};
BEDROCK_STATIC_ASSERT_SIZE(BinaryStream, 80, 72);
