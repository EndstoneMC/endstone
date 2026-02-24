// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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
#include <string_view>

#include "endstone/actor/actor.h"

namespace endstone {

/**
 * @brief Represents a source of damage.
 */
class DamageSource {
public:
    virtual ~DamageSource() = default;

    /**
     * @brief Get the damage type
     *
     * @return the damage type
     */
    [[nodiscard]] virtual std::string_view getType() const = 0;

    /**
     * @brief Get the actor that caused the damage to occur.
     *
     * Not to be confused with DamageSource::getDamagingActor(), the returned actor is the actor to which the damage is
     * ultimately attributed if the receiver is killed. If, for example, the receiver was damaged by a projectile, the
     * shooter/thrower would be returned.
     *
     * @return an Actor or null
     */
    [[nodiscard]] virtual Actor *getActor() const = 0;

    /**
     * @brief Get the actor that directly caused the damage.
     *
     * Not to be confused with DamageSource::getActor(), the returned actor is the actor that actually inflicted the
     * damage. If, for example, the receiver was damaged by a projectile, the projectile would be returned.
     *
     * @return an Actor or null
     */
    [[nodiscard]] virtual Actor *getDamagingActor() const = 0;

    /**
     * @brief Get if this damage is indirect.
     *
     * Damage is considered indirect if DamageSource::getActor() is not equal to DamageSource::getDamagingActor(). This
     * will be the case, for example, if a skeleton shot an arrow or a player threw a potion.
     *
     * @return true if is indirect, false otherwise.
     */
    [[nodiscard]] virtual bool isIndirect() const = 0;
};

}  // namespace endstone

namespace fmt {
template <>
struct formatter<endstone::DamageSource> : formatter<string_view> {
    using Type = endstone::DamageSource;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        auto it = ctx.out();
        it = fmt::format_to(it, "DamageSource(type={}", val.getType());
        if (auto *actor = val.getActor()) {
            it = fmt::format_to(it, ", actor={}", *actor);
        }
        else {
            it = fmt::format_to(it, ", actor=None");
        }
        if (auto *damaging_actor = val.getDamagingActor()) {
            it = fmt::format_to(it, ", damaging_actor={}", *damaging_actor);
        }
        else {
            it = fmt::format_to(it, ", damaging_actor=None");
        }
        it = fmt::format_to(it, ")");
        return it;
    }
};
}  // namespace fmt
