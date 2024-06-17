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
#include <vector>

#include "bedrock/forward.h"
#include "bedrock/world/actor/actor_unique_id.h"

class Actor;
class ActorDamageSource {
public:
    virtual ~ActorDamageSource() = 0;
    [[nodiscard]] virtual bool isEntitySource() const = 0;
    [[nodiscard]] virtual bool isChildEntitySource() const = 0;
    [[nodiscard]] virtual bool isBlockSource() const = 0;
    [[nodiscard]] virtual bool isFire() const = 0;
    [[nodiscard]] virtual bool isReducedByResistanceEffect() const = 0;
    [[nodiscard]] virtual bool isReducedByEnchantReduction() const = 0;
    [[nodiscard]] virtual bool isReducedByArmorReduction() const = 0;
    [[nodiscard]] virtual bool isFallingBlockDamage() const = 0;
    [[nodiscard]] virtual bool isFallDamage() const = 0;
    [[nodiscard]] virtual std::pair<std::string, std::vector<std::string>> getDeathMessage(std::string,
                                                                                           Actor *) const = 0;
    [[nodiscard]] virtual bool getIsCreative() const = 0;
    [[nodiscard]] virtual bool getIsWorldBuilder() const = 0;
    [[nodiscard]] virtual ActorUniqueID getEntityUniqueID() const = 0;
    [[nodiscard]] virtual ActorType getEntityType() const = 0;
    [[nodiscard]] virtual ActorCategory getEntityCategories() const = 0;
    [[nodiscard]] virtual bool getDamagingEntityIsCreative() const = 0;
    [[nodiscard]] virtual bool getDamagingEntityIsWorldBuilder() const = 0;
    [[nodiscard]] virtual ActorUniqueID getDamagingEntityUniqueID() const = 0;
    [[nodiscard]] virtual ActorType getDamagingEntityType() const = 0;
    [[nodiscard]] virtual ActorCategory getDamagingEntityCategories() const = 0;
    [[nodiscard]] virtual std::unique_ptr<ActorDamageSource> clone() const = 0;
};
