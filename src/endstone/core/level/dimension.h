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

#include "bedrock/world/level/dimension/dimension.h"
#include "endstone/actor/actor.h"
#include "endstone/core/server.h"
#include "endstone/level/dimension.h"

namespace endstone::core {
class EndstoneDimension : public Dimension {
public:
    explicit EndstoneDimension( WeakRef<::Dimension> dimension, EndstoneLevel &level);
    ~EndstoneDimension() override = default;
    [[nodiscard]] std::string getName() const override;
    [[nodiscard]] Type getType() const override;
    [[nodiscard]] Level &getLevel() const override;
    [[nodiscard]] std::unique_ptr<Block> getBlockAt(int x, int y, int z) const override;
    [[nodiscard]] std::unique_ptr<Block> getBlockAt(Location location) const override;
    [[nodiscard]] int getHighestBlockYAt(int x, int z) const override;
    [[nodiscard]] std::unique_ptr<Block> getHighestBlockAt(int x, int z) const override;
    [[nodiscard]] std::unique_ptr<Block> getHighestBlockAt(Location location) const override;
    [[nodiscard]] std::vector<std::unique_ptr<Chunk>> getLoadedChunks() override;
    [[nodiscard]] Item &dropItem(Location location, const ItemStack &item) override;
    [[nodiscard]] Actor *spawnActor(Location location, std::string type) override;
    [[nodiscard]] std::vector<Actor *> getActors() const override;

    [[nodiscard]] ::Dimension &getHandle() const;

private:
    WeakRef<::Dimension> dimension_;
    EndstoneLevel &level_;
};
}  // namespace endstone::core
