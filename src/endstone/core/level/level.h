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
#include "endstone/level/dimension.h"
#include "endstone/level/level.h"

namespace endstone::core {

class EndstoneServer;

class EndstoneLevel : public Level {
public:
    explicit EndstoneLevel(::Level &level);
    ~EndstoneLevel() override = default;

    [[nodiscard]] std::string getName() const override;
    [[nodiscard]] std::vector<Actor *> getActors() const override;
    [[nodiscard]] int getTime() const override;
    void setTime(int time) override;
    [[nodiscard]] std::vector<Dimension *> getDimensions() const override;
    [[nodiscard]] Dimension *getDimension(std::string name) const override;
    [[nodiscard]] Dimension *getDimension(int id) const;  // TODO(fixme): make it virtual
    [[nodiscard]] std::int64_t getSeed() const override;
    [[nodiscard]] std::variant<bool, int, float> getGameRuleValue(const GameRule &rule) const override;
    [[nodiscard]] bool setGameRuleValue(const GameRule &rule, const std::variant<bool, int, float> &value) override;

    [[nodiscard]] EndstoneServer &getServer() const;
    [[nodiscard]] ::Level &getHandle() const;

private:
    EndstoneServer &server_;
    ::Level &level_;
    std::map<int, std::unique_ptr<Dimension>> dimensions_;
};

}  // namespace endstone::core
