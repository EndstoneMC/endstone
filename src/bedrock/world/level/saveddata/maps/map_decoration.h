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

#include "bedrock/core/math/color.h"

class MapDecoration {
public:
    enum class Type : int8_t {
        MarkerWhite = 0,
        MarkerGreen = 1,
        MarkerRed = 2,
        MarkerBlue = 3,
        XWhite = 4,
        TriangleRed = 5,
        SquareWhite = 6,
        MarkerSign = 7,
        MarkerPink = 8,
        MarkerOrange = 9,
        MarkerYellow = 10,
        MarkerTeal = 11,
        TriangleGreen = 12,
        SmallSquareWhite = 13,
        Mansion = 14,
        Monument = 15,
        NoDraw = 16,
        VillageDesert = 17,
        VillagePlains = 18,
        VillageSavanna = 19,
        VillageSnowy = 20,
        VillageTaiga = 21,
        JungleTemple = 22,
        WitchHut = 23,
        TrialChambers = 24,
        Count = 25,
        // Define the style for different objects
        Player = MarkerWhite,
        PlayerOffMap = SquareWhite,
        PlayerOffLimits = SmallSquareWhite,
        PlayerHidden = NoDraw,
        ItemFrame = MarkerGreen,
    };

    MapDecoration();
    MapDecoration(Type, int8_t, int8_t, int8_t, const std::string &, const Color &);
    MapDecoration(const MapDecoration &);
    MapDecoration &operator=(const MapDecoration &);
    [[nodiscard]] Type getImg() const;
    void setImg(Type);
    [[nodiscard]] std::int8_t getX() const;
    void setX(int8_t);
    [[nodiscard]] std::int8_t getY() const;
    void setY(int8_t);
    [[nodiscard]] std::int8_t getRot() const;
    void setRot(int8_t);
    [[nodiscard]] bool equals(const MapDecoration &) const;
    [[nodiscard]] bool isRenderedOnFrame() const;
    int hashCode();
    [[nodiscard]] const std::string &getLabel() const;
    void setLabel(const std::string &);
    [[nodiscard]] const Color &getColor() const;
    void setColor(const Color &);

private:
    std::string label_;
    Type image_;
    int8_t x;
    int8_t y;
    int8_t rotation_;
    Color color;
};
