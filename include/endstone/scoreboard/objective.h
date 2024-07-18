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

namespace endstone {

/**
 * @brief Represents an objective on a scoreboard that can show scores specific to entries.
 */
class Objective {
public:
    virtual ~Objective() = default;

    /**
     * @brief Gets the name of this Objective
     *
     * @return this objective's name
     */
    [[nodiscard]] virtual std::string getName() const = 0;

    /**
     * @brief Gets the name displayed to players for this objective
     *
     * @return this objective's display name
     */
    [[nodiscard]] virtual std::string getDisplayName() const = 0;

    /**
     * @brief Determines if the Objective is valid.
     *
     * @return true if the Objective is valid, false otherwise
     */
    [[nodiscard]] virtual bool isValid() const = 0;
};

}  // namespace endstone
