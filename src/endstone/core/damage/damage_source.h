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

#include "bedrock/world/actor/actor_damage_source.h"
#include "endstone/damage/damage_source.h"

namespace endstone::core {

class EndstoneDamageSource : public DamageSource {
public:
    explicit EndstoneDamageSource(ActorDamageSource &damage_source);

    [[nodiscard]] std::string_view getType() const override;
    [[nodiscard]] Actor *getActor() const override;
    [[nodiscard]] Actor *getDamagingActor() const override;
    [[nodiscard]] bool isIndirect() const override;

private:
    static std::string_view toEndstone(ActorDamageCause cause);

    ActorDamageSource &damage_source_;
    std::string_view type_;
};

}  // namespace endstone::core
