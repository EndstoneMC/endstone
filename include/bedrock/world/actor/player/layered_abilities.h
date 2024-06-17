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

#include <array>

#include "bedrock/world/actor/player/abilities.h"
#include "bedrock/world/actor/player/abilities_layer.h"
#include "bedrock/world/actor/player/permissions_handler.h"

class LayeredAbilities {
public:
    [[nodiscard]] Ability &getAbility(AbilitiesIndex index)
    {
        Ability *ability;
        for (auto i = static_cast<int>(AbilitiesLayer::LayerCount) - 1; i >= 0; i--) {
            auto &abilities = layers_[i];
            ability = &abilities.getAbility(index);
            if (ability->getType() != Ability::Type::Unset) {
                break;
            }
        }
        return *ability;
    }

    [[nodiscard]] const Ability &getAbility(AbilitiesIndex index) const
    {
        const Ability *ability;
        for (auto i = static_cast<int>(AbilitiesLayer::LayerCount) - 1; i >= 0; i--) {
            const auto &abilities = layers_[i];
            ability = &abilities.getAbility(index);
            if (ability->getType() != Ability::Type::Unset) {
                break;
            }
        }
        return *ability;
    }

    [[nodiscard]] bool getBool(AbilitiesIndex index) const
    {
        return getAbility(index).getBool();
    }

    [[nodiscard]] float getFloat(AbilitiesIndex index) const
    {
        return getAbility(index).getFloat();
    }

    void setAbility(AbilitiesIndex index, bool value)
    {
        layers_.at(static_cast<int>(AbilitiesLayer::Base)).setAbility(index, value);
    }

    void setAbility(AbilitiesIndex index, float value)
    {
        layers_.at(static_cast<int>(AbilitiesLayer::Base)).setAbility(index, value);
    }

    PermissionsHandler &getPermissionHandler()
    {
        return permissions_;
    }

    [[nodiscard]] const PermissionsHandler &getPermissionHandler() const
    {
        return permissions_;
    }

    void forEachLayer(const std::function<void(AbilitiesLayer, Abilities &)> &callback)
    {
        for (auto i = static_cast<int>(AbilitiesLayer::LayerCount) - 1; i >= 0; i--) {
            callback(static_cast<AbilitiesLayer>(i), layers_.at(i));
        }
    }

    void forEachLayer(const std::function<void(AbilitiesLayer, const Abilities &)> &callback) const
    {
        for (auto i = static_cast<int>(AbilitiesLayer::LayerCount) - 1; i >= 0; i--) {
            callback(static_cast<AbilitiesLayer>(i), layers_.at(i));
        }
    }

private:
    PermissionsHandler permissions_;
    std::array<Abilities, static_cast<int>(AbilitiesLayer::LayerCount)> layers_;
};
