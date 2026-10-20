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

#include <string>

#include <entt/core/type_info.hpp>

#include "bedrock/bedrock.h"
#include "bedrock/world/container.h"
#include "bedrock/world/inventory/filling_container.h"
#include "bedrock/world/level/dimension/dimension_type.h"

class Actor;
class Level;
class Player;

class ContainerComponent : public ContainerContentChangeListener, ContainerCloseListener {
public:
    void containerContentChanged(int slot) override;
    void containerClosed(Actor &actor) override;
    [[nodiscard]] bool canOpenContainer(const Actor &container_actor, Player &player) const;
    ENDSTONE_HOOK bool openContainer(Actor &container_actor, Player &player);
    FillingContainer *_getRawContainerPtr() { return &container_; }
    void unpackLootTable(Level &level, DimensionType dimension_id, Actor &owner);

private:
    FillingContainer container_;
    Actor *listener_shim_;
    bool can_be_siphoned_from_;
    bool private_;
    bool restrict_to_owner_;
    std::string loot_table_;
    int loot_table_seed_;
};
BEDROCK_STATIC_ASSERT_SIZE(ContainerComponent, 512, 416);
#if defined __clang__ || defined __GNUC__
static_assert(entt::type_hash<ContainerComponent>::value() == 0x1779E2C7);
#endif
