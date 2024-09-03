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

#include "endstone/event/actor/actor_event.h"

namespace endstone {

/**
 * @brief Called when a living entity receives knockback.
 */
class ActorKnockbackEvent : public ActorEvent {
public:
    explicit ActorKnockbackEvent(Mob &mob, Actor *source, Vector<float> raw_knockback, Vector<float> knockback)
        : ActorEvent(mob), mob_(mob), source_(source), raw_knockback_(raw_knockback), knockback_(knockback)
    {
    }
    ~ActorKnockbackEvent() override = default;

    inline static const std::string NAME = "ActorKnockbackEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    [[nodiscard]] bool isCancellable() const override
    {
        return true;
    }

    /**
     * @brief Returns the Mob involved in this event
     *
     * @return Mob which is involved in this event
     */
    [[nodiscard]] Mob &getActor() const
    {
        return mob_;
    }

    /**
     * @brief Get the source actor that has caused knockback to the defender, if exists.
     *
     * @return actor that caused knockback, or nullptr if the knockback is not caused by an actor.
     */
    [[nodiscard]] Actor *getSource() const
    {
        return source_;
    }

    /**
     * @brief Gets the raw knockback that will be applied to the entity.
     *
     * <p>
     * This value is read-only, changes made to it <b>will not</b> have any effect on the final knockback received.
     *
     * @return the raw knockback
     */
    [[nodiscard]] Vector<float> getRawKnockback() const
    {
        return raw_knockback_;
    }

    /**
     * Gets the knockback that will be applied to the entity.
     *
     * <p>
     * <b>Note:</b> this method returns a copy, changes must be applied with setKnockback(Vector)
     *
     * @return the knockback
     */
    [[nodiscard]] Vector<float> getKnockback() const
    {
        return knockback_;
    }

    /**
     * @brief Sets the knockback that will be applied to the entity.
     *
     * @param knockback the knockback to apply
     */
    void setKnockback(Vector<float> knockback)
    {
        knockback_ = knockback;
    }

private:
    Mob &mob_;
    Actor *source_;
    Vector<float> raw_knockback_;
    Vector<float> knockback_;
};

}  // namespace endstone
