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
    if (!type_result.ignoreError()) {
        return BEDROCK_RETHROW(type_result);
    }

    auto type = static_cast<Tag::Type>(type_result.discardError().value());
    if (type == Tag::Type::End) {
        return std::make_unique<EndTag>();
    }

    auto name_result = dis.readStringResult();
    if (!name_result.ignoreError()) {
        return BEDROCK_RETHROW(name_result);
    }
    name = name_result.discardError().value();

    auto tag_result = Tag::newTag(type);
    if (!tag_result.ignoreError()) {
        return BEDROCK_RETHROW(tag_result);
    }

    tag_result.discardError().value()->load(dis);
    return std::move(tag_result.discardError().value());
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
