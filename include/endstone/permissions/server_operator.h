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
 * @brief Represents an object that may become a server operator, such as a Player
 */
class ServerOperator {
public:
    virtual ~ServerOperator() = default;

    /**
     * @brief Checks if this object is a server operator
     *
     * @return true if this is an operator, otherwise false
     */
    virtual bool isOp() const = 0;

    /**
     * @brief Sets the operator status of this object
     *
     * @param value New operator value
     */
    virtual void setOp(bool value) = 0;
};
}  // namespace endstone
