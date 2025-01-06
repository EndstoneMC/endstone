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

StackResultStorageEntity::StackResultStorageEntity(nullptr_t) {}

StackResultStorageEntity::StackResultStorageEntity(EntityContext const &context) : context_(context) {}

StackResultStorageEntity::StackResultStorageEntity(OwnerStorageEntity const &ref) : context_(ref.getStackRef()) {}

StackResultStorageEntity::StackResultStorageEntity(WeakStorageEntity const &ref)
    : context_(EntityContext{*ref.registry_, ref.entity_})
{
}

EntityContext &StackResultStorageEntity::getStackRef() const
{
    if (!context_.has_value()) {
        throw std::bad_optional_access();
    }
    return const_cast<EntityContext &>(context_.value());
}
