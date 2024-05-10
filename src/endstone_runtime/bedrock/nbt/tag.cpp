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

void Tag::deleteChildren() {}

void Tag::print(const std::string &, PrintStream &stream) const
{
    stream.print(Tag::getTagName(getId()));
    stream.print(": ");
    stream.println(toString());
}

void Tag::print(PrintStream &stream) const
{
    print({}, stream);
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
