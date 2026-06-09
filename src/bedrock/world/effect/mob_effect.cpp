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

#include "bedrock/world/effect/mob_effect.h"

#include <memory>
#include <string_view>
#include <unordered_map>

#include "bedrock/symbol.h"

MobEffect *MobEffect::getById(MobEffectId id)
{
    if (id >= NUM_EFFECTS) {
        return nullptr;
    }
    // Endstone: MobEffect::mMobEffects[NUM_EFFECTS] is the BDS registry global, indexed by effect id.
    static auto *effects = BEDROCK_VAR(std::unique_ptr<MobEffect> *, "MobEffect::mMobEffects");
    return effects[id].get();
}

MobEffect *MobEffect::getByName(const std::string &name)
{
    // Endstone: BDS keeps its own EFFECTS_BY_NAME hash map; we build the equivalent once from the registry global.
    static const auto effects_by_name = [] {
        std::unordered_map<std::string_view, MobEffect *> map;
        for (std::size_t id = 0; id < NUM_EFFECTS; ++id) {
            if (auto *effect = getById(id); effect) {
                map.emplace(effect->getResourceName(), effect);
            }
        }
        return map;
    }();
    if (const auto it = effects_by_name.find(name); it != effects_by_name.end()) {
        return it->second;
    }
    return nullptr;
}
