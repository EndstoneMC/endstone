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

#include "endstone/actor/actor.h"

namespace endstone {

/**
 * @brief Represents a chunk of blocks.
 */
class Chunk {
public:
    virtual ~Chunk() = default;

    /**
     * @brief Gets the X-coordinate of this chunk
     *
     * @return X-coordinate
     */
    [[nodiscard]] virtual int getX() const = 0;

    /**
     * @brief Gets the Z-coordinate of this chunk
     *
     * @return Z-coordinate
     */
    [[nodiscard]] virtual int getZ() const = 0;

    /**
     * @brief Gets the level containing this chunk
     *
     * @return Parent Level
     */
    [[nodiscard]] virtual Level &getLevel() const = 0;

    /**
     * @brief Gets the dimension containing this chunk
     *
     * @return Parent Dimension
     */
    [[nodiscard]] virtual Dimension &getDimension() const = 0;
};

}  // namespace endstone

template <>
struct fmt::formatter<endstone::Chunk> : formatter<string_view> {
    template <typename FormatContext>
    auto format(const endstone::Chunk &self, FormatContext &ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "Chunk(x={}, z={})", self.getX(), self.getZ());
    }
};
