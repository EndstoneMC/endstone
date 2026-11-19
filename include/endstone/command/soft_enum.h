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

namespace endstone {

/**
 * Represents a soft enum, a set of command completions that can change while the server runs.
 */
class SoftEnum {
public:
    virtual ~SoftEnum() = default;

    /**
     * Gets the name the client sees, unique across the server.
     *
     * @return the name of this soft enum
     */
    [[nodiscard]] virtual std::string getName() const = 0;

    /**
     * Gets the values of this soft enum.
     *
     * @return the values of this soft enum
     * @throws std::runtime_error if the plugin that owned this soft enum has removed it
     */
    [[nodiscard]] virtual std::vector<std::string> getValues() const = 0;

    /**
     * Replaces the values of this soft enum and updates every client's completion at once.
     *
     * @param values the new values
     * @throws std::invalid_argument if this soft enum is read-only or a value is empty or holds whitespace
     * @throws std::runtime_error if the plugin that owned this soft enum has removed it
     */
    virtual void setValues(std::vector<std::string> values) = 0;

    /**
     * Adds values to this soft enum and updates every client's completion at once.
     *
     * @param values the values to add
     * @throws std::invalid_argument if this soft enum is read-only or a value is empty or holds whitespace
     * @throws std::runtime_error if the plugin that owned this soft enum has removed it
     */
    virtual void addValues(std::vector<std::string> values) = 0;

    /**
     * Removes values from this soft enum and updates every client's completion at once.
     *
     * @param values the values to remove
     * @throws std::invalid_argument if this soft enum is read-only or a value is empty or holds whitespace
     * @throws std::runtime_error if the plugin that owned this soft enum has removed it
     */
    virtual void removeValues(std::vector<std::string> values) = 0;
};

}  // namespace endstone
