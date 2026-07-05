

# File dimension.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**level**](dir_8e239ca1e5fd0d936d66a30330d3a329.md) **>** [**dimension.h**](dimension_8h.md)

[Go to the documentation of this file](dimension_8h.md)


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

#include <format>
#include <memory>
#include <string>
#include <vector>

#include "endstone/actor/actor.h"
#include "endstone/actor/item.h"
#include "endstone/block/block.h"
#include "endstone/identifier.h"
#include "endstone/inventory/item_stack.h"
#include "endstone/level/chunk.h"

namespace endstone {

class Dimension;
using DimensionId = Identifier<Dimension>;

class Dimension : public std::enable_shared_from_this<Dimension> {
public:
    static constexpr auto Overworld = DimensionId::minecraft("overworld");
    static constexpr auto Nether = DimensionId::minecraft("nether");
    static constexpr auto TheEnd = DimensionId::minecraft("the_end");

    virtual ~Dimension() = default;

    [[nodiscard]] virtual DimensionId getId() const = 0;

    [[nodiscard]] virtual std::string getTranslationKey() const = 0;

    [[nodiscard]] virtual Level &getLevel() const = 0;

    [[nodiscard]] virtual bool isValid() const = 0;

    [[nodiscard]] virtual std::unique_ptr<Block> getBlockAt(int x, int y, int z) const = 0;

    [[nodiscard]] virtual std::unique_ptr<Block> getBlockAt(Location location) const = 0;

    [[nodiscard]] virtual int getHighestBlockYAt(int x, int z) const = 0;

    [[nodiscard]] virtual std::unique_ptr<Block> getHighestBlockAt(int x, int z) const = 0;

    [[nodiscard]] virtual std::unique_ptr<Block> getHighestBlockAt(Location location) const = 0;

    [[nodiscard]] virtual std::vector<std::unique_ptr<Chunk>> getLoadedChunks() = 0;

    [[nodiscard]] virtual bool isChunkLoaded(int x, int z) const = 0;

    virtual bool loadChunk(int x, int z) = 0;

    virtual bool unloadChunk(int x, int z) = 0;

    [[nodiscard]] virtual Item &dropItem(Location location, const ItemStack &item) = 0;

    [[nodiscard]] virtual Actor *spawnActor(Location location, ActorTypeId type) = 0;

    [[nodiscard]] virtual std::vector<Actor *> getActors() const = 0;
};

inline Nullable<Dimension> Location::getDimension() const
{
    auto dimension = dimension_.lock();
    if (!dimension) {
        // The wrapper is gone: tell "never set" (empty weak_ptr) apart from a wrapper that has expired.
        const std::weak_ptr<Dimension> unset;
        const bool was_set = dimension_.owner_before(unset) || unset.owner_before(dimension_);
        Preconditions::checkArgument(!was_set, "Dimension unloaded");
        return nullptr;
    }
    // The wrapper is alive, but the underlying dimension it points to may have been unloaded.
    Preconditions::checkArgument(dimension->isValid(), "Dimension unloaded");
    return dimension;
}

inline bool Location::isDimensionLoaded() const
{
    const auto dimension = dimension_.lock();
    return dimension && dimension->isValid();
}

inline std::unique_ptr<Block> Location::getBlock() const
{
    return getDimension().value().getBlockAt(*this);
}

inline float Location::distanceSquared(const Location &other) const
{
    const auto dimension = getDimension();
    const auto other_dimension = other.getDimension();
    Preconditions::checkArgument(dimension != nullptr && other_dimension != nullptr,
                                 "Cannot measure distance to a null dimension.");
    Preconditions::checkArgument(dimension == other_dimension, "Cannot measure distance between {} and {}.",
                                 dimension.value().getId(), other_dimension.value().getId());
    return ((x_ - other.x_) * (x_ - other.x_)) + ((y_ - other.y_) * (y_ - other.y_)) +
           ((z_ - other.z_) * (z_ - other.z_));
}
}  // namespace endstone

template <>
struct std::formatter<endstone::Dimension> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const endstone::Dimension &self, FormatContext &ctx) const -> format_context::iterator
    {
        return std::format_to(ctx.out(), "Dimension(id={})", self.getId());
    }
};
```


