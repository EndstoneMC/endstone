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

#include "bedrock/entity/strict/strict_entity_context.h"

#include "bedrock/entity/gamerefs_entity/entity_context.h"
#include "bedrock/entity/gamerefs_entity/entity_registry.h"

StrictEntityContext::StrictEntityContext() : entity_id_(entt::null), registry_id_(0) {}

StrictEntityContext::StrictEntityContext(const EntityRegistry &registry, EntityId entity)
    : entity_id_(entity), registry_id_(registry.id_)
{
}

StrictEntityContext::StrictEntityContext(const EntityContext &entity)
    : entity_id_(entity.entity_), registry_id_(entity.registry_.id_)
{
}

StrictEntityContext::StrictEntityContext(const StrictEntityContext &) = default;

StrictEntityContext::StrictEntityContext(StrictEntityContext &&other)
    : entity_id_(other.entity_id_), registry_id_(other.registry_id_)
{
    other.entity_id_ = entt::null;
    other.registry_id_ = 0;
}

StrictEntityContext::~StrictEntityContext() = default;

StrictEntityContext &StrictEntityContext::operator=(const StrictEntityContext &) = default;

StrictEntityContext &StrictEntityContext::operator=(StrictEntityContext &&other)
{
    if (this != &other) {
        entity_id_ = other.entity_id_;
        other.entity_id_ = entt::null;
        registry_id_ = other.registry_id_;
        other.registry_id_ = 0;
    }
    return *this;
}

bool StrictEntityContext::operator==(const StrictEntityContext &other) const
{
    return entt::to_integral(entity_id_) == entt::to_integral(other.entity_id_) && registry_id_ == other.registry_id_;
}

bool StrictEntityContext::operator!=(const StrictEntityContext &other) const
{
    return !(*this == other);
}
