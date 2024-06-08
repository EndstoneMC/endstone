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

namespace endstone {

/**
 * @brief Represents a dimension within a Level.
 */
class Dimension {
public:
    /**
     * @brief Represents various dimension types.
     */
    enum class Type {
        Overworld = 0,
        Nether = 1,
        TheEnd = 2,
        Custom = 999
    };

    virtual ~Dimension() = default;

    /**
     * @brief Gets the name of this dimension
     *
     * @return Name of this dimension
     */
    [[nodiscard]] virtual std::string getName() const = 0;

    /**
     * @brief Gets the type of this dimension
     *
     * @return Type of this dimension
     */
    [[nodiscard]] virtual Type getType() const = 0;

    /**
     * @brief Gets the level to which this dimension belongs
     *
     * @return Level containing this dimension.
     */
    [[nodiscard]] virtual Level &getLevel() const = 0;
};
}  // namespace endstone
