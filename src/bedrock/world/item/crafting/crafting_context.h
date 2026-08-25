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

#include <memory>

#include "bedrock/bedrock.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/resources/base_game_version.h"
#include "bedrock/world/item/registry/trim_material_registry.h"
#include "bedrock/world/item/registry/trim_pattern_registry.h"

class ILevel;

class CraftingContext {
    class Impl {
        const std::weak_ptr<TrimPatternRegistry> trim_pattern_registry_;
        const std::weak_ptr<const TrimMaterialRegistry> trim_material_registry_;
        BaseGameVersion base_game_version_;
        // Experiments and map callbacks occupy the rest of the 392-byte Impl; unused by trim assemble.

    public:
        explicit Impl(Bedrock::NotNullNonOwnerPtr<ILevel> level);
        [[nodiscard]] std::weak_ptr<const TrimPatternRegistry> getTrimPatternRegistry() const
        {
            return trim_pattern_registry_;
        }
        [[nodiscard]] std::weak_ptr<TrimPatternRegistry> getTrimPatternRegistry()
        {
            return trim_pattern_registry_;
        }
        [[nodiscard]] std::weak_ptr<const TrimMaterialRegistry> getTrimMaterialRegistry() const
        {
            return trim_material_registry_;
        }
    };

public:
    ~CraftingContext() = default;
    explicit CraftingContext(Bedrock::NotNullNonOwnerPtr<ILevel> level);

    [[nodiscard]] std::weak_ptr<const TrimPatternRegistry> getTrimPatternRegistry() const
    {
        return impl_->getTrimPatternRegistry();
    }
    [[nodiscard]] std::weak_ptr<TrimPatternRegistry> getTrimPatternRegistry()
    {
        return impl_->getTrimPatternRegistry();
    }
    [[nodiscard]] std::weak_ptr<const TrimMaterialRegistry> getTrimMaterialRegistry() const
    {
        return impl_->getTrimMaterialRegistry();
    }

private:
    std::unique_ptr<Impl> impl_;
};
BEDROCK_STATIC_ASSERT_SIZE(CraftingContext, 8, 8);
