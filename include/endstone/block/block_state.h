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

#include <fmt/format.h>

#include "endstone/block/block.h"
#include "endstone/util/result.h"

namespace endstone {

/**
 * @brief Represents a captured state of a block, which will not update automatically.
 *
 * <p>
 * Unlike Block, which only one object can exist per coordinate, BlockState can exist multiple times for any given
 * Block. Note that another plugin may change the state of the block, and you will not know, or they may change the
 * block to another type entirely, causing your BlockState to become invalid.
 */
class BlockState {
public:
    virtual ~BlockState() = default;

    /**
     * @brief Gets the block represented by this block state.
     *
     * @return the block represented by this block state
     */
    [[nodiscard]] virtual std::unique_ptr<Block> getBlock() const = 0;

    /**
     * @brief Gets the type of this block state.
     *
     * @return block type
     */
    [[nodiscard]] virtual std::string getType() const = 0;

    /**
     * @brief Sets the type of this block state.
     *
     * @param type Block type to change this block state to
     */
    virtual void setType(std::string type) = 0;

    /**
     * @brief Gets the data for this block state.
     *
     * @return block specific data
     */
    [[nodiscard]] virtual std::unique_ptr<BlockData> getData() const = 0;

    /**
     * @brief Sets the data for this block state.
     *
     * @param data New block specific data
     */
    virtual void setData(const BlockData &data) = 0;

    /**
     * @brief Gets the dimension which contains the block represented by this block state.
     *
     * @return the dimension containing the block represented by this block state
     */
    [[nodiscard]] virtual Dimension &getDimension() const = 0;

    /**
     * @brief Gets the x-coordinate of this block state.
     *
     * @return x-coordinate
     */
    [[nodiscard]] virtual int getX() const = 0;

    /**
     * @brief Gets the y-coordinate of this block state.
     *
     * @return y-coordinate
     */
    [[nodiscard]] virtual int getY() const = 0;

    /**
     * @brief Gets the z-coordinate of this block state.
     *
     * @return z-coordinate
     */
    [[nodiscard]] virtual int getZ() const = 0;

    /**
     * @brief Gets the location of this block state.
     *
     * @return the location
     */
    [[nodiscard]] virtual Location getLocation() const = 0;

    /**
     * @brief Attempts to update the block represented by this state, setting it to yhe new values as defined by this
     * state. <p> This has the same effect as calling update(false). That is to say, this will not modify the state of a
     * block if it is no longer the same type as it was when this state was taken. It will return false in this
     * eventuality.
     *
     * @return true if the update was successful, otherwise false
     */
    virtual bool update() = 0;

    /**
     * @brief Attempts to update the block represented by this state, setting it to the new values as defined by this
     * state. <p> This has the same effect as calling update(force, true). That is to say, this will trigger a physics
     * update to surrounding blocks.
     *
     * @param force true to forcefully set the state
     * @return true if the update was successful, otherwise false
     */
    virtual bool update(bool force) = 0;

    /**
     * @brief Attempts to update the block represented by this state, setting it to the new values as defined by this
     * state. <p> Unless force is true, this will not modify the state of a block if it is no longer the same type as it
     * was when this state was taken. It will return false in this eventuality. <p> If force is true, it will set the
     * type of the block to match the new state, set the state data and then return true. <p> If apply_physics is true,
     * it will trigger a physics update on surrounding blocks which could cause them to update or disappear.
     *
     * @param force true to forcefully set the state
     * @param apply_physics false to cancel updating physics on surrounding blocks
     * @return true if the update was successful, otherwise false
     */
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
        return fmt::format_to(ctx.out(), "BlockState(pos=BlockPos(x={}, y={}, z={}), type={}, data={})", val.getX(),
                              val.getY(), val.getZ(), val.getType(), *val.getData());
    }
};
}  // namespace fmt
