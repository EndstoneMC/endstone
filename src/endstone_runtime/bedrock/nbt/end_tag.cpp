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

#include "bedrock/nbt/end_tag.h"

#include "bedrock/io/data_input.h"
#include "bedrock/io/data_output.h"
#include "bedrock/result.h"

void EndTag::write(IDataOutput &output) const {}

Bedrock::Result<void, std::error_code> EndTag::load(IDataInput &input)
{
    return {};
}

std::string EndTag::toString() const
{
    return "END";
}

Tag::Type EndTag::getId() const
{
    return Type::End;
}

bool EndTag::equals(const Tag &tag) const
{
    return Tag::equals(tag);
}

std::unique_ptr<Tag> EndTag::copy() const
{
    return std::unique_ptr<EndTag>();
}

uint64_t EndTag::hash() const
{
    return 0;
}
