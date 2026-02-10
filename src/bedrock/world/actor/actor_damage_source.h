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

#include "bedrock/world/actor/actor_category.h"
#include "bedrock/world/actor/actor_damage_cause.h"
#include "bedrock/world/actor/actor_types.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/level/block/block.h"

class Actor;

class ActorDamageSource {
public:
    ActorDamageSource(const std::string &name, std::string death_message_override);
    ActorDamageSource(ActorDamageCause cause, std::string death_message_override);
    [[nodiscard]] ActorDamageCause getCause() const;
    void setCause(ActorDamageCause cause);
    [[nodiscard]] const std::string &getDeathMessageOverride() const;
    void setDeathMessageOverride(const std::string &death_message_override);

    virtual ~ActorDamageSource() = default;
    [[nodiscard]] virtual bool isEntitySource() const;
    [[nodiscard]] virtual bool isChildEntitySource() const;
    [[nodiscard]] virtual bool isBlockSource() const;
    [[nodiscard]] virtual bool isFire() const;
    [[nodiscard]] virtual bool isReducedByResistanceEffect() const;
    [[nodiscard]] virtual bool isReducedByEnchantReduction() const;
    [[nodiscard]] virtual bool isReducedByArmorReduction() const;
    [[nodiscard]] virtual bool isFallingBlockDamage() const;
    [[nodiscard]] virtual bool isFallDamage() const;
    [[nodiscard]] virtual std::pair<std::string, std::vector<std::string>> getDeathMessage(std::string, Actor *) const;
    [[nodiscard]] virtual bool getIsCreative() const;
    [[nodiscard]] virtual bool getIsWorldBuilder() const;
    [[nodiscard]] virtual ActorUniqueID getEntityUniqueID() const;
    [[nodiscard]] virtual ActorType getEntityType() const;
    [[nodiscard]] virtual ActorCategory getEntityCategories() const;
    [[nodiscard]] virtual bool getDamagingEntityIsCreative() const;
    [[nodiscard]] virtual bool getDamagingEntityIsWorldBuilder() const;
    [[nodiscard]] virtual ActorUniqueID getDamagingEntityUniqueID() const;
    [[nodiscard]] virtual ActorType getDamagingEntityType() const;
    [[nodiscard]] virtual ActorCategory getDamagingEntityCategories() const;
    [[nodiscard]] virtual std::unique_ptr<ActorDamageSource> clone() const;

private:
    ActorDamageCause cause_;
    std::string death_message_override_;
};

class ActorDamageByBlockSource : public ActorDamageSource {
public:
    using ActorDamageSource::ActorDamageSource;
    ActorDamageByBlockSource(const Block &, ActorDamageCause);
    [[nodiscard]] const Block &getBlock() const;
    [[nodiscard]] bool isBlockSource() const override;
    [[nodiscard]] std::pair<std::string, std::vector<std::string>> getDeathMessage(std::string, Actor *) const override;
    [[nodiscard]] std::unique_ptr<ActorDamageSource> clone() const override;

private:
    const Block *block_;  // +16
};
