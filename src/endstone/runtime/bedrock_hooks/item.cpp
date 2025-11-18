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

#include "bedrock/world/item/item.h"

#include "bedrock/util/string_byte_input.h"

void Item::readUserData(ItemStackBase &stack, IDataInput &input, ReadOnlyBinaryStream &underlying_stream) const
{
    auto marker_result = input.readShortResult();
    if (!marker_result.ignoreError()) {
        return;
    }
    if (marker_result.discardError().value() == -1) {
        auto version_result = input.readByteResult();
        if (!version_result.ignoreError()) {
            return;
        }
        if (version_result.discardError().value() != 1) {
            return;
        }
        if (auto tags = NbtIo::readOrGetEmpty(input)) {
            stack.setUserData(std::move(tags));
        }
    }
    else if (marker_result.discardError().value() != 0) {
        // legacy format?
        return;
    }
    stack.deserializeComponents(input);
}
