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

#include "endstone/actor/actor.h"

namespace endstone {

class Level {
public:
    virtual ~Level() = default;

    /**
     * Gets the unique name of this level
     *
     * @return Name of this level
     */
    [[nodiscard]] virtual std::string getName() const = 0;

    /**
     * Get a list of all actors in this World
     *
     * @return A List of all Actors currently residing in this world
     */
    [[nodiscard]] virtual std::vector<Actor *> getActors() const = 0;

    /**
     * Gets the relative in-game time of this level.
     *
     * @return The current relative time
     */
    [[nodiscard]] virtual int getTime() const = 0;

    /**
     * Sets the relative in-game time on the server.
     *
     * @param time The new relative time to set the in-game time to
     */
    virtual void setTime(int time) = 0;
};

}  // namespace endstone
