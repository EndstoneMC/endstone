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

#include "endstone/scoreboard/render_type.h"

namespace endstone {

/**
 * @brief Represents a scoreboard criteria.
 */
class Criteria {
public:
    enum class Type {
        /**
         * @brief The dummy criteria. Not changed by the server.
         */
        Dummy,
    };

    virtual ~Criteria() = default;

    /**
     * @brief Get the name of this criteria (its unique id).
     *
     * @return the name
     */
    [[nodiscard]] virtual std::string getName() const = 0;

    /**
     * @brief Get whether or not this criteria is read only. If read only, scoreboards with this criteria cannot have
     * their scores changed.
     *
     * @return true if read only, false otherwise
     */
    [[nodiscard]] virtual bool isReadOnly() const = 0;

    /**
     * Get the RenderType used by default for this criteria.
     *
     * @return the default render type
     */
    [[nodiscard]] virtual RenderType getDefaultRenderType() const = 0;
};

}  // namespace endstone
