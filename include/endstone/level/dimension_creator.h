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

#include "endstone/identifier.h"

namespace endstone {

using DimensionId = Identifier<class Dimension>;

/**
 * Represents the options that may be used to create a custom Dimension.
 */
class DimensionCreator {
public:
    /**
     * Creates an instance of a DimensionCreator for the given dimension id.
     *
     * @param id the identifier of the dimension to create, e.g. `myplugin:void_realm`
     */
    explicit DimensionCreator(DimensionId id) : id_(std::string(id)) {}

    /**
     * Gets the identifier of the dimension that is being created.
     *
     * @return the dimension identifier
     */
    [[nodiscard]] DimensionId getId() const
    {
        return id_;
    }

    /**
     * Sets the identifier of the dimension that is being created.
     *
     * @param id the dimension identifier
     * @return this DimensionCreator, for chaining
     */
    DimensionCreator &id(DimensionId id)
    {
        id_ = std::string(id);
        return *this;
    }

private:
    std::string id_;
};

}  // namespace endstone
