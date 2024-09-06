

# File block\_state.h

[**File List**](files.md) **>** [**block**](dir_faca67fc60a7463eb1bd30eabe023cf1.md) **>** [**block\_state.h**](block__state_8h.md)

[Go to the documentation of this file](block__state_8h.md)


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

#include <fmt/format.h>

#include "endstone/block/block.h"

namespace endstone {

class BlockState {
public:
    virtual ~BlockState() = default;

    [[nodiscard]] virtual std::unique_ptr<Block> getBlock() const = 0;

    [[nodiscard]] virtual std::string getType() const = 0;

    virtual void setType(std::string type) = 0;

    [[nodiscard]] virtual std::shared_ptr<BlockData> getData() const = 0;

    virtual void setData(std::shared_ptr<BlockData> data) = 0;

    [[nodiscard]] virtual Dimension &getDimension() const = 0;

    [[nodiscard]] virtual int getX() const = 0;

    [[nodiscard]] virtual int getY() const = 0;

    [[nodiscard]] virtual int getZ() const = 0;

    [[nodiscard]] virtual Location getLocation() const = 0;

    virtual bool update() = 0;

    virtual bool update(bool force) = 0;

    virtual bool update(bool force, bool apply_physics) = 0;
};
}  // namespace endstone

namespace fmt {
template <>
struct formatter<endstone::BlockState> : formatter<string_view> {
    using Type = endstone::BlockState;

    template <typename FormatContext>
    auto format(const Type &val, FormatContext &ctx) const -> format_context::iterator
    {
        return format_to(ctx.out(), "BlockState(pos=BlockPos(x={}, y={}, z={}), type={}, data={})", val.getX(),
                         val.getY(), val.getZ(), val.getType(), *val.getData());
    }
};
}  // namespace fmt
```


