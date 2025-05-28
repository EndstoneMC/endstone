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

#include <fmt/format.h>

#include "bedrock/world/container_id.h"

enum class InventorySourceType : uint32_t {
    InvalidInventory = static_cast<std::underlying_type_t<InventorySourceType>>(-1),
    ContainerInventory = 0,
    GlobalInventory = 1,
    WorldInteraction = 2,
    CreativeInventory = 3,
    NonImplementedFeatureTODO = 99999,
};

class InventorySource {
public:
    enum InventorySourceFlags : uint32_t {
        NoFlag = 0,
        WorldInteraction_Random = 1,
    };

    [[nodiscard]] InventorySourceType getType() const
    {
        return type_;
    }

    [[nodiscard]] ContainerID getContainerId() const
    {
        return container_id_;
    }

    [[nodiscard]] std::uint32_t getFlags() const
    {
        return flags_;
    }

private:
    InventorySource(ContainerID);
    InventorySource(InventorySourceType);
    InventorySource(InventorySourceType, InventorySourceFlags);
    InventorySourceType type_;
    ContainerID container_id_;
    InventorySourceFlags flags_;
};
static_assert(sizeof(InventorySource) == 12);

template <>
struct fmt::formatter<InventorySource> : formatter<string_view> {
    template <typename FormatContext>
    auto format(const InventorySource &source, FormatContext &ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "InventorySource(type={}, container_id={}, flags={})",
                              magic_enum::enum_name(source.getType()), magic_enum::enum_name(source.getContainerId()),
                              source.getFlags());
    }
};
