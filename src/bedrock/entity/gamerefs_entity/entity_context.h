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

#include "bedrock/entity/entity_id.h"
#include "bedrock/entity/gamerefs_entity/entity_registry.h"

class EntityContext : public EnableGetWeakRef<EntityContext> {
public:
    EntityContext(EntityRegistry &registry, EntityId entity_id)
        : registry_(registry), entt_registry_(registry.registry_), entity_(entity_id)
    {
    }
    EntityContext(const EntityContext &) = default;
    EntityContext(EntityContext &&) = default;
    EntityContext &operator=(const EntityContext &) = delete;
    EntityContext &operator=(EntityContext &&) = delete;

    [[nodiscard]] bool isValid() const;

    template <typename Component>
    void removeComponent()
    {
        entt_registry_.remove<Component>(entity_);
    }

    template <typename Component>
    [[nodiscard]] bool hasComponent() const
    {
        return entt_registry_.all_of<Component>(entity_);
    }

    template <typename Component>
    Component *tryGetComponent() const
    {
        return entt_registry_.try_get<Component>(entity_);
    }

    template <typename Component>
    Component *tryGetComponent()
    {
        return entt_registry_.try_get<Component>(entity_);
    }

    template <typename Component, typename... Args>
    Component &getOrAddComponent(Args &&...args) const
    {
        return entt_registry_.get_or_emplace<Component>(entity_, std::forward<Args>(args)...);
    }

    [[nodiscard]] WeakRef<EntityContext> getWeakRef() const;
    [[nodiscard]] EntityRegistry &_registry() const;

protected:
    friend class WeakStorageEntity;

    [[nodiscard]] EntityId _getEntityId() const
    {
        return entity_;
    }

    [[nodiscard]] std::uint32_t _getRegistryId() const
    {
        return entity_;
    }

    EntityRegistry &registry_;     // +0
    EnTTRegistry &entt_registry_;  // +8
    const EntityId entity_;        // +16
};
