

# File actor.h

[**File List**](files.md) **>** [**actor**](dir_dd7779a583e02d88c9a89a2c881c3946.md) **>** [**actor.h**](actor_8h.md)

[Go to the documentation of this file](actor_8h.md)


```C++
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

#include "endstone/command/command_sender.h"
#include "endstone/level/location.h"
#include "endstone/level/position.h"

namespace endstone {
class Item;
class Mob;
class Actor : public CommandSender {
public:
    [[nodiscard]] virtual Mob *asMob() const = 0;

    [[nodiscard]] virtual Item *asItem() const = 0;

    [[nodiscard]] virtual std::string getType() const = 0;

    [[nodiscard]] virtual std::uint64_t getRuntimeId() const = 0;

    [[nodiscard]] virtual Location getLocation() const = 0;

    [[nodiscard]] virtual Vector<float> getVelocity() const = 0;

    [[nodiscard]] virtual bool isOnGround() const = 0;

    [[nodiscard]] virtual bool isInWater() const = 0;

    [[nodiscard]] virtual bool isInLava() const = 0;

    [[nodiscard]] virtual Level &getLevel() const = 0;

    [[nodiscard]] virtual Dimension &getDimension() const = 0;

    virtual void setRotation(float yaw, float pitch) = 0;

    virtual void teleport(Location location) = 0;

    virtual void teleport(Actor &target) = 0;

    [[nodiscard]] virtual std::int64_t getId() const = 0;

    virtual void remove() = 0;

    [[nodiscard]] virtual bool isDead() const = 0;

    [[nodiscard]] virtual bool isValid() const = 0;

    [[nodiscard]] virtual int getHealth() const = 0;

    [[nodiscard]] virtual Result<void> setHealth(int health) const = 0;

    [[nodiscard]] virtual int getMaxHealth() const = 0;

    [[nodiscard]] virtual std::vector<std::string> getScoreboardTags() const = 0;

    [[nodiscard]] virtual bool addScoreboardTag(std::string tag) const = 0;

    [[nodiscard]] virtual bool removeScoreboardTag(std::string tag) const = 0;

    [[nodiscard]] virtual bool isNameTagVisible() const = 0;

    virtual void setNameTagVisible(bool visible) = 0;

    [[nodiscard]] virtual bool isNameTagAlwaysVisible() const = 0;

    virtual void setNameTagAlwaysVisible(bool visible) = 0;

    [[nodiscard]] virtual std::string getNameTag() const = 0;

    virtual void setNameTag(std::string name) = 0;

    [[nodiscard]] virtual std::string getScoreTag() const = 0;

    virtual void setScoreTag(std::string score) = 0;
};

}  // namespace endstone

namespace fmt {
template <>
struct formatter<endstone::Actor> : formatter<string_view> {
    using Type = endstone::Actor;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "{}", val.getName());
    }
};
}  // namespace fmt
```


