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

#include "bedrock/core/utility/binary_stream.h"

#include <fmt/core.h>

#include "bedrock/core/utility/serialize_simple_types.h"

ReadOnlyBinaryStream::ReadOnlyBinaryStream(std::string_view buffer, bool copy_buffer)
{
    auto view = buffer;
    if (copy_buffer) {
        owned_buffer_ = buffer;
        view = owned_buffer_;
    }
    read_pointer_ = 0;
    has_overflowed_ = false;
    view_ = view;
}

Bedrock::Result<void> ReadOnlyBinaryStream::read(void *target, std::uint64_t num)
{
    if (has_overflowed_) {
        return BEDROCK_NEW_ERROR_MESSAGE(std::errc::invalid_seek, "BinaryStream read() overflow");
    }

    if (num == 0) {
        return {};
    }

    if (auto checked_number = read_pointer_ + num; checked_number < read_pointer_ || checked_number > view_.size()) {
        has_overflowed_ = true;
        return BEDROCK_NEW_ERROR_MESSAGE(
            std::errc::invalid_seek,
            fmt::format(
                "BinaryStream read() overflow checkedNumber = {}, mReadPointer = {}, buffer Length is = {} bytes",
                checked_number, read_pointer_, view_.size()));
    }
    std::memcpy(target, view_.data() + read_pointer_, num);
    read_pointer_ += num;
    return {};
}

void ReadOnlyBinaryStream::setReadPointer(size_t read_pointer)
{
    read_pointer_ = read_pointer;
}

size_t ReadOnlyBinaryStream::getReadPointer() const
{
    return read_pointer_;
}

size_t ReadOnlyBinaryStream::getUnreadLength() const
{
    return view_.size() - read_pointer_;
}

size_t ReadOnlyBinaryStream::getLength() const
{
    return view_.size();
}

Bedrock::Result<unsigned char> ReadOnlyBinaryStream::getByte()
{
    unsigned char value = 0;
    auto result = read(&value, sizeof(value));
    if (!result.ignoreError()) {
        return BEDROCK_RETHROW(result);
    }
    return value;
}

Bedrock::Result<unsigned int> ReadOnlyBinaryStream::getUnsignedVarInt()
{
    unsigned int value = 0;
    for (auto i = 0;; i += 7) {
        auto byte_result = getByte();
        if (!byte_result.ignoreError()) {
            return BEDROCK_RETHROW(byte_result);
        }
        const auto byte = byte_result.discardError().value();
        value |= (byte & 0x7F) << i;
        if ((byte & 0x80U) == 0) {
            break;
        }
    }
    return value;
}

std::string_view ReadOnlyBinaryStream::getView() const
{
    return view_;
}

bool ReadOnlyBinaryStream::hasOverflowed() const
{
    return has_overflowed_;
}

BinaryStream::BinaryStream() : ReadOnlyBinaryStream("", false), buffer_(&owned_buffer_)
{
    view_ = owned_buffer_;
}

const std::string &BinaryStream::getBuffer() const
{
    return *buffer_;
}

void BinaryStream::reset()
{
    buffer_->clear();
    view_ = *buffer_;
    setReadPointer(0);
}

void BinaryStream::writeBool(bool value, char const *doc_field_name, char const *doc_field_notes)
{
    writeByte(value ? 1 : 0, doc_field_name, nullptr);
}

void BinaryStream::writeByte(std::uint8_t value, char const *doc_field_name, char const *doc_field_notes)
{
    write(&value, sizeof(value));
}

void BinaryStream::writeUnsignedShort(std::uint16_t value, char const *doc_field_name, char const *doc_field_notes)
{
    write(&value, sizeof(value));
}

void BinaryStream::writeSignedShort(std::int16_t value, char const *doc_field_name, char const *doc_field_notes)
{
    write(&value, sizeof(value));
}

void BinaryStream::writeUnsignedInt(uint32_t value, char const *doc_field_name, char const *doc_field_notes)
{
    write(&value, sizeof(value));
}
void BinaryStream::writeSignedBigEndianInt(int32_t value, char const *doc_field_name, char const *doc_field_notes)
{
#if defined(_MSC_VER)
    auto v = _byteswap_ulong(value);
#else
    auto v = __builtin_bswap32(value);
#endif
    write(&v, sizeof(v));
}

void BinaryStream::writeSignedInt(int32_t value, char const *doc_field_name, char const *doc_field_notes)
{
    write(&value, sizeof(value));
}

void BinaryStream::writeUnsignedInt64(uint64_t value, char const *doc_field_name, char const *doc_field_notes)
{
    write(&value, sizeof(value));
}

