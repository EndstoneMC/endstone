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

class OwnerStorageEntity {
protected:
    OwnerStorageEntity() = default;
    OwnerStorageEntity(EntityRegistry &registry);
    OwnerStorageEntity(const OwnerStorageEntity &) = delete;
    OwnerStorageEntity(OwnerStorageEntity &&other) noexcept : context_(other.context_)
    {
        other.context_.reset();
    }
    ~OwnerStorageEntity()
    {
        if (context_.has_value()) {
            context_->_registry()._destroyEntity(context_.value());
        }
    }
    OwnerStorageEntity &operator=(const OwnerStorageEntity &) = delete;
    OwnerStorageEntity &operator=(OwnerStorageEntity &&other) noexcept
    {
        if (this != &other) {
            if (context_.has_value()) {
                context_->_registry()._destroyEntity(context_.value());
                context_.reset();
            }

            if (other.context_.has_value()) {
                context_.emplace(std::move(other.context_.value()));
                other.context_.reset();
            }
        }
        return *this;
    }

    [[nodiscard]] bool _hasValue() const
    {
        return context_.has_value();
    }

    [[nodiscard]] EntityContext &_getStackRef() const
    {
        return const_cast<EntityContext &>(context_.value());
    }

private:
    friend class StackResultStorageEntity;
    std::optional<EntityContext> context_;
};
BEDROCK_STATIC_ASSERT_SIZE(OwnerStorageEntity, 32, 32);
