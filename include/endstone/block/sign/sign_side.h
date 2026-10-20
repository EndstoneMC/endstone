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

#include <string>
#include <vector>

#include "endstone/util/color.h"

namespace endstone {

/**
 * Represents a side of a sign.
 */
class SignSide {
public:
    virtual ~SignSide() = default;

    /**
     * Gets all the lines of text currently on this side of the sign.
     *
     * @return the lines of text
     */
    [[nodiscard]] virtual std::vector<std::string> getLines() const = 0;

    /**
     * Gets the line of text at the specified index on this side of the sign.
     *
     * <p>
     * For example, `getLine(0)` will return the first line of text.
     *
     * @param index the index of the line to get, between 0 and 3
     * @return the text of the line, empty if the line is blank
     */
    [[nodiscard]] virtual std::string getLine(int index) const = 0;

    /**
     * Sets the line of text at the specified index on this side of the sign.
     *
     * <p>
     * For example, `setLine(0, "Line One")` will set the first line to "Line One".
     *
     * @param index the index of the line to set, between 0 and 3
     * @param line the new text to set, dropping anything after a line break
     */
    virtual void setLine(int index, std::string line) = 0;

    /**
     * Gets whether this side of the sign has glowing text.
     *
     * @return `true` if this side of the sign has glowing text
     */
    [[nodiscard]] virtual bool isGlowingText() const = 0;

    /**
     * Sets whether this side of the sign has glowing text.
     *
     * @param glowing whether this side of the sign has glowing text
     */
    virtual void setGlowingText(bool glowing) = 0;

    /**
     * Gets the color of this side of the sign.
     *
     * <p>
     * Bedrock stores the text color of a sign as a color rather than as one of the dyes, so this returns a Color where
     * Bukkit returns a DyeColor.
     *
     * @return the color
     */
    [[nodiscard]] virtual Color getColor() const = 0;

    /**
     * Sets the color of this side of the sign.
     *
     * @param color the color to set
     */
    virtual void setColor(Color color) = 0;
};

}  // namespace endstone
