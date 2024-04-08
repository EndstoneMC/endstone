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

using EntityId = std::uint32_t;

namespace entt {
template <>
struct storage_type<EntityId, EntityId, std::allocator<EntityId>, void> {
    using type = basic_storage<EntityId, EntityId, std::allocator<EntityId>>;  // NOLINT(*-identifier-naming)
};
}  // namespace entt

class EntityRegistry : public std::enable_shared_from_this<EntityRegistry> {
public:
private:
    friend class EntityContext;

    std::string name_;                         // +16
    entt::basic_registry<EntityId> registry_;  // +48
    std::uint32_t id_;                         // +336
};
