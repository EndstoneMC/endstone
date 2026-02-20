

# File map\_cursor.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**map**](dir_35fd4abc90217931459f3a8776f2bf4e.md) **>** [**map\_cursor.h**](map__cursor_8h.md)

[Go to the documentation of this file](map__cursor_8h.md)


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

#include <cstdint>
#include <string>
#include <utility>

#include "endstone/check.h"

namespace endstone {
class MapCursor {
public:
    enum class Type {
        Player = 0,
        Frame = 1,
        RedMarker = 2,
        BlueMarker = 3,
        TargetX = 4,
        TargetPoint = 5,
        PlayerOffMap = 6,
        SignMarker = 7,
        PinkMarker = 8,
        OrangeMarker = 9,
        YellowMarker = 10,
        CyanMarker = 11,
        GreenPoint = 12,
        PlayerOffLimits = 13,
        Mansion = 14,
        Monument = 15,
        // PlayerHidden = 16,
        VillageDesert = 17,
        VillagePlains = 18,
        VillageSavanna = 19,
        VillageSnowy = 20,
        VillageTaiga = 21,
        JungleTemple = 22,
        SwampHut = 23,
        TrialChambers = 24,
    };

    MapCursor(std::int8_t x, std::int8_t y, std::int8_t direction, Type type, bool visible, std::string caption = "")
        : x_(x), y_(y), direction_(direction), visible_(visible), caption_(std::move(caption)), type_(type)
    {
    }

    [[nodiscard]] std::int8_t getX() const { return x_; }

    [[nodiscard]] std::int8_t getY() const { return y_; }

    [[nodiscard]] std::int8_t getDirection() const { return direction_; }

    [[nodiscard]] Type getType() const { return type_; }

    [[nodiscard]] bool isVisible() const { return visible_; }

    void setX(const std::int8_t x) { x_ = x; }

    void setY(const std::int8_t y) { y_ = y; }

    void setDirection(std::int8_t direction)
    {
        Preconditions::checkArgument(direction >= 0 && direction <= 15, "direction must be between 0 and 15 but is {}",
                                     direction);
        direction_ = direction;
    }

    void setType(Type type) { type_ = type; }

    void setVisible(bool visible) { visible_ = visible; }

    [[nodiscard]] std::string getCaption() const { return caption_; }

    void setCaption(std::string caption) { caption_ = std::move(caption); }

private:
    std::int8_t x_, y_;
    std::int8_t direction_;
    bool visible_;
    std::string caption_;
    Type type_;
};
}  // namespace endstone
```


