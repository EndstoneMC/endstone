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

#include "bedrock/entity/gamerefs_entity/stack_result_storage_entity.h"

StackResultStorageEntity::StackResultStorageEntity(nullptr_t) : context_(std::nullopt) {}

StackResultStorageEntity::StackResultStorageEntity(WeakStorageEntity const &weak_storage)
{
    if (weak_storage._isSet()) {
        if (auto registry = weak_storage.registry_.unwrap(); registry) {
            if (EntityContext entity_context(registry.value(), weak_storage.entity_); entity_context.isValid()) {
                context_.emplace(entity_context);
            }
        }
    }
}

StackResultStorageEntity::StackResultStorageEntity(EntityContext const &context)
{
    if (context.isValid()) {
        context_.emplace(context);
    }
}

StackResultStorageEntity::StackResultStorageEntity(OwnerStorageEntity const &ref) : context_(ref.getStackRef()) {}

bool StackResultStorageEntity::_hasValue() const
{
    return context_.has_value();
}

EntityContext &StackResultStorageEntity::_getStackRef() const
{
    return context_.value();
}
