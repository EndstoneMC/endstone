

# File block.h

[**File List**](files.md) **>** [**block**](dir_faca67fc60a7463eb1bd30eabe023cf1.md) **>** [**block.h**](block_8h.md)

[Go to the documentation of this file](block_8h.md)


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

#include <memory>
#include <string>

#include "endstone/block/block_data.h"
#include "endstone/block/block_face.h"
#include "endstone/level/location.h"

namespace endstone {

class Dimension;

class Block {
public:
    virtual ~Block() = default;

    [[nodiscard]] virtual bool isValid() const = 0;

    [[nodiscard]] virtual std::string getType() const = 0;

    virtual void setType(std::string type) = 0;

    virtual void setType(std::string type, bool apply_physics) = 0;

    [[nodiscard]] virtual std::shared_ptr<BlockData> getData() const = 0;

    virtual void setData(std::shared_ptr<BlockData> data) = 0;

    virtual void setData(std::shared_ptr<BlockData> data, bool apply_physics) = 0;

    virtual std::unique_ptr<Block> getRelative(int offset_x, int offset_y, int offset_z) = 0;

    virtual std::unique_ptr<Block> getRelative(BlockFace face) = 0;

    virtual std::unique_ptr<Block> getRelative(BlockFace face, int distance) = 0;

    [[nodiscard]] virtual Dimension &getDimension() const = 0;

    [[nodiscard]] virtual int getX() const = 0;

    [[nodiscard]] virtual int getY() const = 0;

    [[nodiscard]] virtual int getZ() const = 0;

    [[nodiscard]] virtual Location getLocation() const = 0;
};

}  // namespace endstone

namespace fmt {
template <>
struct formatter<endstone::Block> : formatter<string_view> {
    using Type = endstone::Block;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return format_to(ctx.out(), "Block(pos=BlockPos(x={}, y={}, z={}), type={}, data={})", val.getX(), val.getY(),
                         val.getZ(), val.getType(), *val.getData());
    }
};
}  // namespace fmt
```


