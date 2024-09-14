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

#include <entt/entt.hpp>

class EntityId;

struct EntityIdTraits {
    using value_type = EntityId;                         // NOLINT(*-identifier-naming)
    using entity_type = std::uint32_t;                   // NOLINT(*-identifier-naming)
    using version_type = std::uint16_t;                  // NOLINT(*-identifier-naming)
    static constexpr entity_type entity_mask = 0x3FFFF;  // 18b
    static constexpr entity_type version_mask = 0x3FFF;  // 14b
};

class EntityId {
public:
    using underlying_type = EntityIdTraits::entity_type;  // NOLINT(*-identifier-naming)

    EntityId() = default;

    explicit constexpr EntityId(underlying_type entity) : raw_id_(entity){};
    explicit constexpr operator underlying_type() const
    {
        return raw_id_;
    }

private:
    underlying_type raw_id_;
};
