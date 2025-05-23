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

#include "endstone/core/util/uuid.h"

namespace endstone::core {

mce::UUID EndstoneUUID::toMinecraft(const UUID &in)
{
    mce::UUID out;
    auto *ms = reinterpret_cast<uint8_t *>(&out.data[0]);
    auto *ls = reinterpret_cast<uint8_t *>(&out.data[1]);
    std::reverse_copy(in.data + 0, in.data + 4, ms + 4);
    std::reverse_copy(in.data + 4, in.data + 6, ms + 2);
    std::reverse_copy(in.data + 6, in.data + 8, ms + 0);
    std::reverse_copy(in.data + 8, in.data + 10, ls + 6);
    std::reverse_copy(in.data + 10, in.data + 14, ls + 2);
    std::reverse_copy(in.data + 14, in.data + 16, ls + 0);
    return out;
}

UUID EndstoneUUID::fromMinecraft(const mce::UUID &in)
{
    auto out = UUID{};
    const auto *ms = reinterpret_cast<const uint8_t *>(&in.data[0]);
    const auto *ls = reinterpret_cast<const uint8_t *>(&in.data[1]);
    std::reverse_copy(ms + 4, ms + 8, out.data + 0);
    std::reverse_copy(ms + 2, ms + 4, out.data + 4);
    std::reverse_copy(ms + 0, ms + 2, out.data + 6);
    std::reverse_copy(ls + 6, ls + 8, out.data + 8);
    std::reverse_copy(ls + 2, ls + 6, out.data + 10);
    std::reverse_copy(ls + 0, ls + 2, out.data + 14);
    return out;
}
}  // namespace endstone::core
