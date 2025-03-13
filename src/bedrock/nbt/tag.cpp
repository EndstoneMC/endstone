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

#include "bedrock/nbt/tag.h"

#include "bedrock/nbt/byte_array_tag.h"
#include "bedrock/nbt/byte_tag.h"
#include "bedrock/nbt/compound_tag.h"
#include "bedrock/nbt/double_tag.h"
#include "bedrock/nbt/end_tag.h"
#include "bedrock/nbt/float_tag.h"
#include "bedrock/nbt/int64_tag.h"
#include "bedrock/nbt/int_array_tag.h"
#include "bedrock/nbt/int_tag.h"
#include "bedrock/nbt/list_tag.h"
#include "bedrock/nbt/short_tag.h"
#include "bedrock/nbt/string_tag.h"

bool Tag::equals(const Tag &other) const
{
    return getId() == other.getId();
}

void Tag::print(const std::string &, PrintStream &stream) const
{
    stream.print(getTagName(getId()));
    stream.print(": ");
    stream.print(toString());
    stream.print("\n");
}

void Tag::print(PrintStream &stream) const
{
    print("", stream);
}

Bedrock::Result<std::unique_ptr<Tag>> Tag::newTag(Type type)
{
    switch (type) {
    case Type::End:
        return std::make_unique<EndTag>();
    case Type::Byte:
        return std::make_unique<ByteTag>();
    case Type::Short:
        return std::make_unique<ShortTag>();
    case Type::Int:
        return std::make_unique<IntTag>();
    case Type::Int64:
        return std::make_unique<Int64Tag>();
    case Type::Float:
        return std::make_unique<FloatTag>();
    case Type::Double:
        return std::make_unique<DoubleTag>();
    case Type::ByteArray:
        return std::make_unique<ByteArrayTag>();
    case Type::String:
        return std::make_unique<StringTag>();
    case Type::List:
        return std::make_unique<ListTag>();
    case Type::Compound:
        return std::make_unique<CompoundTag>();
    case Type::IntArray:
        return std::make_unique<IntArrayTag>();
    case Type::NumTagTypes:
    default:
        return BEDROCK_NEW_ERROR(std::errc::bad_message);
    }
}

std::string Tag::getTagName(Tag::Type type)
{
    switch (type) {
    case Type::End:
        return "TAG_End";
    case Type::Byte:
        return "TAG_Byte";
    case Type::Short:
        return "TAG_Short";
    case Type::Int:
        return "TAG_Int";
    case Type::Int64:
        return "TAG_Long";
    case Type::Float:
        return "TAG_Float";
    case Type::Double:
        return "TAG_Double";
    case Type::ByteArray:
        return "TAG_Byte_Array";
    case Type::String:
        return "TAG_String";
    case Type::List:
        return "TAG_List";
    case Type::Compound:
        return "TAG_Compound";
    case Type::IntArray:
        return "TAG_Int_Array";
    case Type::NumTagTypes:
    default:
        return "UNKNOWN";
    }
}
