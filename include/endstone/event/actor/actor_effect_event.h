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

#include <optional>
#include <utility>

#include "endstone/actor/mob.h"
#include "endstone/event/actor/actor_event.h"
#include "endstone/event/cancellable.h"
#include "endstone/potion/effect.h"
#include "endstone/potion/effect_type.h"

namespace endstone {

/**
 * Called when an effect is modified on a Mob.
 *
 * If the event is cancelled, no change will be made on the mob.
 */
class ActorEffectEvent : public Cancellable<ActorEvent<Mob>> {
public:
    /**
     * An enum to specify the action to be performed.
     */
    enum class Action {
        /**
         * When the effect is added because the mob didn't have its type.
         */
        Added,
    };

    ENDSTONE_EVENT(ActorEffectEvent);

    explicit ActorEffectEvent(const NotNull<Mob> &mob, Action action, EffectId modified_type,
                              std::optional<Effect> old_effect, std::optional<Effect> new_effect)
        : Cancellable(mob), action_(action), modified_type_(std::move(modified_type)),
          old_effect_(std::move(old_effect)), new_effect_(std::move(new_effect))
    {
    }

    /**
     * Gets the action which will be performed on the effect type.
     *
     * @return An action to be performed on the effect type.
     */
    [[nodiscard]] Action getAction() const { return action_; }

    /**
     * Gets the modified effect type.
     *
     * @return The effect type which will be modified on the mob.
     */
    [[nodiscard]] EffectId getModifiedType() const { return modified_type_; }

    /**
     * Gets the old effect of the changed type, which will be removed.
     *
     * @return The old effect, or std::nullopt if the mob did not have the changed effect type.
     */
    [[nodiscard]] const std::optional<Effect> &getOldEffect() const { return old_effect_; }

    /**
     * Gets the new effect of the changed type to be applied.
     *
     * @return The new effect, or std::nullopt if the effect of the changed type will be removed.
     */
    [[nodiscard]] const std::optional<Effect> &getNewEffect() const { return new_effect_; }

private:
    Action action_;
    EffectId modified_type_;
    std::optional<Effect> old_effect_;
    std::optional<Effect> new_effect_;
};

}  // namespace endstone
