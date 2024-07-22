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
#include <unordered_map>

#include "bedrock/forward.h"

class ScoreboardId {
public:
    ScoreboardId() = default;
    explicit ScoreboardId(std::int64_t id) : raw_id(id) {}

    static const ScoreboardId INVALID;

    [[nodiscard]] bool isValid() const
    {
        return raw_id != ScoreboardId::INVALID.raw_id;
    }

    bool operator==(const ScoreboardId &other) const
    {
        return raw_id == other.raw_id;
    }

    std::int64_t raw_id{-1};
    IdentityDefinition *identity_def{nullptr};
};

inline const ScoreboardId ScoreboardId::INVALID;

namespace std {
template <>
struct hash<ScoreboardId> {  // NOLINT
    std::size_t operator()(const ScoreboardId &value) const noexcept
    {
        static std::hash<std::int64_t> hasher;
        return hasher(value.raw_id);
    }
};
}  // namespace std