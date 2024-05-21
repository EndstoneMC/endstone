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

#include <unordered_map>

#include "bedrock/world/level/dimension/dimension.h"
#include "bedrock/world/level/level.h"
#include "endstone/actor/actor.h"
#include "endstone/detail/server.h"
#include "endstone/level/dimension.h"
#include "endstone/level/level.h"

namespace endstone::detail {

class EndstoneDimension : public endstone::Dimension {
public:
    explicit EndstoneDimension(::Dimension &dimension, Level &level);
    ~EndstoneDimension() override = default;
    [[nodiscard]] std::string getName() const override;
    [[nodiscard]] Type getType() const override;
    [[nodiscard]] Level &getLevel() const override;

private:
    ::Dimension &dimension_;
    Level &level_;
};

}  // namespace endstone::detail
