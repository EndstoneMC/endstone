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
#include <optional>

#include <entt/entt.hpp>

#include "bedrock/memory.h"
#include "bedrock/world/actor/registry/entity_registry.h"

class EntityContext {
public:
    EntityContext(EntityRegistry &registry, EntityId entity_id)
        : registry_(registry), entt_registry_(registry.registry_), entity_id_(entity_id){};

    template <typename Component>
    Component *tryGetComponent()
    {
        return entt_registry_.try_get<Component>(entity_id_);
    }

    template <typename Component>
    Component *tryGetComponent() const
    {
        return entt_registry_.try_get<Component>(entity_id_);
    }

    template <typename Component, typename... Args>
    Component &getOrAddComponent(Args &&...args) const
    {
        return entt_registry_.get_or_emplace<Component>(entity_id_, std::forward<Args>(args)...);
    }

private:
    EntityRegistry &registry_;                       // +0
    entt::basic_registry<EntityId> &entt_registry_;  // +8
    EntityId entity_id_;                             // +16
};

class OwnerStorageEntity {
public:
    OwnerStorageEntity(const OwnerStorageEntity &) = delete;
    OwnerStorageEntity &operator=(const OwnerStorageEntity &) = delete;
    OwnerStorageEntity(OwnerStorageEntity &&) noexcept = default;
    OwnerStorageEntity &operator=(OwnerStorageEntity &&) noexcept = delete;

    std::optional<EntityContext> context;
};
static_assert(sizeof(OwnerStorageEntity) == 32);

class WeakStorageEntity {
public:
    WeakRef<EntityRegistry> registry;
    EntityId entity_id;
};
static_assert(sizeof(WeakStorageEntity) == 24);

template <>
class OwnerPtr<EntityContext> {
public:
    OwnerPtr(const OwnerPtr &) = delete;
    OwnerPtr &operator=(const OwnerPtr &) = delete;
    OwnerPtr(OwnerPtr &&) noexcept = default;
    OwnerPtr &operator=(OwnerPtr &&) noexcept = delete;

    OwnerStorageEntity storage;
};

template <>
class WeakRef<EntityContext> {
public:
    WeakStorageEntity storage;
};
