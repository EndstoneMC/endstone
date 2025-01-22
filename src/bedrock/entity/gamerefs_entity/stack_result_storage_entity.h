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

#include <optional>

#include "bedrock/bedrock.h"
#include "bedrock/entity/gamerefs_entity/entity_context.h"
#include "bedrock/entity/gamerefs_entity/owner_storage_entity.h"
#include "bedrock/entity/gamerefs_entity/weak_storage_entity.h"

class StackResultStorageEntity {
protected:
    StackResultStorageEntity() = default;
    explicit StackResultStorageEntity(nullptr_t);
    explicit StackResultStorageEntity(WeakStorageEntity const &weak_storage);
    explicit StackResultStorageEntity(EntityContext const &);
    StackResultStorageEntity(StackResultStorageEntity const &) = delete;
    StackResultStorageEntity(StackResultStorageEntity &&) = default;
    explicit StackResultStorageEntity(OwnerStorageEntity const &);
    StackResultStorageEntity &operator=(StackResultStorageEntity const &) = delete;
    StackResultStorageEntity &operator=(StackResultStorageEntity &&) = delete;
    [[nodiscard]] bool _hasValue() const;
    [[nodiscard]] EntityContext &_getStackRef() const;

private:
    friend class WeakEntityRef;
    mutable std::optional<EntityContext> context_;
};
BEDROCK_STATIC_ASSERT_SIZE(StackResultStorageEntity, 32, 32);
