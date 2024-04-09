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

#include <string>

#include <entt/entt.hpp>

class EntityId {
public:
    using entity_type = std::uint32_t;  // NOLINT(*-identifier-naming)
    explicit constexpr EntityId(entity_type id) : value_(id){};
    explicit constexpr operator entity_type() const
    {
        return value_;
    }

private:
    entity_type value_;
};

struct EntityTraits {
    using value_type = EntityId;                         // NOLINT(*-identifier-naming)
    using entity_type = uint32_t;                        // NOLINT(*-identifier-naming)
    using version_type = uint16_t;                       // NOLINT(*-identifier-naming)
    static constexpr entity_type entity_mask = 0x3FFFF;  // 18b
    static constexpr entity_type version_mask = 0x3FFF;  // 14b
};

template <>
struct entt::entt_traits<EntityId> : entt::basic_entt_traits<EntityTraits> {
    static constexpr auto page_size = ENTT_SPARSE_PAGE;
};
static_assert(entt::entt_traits<EntityId>::page_size == 2048);

template <>
struct entt::storage_type<EntityId, EntityId, std::allocator<EntityId>, void> {
    using type = entt::basic_storage<EntityId, EntityId, std::allocator<EntityId>>;  // NOLINT(*-identifier-naming)
};

class EntityRegistry : public std::enable_shared_from_this<EntityRegistry> {
public:
private:
    friend class EntityContext;

    std::string name_;                         // +16
    entt::basic_registry<EntityId> registry_;  // +48
    std::uint32_t id_;                         // +336
};
