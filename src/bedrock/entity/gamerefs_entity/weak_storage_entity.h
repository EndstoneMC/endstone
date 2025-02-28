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

#include "bedrock/bedrock.h"
#include "bedrock/entity/entity_id.h"
#include "bedrock/entity/gamerefs_entity/entity_context.h"
#include "bedrock/entity/gamerefs_entity/entity_registry.h"
#include "bedrock/gamerefs/weak_ref.h"

class WeakStorageEntity {
protected:
    WeakStorageEntity() = default;
    explicit WeakStorageEntity(const EntityContext &ctx);
    [[nodiscard]] bool _isSet() const;

private:
    friend class EntityContext;
    friend class StackResultStorageEntity;
    WeakRef<EntityRegistry> registry_;
    EntityId entity_;
};
BEDROCK_STATIC_ASSERT_SIZE(WeakStorageEntity, 24, 24);
