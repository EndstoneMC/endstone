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

#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/util/random.h"

class Randomize {
public:
    Randomize(IRandom &);
    virtual ~Randomize() = default;
    [[nodiscard]] virtual bool chance(int, int) const = 0;
    [[nodiscard]] virtual bool chanceOneIn(int) const = 0;
    [[nodiscard]] virtual bool chanceAllButOneIn(int) const = 0;
    [[nodiscard]] virtual bool chanceFloatLessThan(float) const = 0;
    [[nodiscard]] virtual bool chanceFloatLessOrEqual(float) const = 0;
    [[nodiscard]] virtual bool chanceFloatGreaterThan(float) const = 0;
    [[nodiscard]] virtual bool chanceFloatGreaterOrEqual(float) const = 0;
    [[nodiscard]] virtual float nextFloat() const = 0;
    [[nodiscard]] virtual int nextIntInclusive(int, int) const = 0;
    [[nodiscard]] virtual int nextInt(int) const = 0;

protected:
    Randomize();
    Bedrock::NonOwnerPointer<IRandom> random_;
};
static_assert(sizeof(Randomize) == 32);
