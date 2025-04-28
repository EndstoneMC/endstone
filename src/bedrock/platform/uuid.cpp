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

#include "bedrock/platform/uuid.h"

#include <iomanip>
#include <sstream>

namespace mce {

UUID UUID::EMPTY{};

std::string UUID::asString() const
{
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    oss << std::setw(8) << (data[0] >> 32 & 0xFFFFFFFF);
    oss << '-';
    oss << std::setw(4) << (data[0] >> 16 & 0xFFFF);
    oss << '-';
    oss << std::setw(4) << (data[0] & 0xFFFF);
    oss << '-';
    oss << std::setw(4) << (data[1] >> 48 & 0xFFFF);
    oss << '-';
    oss << std::setw(12) << (data[1] & 0xFFFFFFFFFFFF);
    return oss.str();
}

bool UUID::operator<(const UUID &other) const
{
    if (data[0] == other.data[0]) {
        return data[1] < other.data[1];
    }
    return data[0] < other.data[0];
}

}  // namespace mce
