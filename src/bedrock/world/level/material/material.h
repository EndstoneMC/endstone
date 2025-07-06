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

enum class MaterialType : unsigned int {
    Air = 0,
    Dirt = 1,
    Wood = 2,
    Metal = 3,
    Grate = 4,
    Water = 5,
    Lava = 6,
    Leaves = 7,
    Plant = 8,
    SolidPlant = 9,
    Fire = 10,
    Glass = 11,
    Explosive = 12,
    Ice = 13,
    PowderSnow = 14,
    Cactus = 15,
    Portal = 16,
    StoneDecoration = 17,
    Bubble = 18,
    Barrier = 19,
    DecorationSolid = 20,
    ClientRequestPlaceholder = 21,
    StructureVoid = 22,
    Solid = 23,
    NonSolid = 24,
    Any = 25,
};

class Material {
    enum class Settings : int {
        Solid = 0,
        Liquid = 1,
        NonSolid = 2,
    };

public:
    explicit Material(MaterialType, Material::Settings);
    [[nodiscard]] bool isType(MaterialType type) const
    {
        return type == type_;
    }
    [[nodiscard]] bool isSolidBlocking() const
    {
        return !never_buildable_ && blocks_motions_;
    }
    [[nodiscard]] bool isLiquid() const
    {
        return liquid_;
    }
    [[nodiscard]] bool getBlocksMotion() const
    {
        return blocks_motions_;
    }
    [[nodiscard]] bool getBlocksPrecipitation() const
    {
        return blocks_precipitation_;
    }
    [[nodiscard]] bool isSolid() const
    {
        return solid_;
    }
    [[nodiscard]] bool isSuperHot() const
    {
        return super_hot_;
    }
    [[nodiscard]] bool isTopSolid(bool flag1, bool flag2) const
    {
        if (isType(MaterialType::Leaves)) {
            return flag2;
        }
        if (!flag1 || !isType(MaterialType::Water)) {
            return getBlocksMotion();
        }
        return true;
    }
    [[nodiscard]] MaterialType getType() const
    {
        return type_;
    }

private:
    MaterialType type_;
    bool never_buildable_;
    bool liquid_;
    bool blocks_motions_;
    bool blocks_precipitation_;
    bool solid_;
    bool super_hot_;
};
static_assert(sizeof(Material) == 12);