void BinaryStream::writeSignedInt64(int64_t value, char const *doc_field_name, char const *doc_field_notes)
{
    write(&value, sizeof(value));
}

void BinaryStream::writeUnsignedVarInt(std::uint32_t value, char const *doc_field_name, char const *doc_field_notes)
{
    do {
        std::uint8_t byte = value & 0x7F;
        value >>= 7;
        if (value) {
            writeByte(byte | 0x80, doc_field_name, nullptr);
        }
        else {
            writeByte(byte, doc_field_name, nullptr);
        }
    } while (value);
}

void BinaryStream::writeUnsignedVarInt64(std::uint64_t value, char const *doc_field_name, char const *doc_field_notes)
{
    do {
        std::uint8_t byte = value & 0x7F;
        value >>= 7;
        if (value) {
            writeByte(byte | 0x80, doc_field_name, nullptr);
        }
        else {
            writeByte(byte, doc_field_name, nullptr);
        }
    } while (value);
}

void BinaryStream::writeVarInt(std::int32_t value, char const *doc_field_name, char const *doc_field_notes)
{
    writeUnsignedVarInt((value >> 31) ^ (value << 1), doc_field_name, nullptr);
}

void BinaryStream::writeVarInt64(std::int64_t value, char const *doc_field_name, char const *doc_field_notes)
{
    writeUnsignedVarInt64((value >> 63) ^ (value << 1), doc_field_name, nullptr);
}

void BinaryStream::writeDouble(double value, char const *doc_field_name, char const *doc_field_notes)
{
    write(&value, sizeof(value));
}

void BinaryStream::writeFloat(float value, char const *doc_field_name, char const *doc_field_notes)
{
    write(&value, sizeof(value));
}

void BinaryStream::writeFixedFloat(float value, char const *doc_field_name, char const *doc_field_notes, double size)
{
    writeVarInt64(value * size, doc_field_name, doc_field_notes);
}

void BinaryStream::writeNormalizedFloat(float value, char const *doc_field_name, char const *doc_field_notes)
{
    writeVarInt64(value * static_cast<double>(std::numeric_limits<int>::max()), doc_field_name, nullptr);
}

void BinaryStream::writeString(std::string_view value, char const *doc_field_name, char const *doc_field_notes)
{
    writeUnsignedVarInt(value.length(), "String Length", nullptr);
    write(value.data(), value.size());
}

void BinaryStream::writeIf(bool control_value, char const *doc_field_name,
                           brstd::function_ref<void(BinaryStream &)> const &write_if_true,
                           std::optional<brstd::function_ref<void(BinaryStream &)>> const &write_if_false)
{
    if (control_value) {
        write_if_true(*this);
    }
    else if (write_if_false.has_value()) {
        write_if_false.value()(*this);
    }
}

void BinaryStream::writeConditional(char const *doc_field_name, std::initializer_list<ConditionBlock> &&conditions,
                                    std::optional<brstd::function_ref<void(BinaryStream &)>> const &write_default)
{
    for (const auto &condition : conditions) {
        if (condition.control_value) {
            // When a valid condition is found, call its function and return.
            condition.write_condition(*this);
            return;
        }
    }
    // If no condition block with a true control_value is found,
    // and a default write function is provided, then call it.
    if (write_default.has_value()) {
        write_default.value()(*this);
    }
}

void BinaryStream::branchingWrite_DEPRECATED(std::function<void(BinaryStream &, int)> &&branch_writer,
                                             int control_value, int doc_range_begin, int doc_range_end,
                                             char const *control_doc_field_name)
{
    branch_writer(*this, control_value);
}

void BinaryStream::branchingWrite_DEPRECATED(std::function<void(BinaryStream &, int)> &&branch_writer,
                                             int control_value, std::vector<int> const &doc_control_set,
                                             char const *control_doc_field_name)
{
    branch_writer(*this, control_value);
}

void BinaryStream::_writeArray(std::function<void(BinaryStream &)> &&size_writer,
                               std::function<void(BinaryStream &)> &&writer, char const *doc_field_name,
                               char const *doc_field_notes)
{
    size_writer(*this);
    writer(*this);
}

void BinaryStream::writeRawBytes(std::string_view span)
{
    write(span.data(), span.size());
}

void BinaryStream::writeStream(BinaryStream &stream)
{
    buffer_->append(stream.getView().substr(stream.getReadPointer(), stream.getUnreadLength()));
}

void BinaryStream::write(const void *data, std::size_t size)
{
    if (size > 0) {
        buffer_->append(static_cast<const char *>(data), size);
    }
    view_ = *buffer_;
}
