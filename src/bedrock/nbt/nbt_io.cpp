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

#include "bedrock/nbt/nbt_io.h"

#include "bedrock/nbt/compound_tag.h"

Bedrock::Result<std::unique_ptr<Tag>> NbtIo::readNamedTag(IDataInput &dis, std::string &name)
{
    auto type_result = dis.readByteResult();
    if (!type_result) {
        return nonstd::make_unexpected(type_result.error());
    }

    auto type = static_cast<Tag::Type>(type_result.value());
    if (type == Tag::Type::End) {
        return std::make_unique<EndTag>();
    }

    auto name_result = dis.readStringResult();
    if (!name_result) {
        return nonstd::make_unexpected(name_result.error());
    }
    name = name_result.value();

    auto tag_result = Tag::newTag(type);
    if (!tag_result) {
        return nonstd::make_unexpected(tag_result.error());
    }

    tag_result.value()->load(dis);
    return std::move(tag_result.value());
}

void NbtIo::writeNamedTag(const std::string &name, const Tag &tag, IDataOutput &output)
{
    auto type = tag.getId();
    output.writeByte(static_cast<std::uint8_t>(type));
    if (type != Tag::Type::End) {
        output.writeString(name);
        tag.write(output);
    }
}
