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

UUID::UUID(uint64_t high, uint64_t low)
{
    data[0] = high;
    data[1] = low;
}

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

bool UUID::canParse(std::string_view in)
{
    // Compact form: 32 hex digits
    if (in.size() == 32) {
        for (char c : in) {
            if (!std::isxdigit(static_cast<unsigned char>(c))) {
                return false;
            }
        }
        return true;
    }
    // Hyphenated form: 8-4-4-4-12 = 36 chars
    if (in.size() == 36) {
        for (size_t i = 0; i < 36; ++i) {
            // hyphens at positions 8, 13, 18, 23
            if (i == 8 || i == 13 || i == 18 || i == 23) {
                if (in[i] != '-') {
                    return false;
                }
            }
            else if (!std::isxdigit(static_cast<unsigned char>(in[i]))) {
                return false;
            }
        }
        return true;
    }
    return false;
}

UUID UUID::fromString(std::string_view in)
{
    if (!canParse(in)) {
        return UUID::EMPTY;
    }

    uint64_t high = 0;
    uint64_t low = 0;
    int count = 0;
    for (const auto c : in) {
        if (c == '-') {
            continue;  // skip hyphens entirely
        }

        int nibble;
        if (c >= '0' && c <= '9') {
            nibble = c - '0';
        }
        else if (c >= 'a' && c <= 'f') {
            nibble = c - 'a' + 10;
        }
        else {  // c >= 'A' && c <= 'F'
            nibble = c - 'A' + 10;
        }

        if (count < 16) {
            // first 16 hex digits go into the high 64 bits
            high |= static_cast<uint64_t>(nibble) << ((15 - count) * 4);
        }
        else {
            // last 16 hex digits go into the low 64 bits
            low |= static_cast<uint64_t>(nibble) << ((31 - count) * 4);
        }
        ++count;
    }
    return {high, low};
}

}  // namespace mce
