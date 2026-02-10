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

#include "bedrock/world/actor/actor_damage_cause.h"
#include "bedrock/world/actor/actor_damage_source.h"
#include "bedrock/world/attribute/amplifier.h"

enum class AttributeBuffType : int {
    Hunger = 0,
    Saturation = 1,
    Regeneration = 2,
    Heal = 3,
    Harm = 4,
    Magic = 5,
    Wither = 6,
    Poison = 7,
    FatalPoison = 8,
    SelfHeal = 9,
    SelfDestruct = 10,
    Unknown = 11,
    None = 12,
};

class AttributeBuff {
public:
    virtual ~AttributeBuff() = default;
    [[nodiscard]] virtual bool isInstantaneous() const = 0;
    [[nodiscard]] virtual bool isSerializable() const = 0;
    virtual void setDurationAmplifier(std::shared_ptr<Amplifier>);

    [[nodiscard]] float getAmount() const;
    [[nodiscard]] AttributeBuffType getType() const;
    [[nodiscard]] ActorDamageCause getCause() const;
    [[nodiscard]] const ActorDamageSource &getSource() const
    {
        return *source_;
    }
    bool operator==(const AttributeBuff &) const;
    void setValueAmplifier(std::shared_ptr<Amplifier>);
    void setAmplificationAmount(int, float);
    void setSource(Actor *);
    [[nodiscard]] std::size_t getId() const;
    void setId(std::size_t);
    [[nodiscard]] int getOperand() const;
    void setOperand(int);
    static ActorDamageCause buffTypeToDamageCause(AttributeBuffType);

protected:
    AttributeBuff(float, int, AttributeBuffType);
    AttributeBuff(float, int, const ActorDamageSource &, AttributeBuffType);

    float amount_;
    AttributeBuffType type_;
    std::shared_ptr<ActorDamageSource> source_;
    std::shared_ptr<Amplifier> value_amplifier_;
    std::shared_ptr<Amplifier> duration_amplifier_;
    float scale_;
    int amplification_;
    size_t id_;
    int operand_;
};
static_assert(sizeof(AttributeBuff) == 88);

class InstantaneousAttributeBuff : public AttributeBuff {
public:
    InstantaneousAttributeBuff(float, AttributeBuffType);
    InstantaneousAttributeBuff(float, const ActorDamageSource &);
    [[nodiscard]] bool isInstantaneous() const override;
    [[nodiscard]] bool isSerializable() const override;
};
static_assert(sizeof(InstantaneousAttributeBuff) == 88);
