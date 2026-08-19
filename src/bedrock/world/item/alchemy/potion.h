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
#include <string>
#include <string_view>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/world/effect/mob_effect_instance.h"

class Potion {
public:
    enum PotionType : int {
        Undefined = -1,
        Regular = 0,
        Splash = 1,
        Lingering = 2,
    };

    enum PotionVariant : unsigned int {
        MOVESLOW = 0,
        MOVESPEED = 1,
        DIGSLOW = 2,
        DIGSPEED = 3,
        DAMAGEBOOST = 4,
        HEAL = 5,
        HARM = 6,
        JUMP = 7,
        CONFUSION = 8,
        REGEN = 9,
        RESISTANCE = 10,
        FIRERESISTANCE = 11,
        WATERBREATH = 12,
        INVISIBILITY = 13,
        BLINDNESS = 14,
        NIGHTVISION = 15,
        HUNGER = 16,
        WEAKNESS = 17,
        POISON = 18,
        WITHER = 19,
        HEALTHBOOST = 20,
        ABSORPTION = 21,
        SATURATION = 22,
        LEVITATION = 23,
        TURTLEMASTER = 24,
        SLOWFALL = 25,
        WINDCHARGED = 26,
        WEAVING = 27,
        OOZING = 28,
        INFESTED = 29,
        BASE = 30,
    };

    static constexpr int MAX_POTIONS = 64;

    static std::shared_ptr<const Potion> getPotion(int potion_id);
    static std::shared_ptr<const Potion> getPotion(std::string_view potion_name_id);

    static std::vector<std::shared_ptr<const Potion>> getPotions();

    [[nodiscard]] int getPotionId() const { return id_; }

    [[nodiscard]] std::string getNameId() const { return name_id_; }

    [[nodiscard]] std::string getDescriptionId(PotionType potion_type) const;

    [[nodiscard]] std::string getDescriptionId() const;

    [[nodiscard]] const std::vector<MobEffectInstance> &getMobEffects() const { return effects_; }

    [[nodiscard]] PotionVariant getPotionVariant() const { return variant_; }

private:
    int id_;
    std::string name_id_;
    std::string prefix_;
    std::vector<MobEffectInstance> effects_;
    std::vector<std::string> description_ids_;
    PotionVariant variant_;
};
BEDROCK_STATIC_ASSERT_SIZE(Potion, 0x80, 0x70);
