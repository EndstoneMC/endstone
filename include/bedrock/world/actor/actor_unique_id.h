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

#pragma once

#include <cstdint>

#include <boost/functional/hash.hpp>

class ActorUniqueID {
public:
    bool operator==(const ActorUniqueID &other) const
    {
        return raw_id == other.raw_id;
    }

    std::int64_t raw_id{-1};
};

namespace std {
template <>
struct hash<ActorUniqueID> {  // NOLINT
    std::size_t operator()(const ActorUniqueID &value) const noexcept
    {
        static std::hash<std::int64_t> hasher;
        std::size_t seed = 0;
        seed ^= hasher(value.raw_id >> 32) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hasher(value.raw_id & 0xffffff) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};
}  // namespace std
