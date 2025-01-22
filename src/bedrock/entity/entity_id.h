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

namespace entt {
template <>
struct entt_traits<EntityId> : basic_entt_traits<EntityIdTraits> {
    static constexpr std::size_t page_size = ENTT_SPARSE_PAGE;  // NOLINT(*-identifier-naming)
};
}  // namespace entt

class EntityId {
    template <typename Traits>
    friend class entt::basic_entt_traits;
    friend class EntityContext;

    using underlying_type = EntityIdTraits::entity_type;             // NOLINT(*-identifier-naming)
    constexpr EntityId(underlying_type raw_id) : raw_id_(raw_id){};  // NOLINT(*-explicit-constructor)
    using entity_type = underlying_type;                             // NOLINT(*-identifier-naming)

public:
    EntityId() = default;
    EntityId(const EntityId &) = default;

private:
    underlying_type raw_id_ = -1;
    constexpr operator unsigned int() const  // NOLINT(*-explicit-constructor)
    {
        return raw_id_;
    }
};

static_assert(entt::entt_traits<EntityId>::page_size == 2048, "ENTT_SPARSE_PAGE should be set to 2048.");
static_assert(std::is_same_v<entt::storage_type<EntityId>::type, entt::basic_storage<EntityId>>,
              "ENTT_NO_MIXIN is not defined.");
using EnTTRegistry = entt::basic_registry<EntityId>;
