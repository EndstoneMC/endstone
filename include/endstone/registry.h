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

#include <functional>
#include <stdexcept>

#include <fmt/format.h>

#include "identifier.h"

namespace endstone {
class IRegistry {
public:
    virtual ~IRegistry() = default;
};

/**
 * @brief Abstract registry interface for objects with identifiers.
 *
 * Provides lookup by identifier (nullable or exception-throwing) and iteration.
 *
 * @tparam T Must implement getId().
 */
template <typename T>
class Registry : public IRegistry {
public:
    /**
     * @brief Get the object by its identifier.
     *
     * Returns a raw pointer to the object, or nullptr if not found.
     *
     * @param id Identifier to look up.
     * @return T* Pointer to the object, or nullptr if it does not exist.
     */
    virtual T *get(Identifier<T> id) noexcept = 0;

    /**
     * @brief Get the object by its identifier.
     *
     * Returns a raw pointer to the object, or nullptr if not found.
     *
     * @param id Non-null identifier to look up.
     * @return const T* Pointer to the object, or nullptr if it does not exist.
     */
    virtual const T *get(Identifier<T> id) const noexcept = 0;

    /**
     * @brief Get the object by its identifier or throw if missing.
     *
     * @param id identifier of the object to retrieve.
     * @return T& Reference to the object with the given identifier.
     * @throws std::invalid_argument if no object with the given identifier exists.
     */
    virtual T &getOrThrow(Identifier<T> id)
    {
        if (auto *p = get(id)) {
            return *p;
        }
        throw std::invalid_argument(fmt::format("No registry entry found for identifier: {}", id));
    }

    /**
     * @brief Get the object by its identifier or throw if missing.
     *
     * @param id identifier of the object to retrieve.
     * @return const T& Const reference to the object with the given identifier.
     * @throws std::invalid_argument if no object with the given identifier exists.
     */
    virtual const T &getOrThrow(Identifier<T> id) const
    {
        if (auto *p = get(id)) {
            return *p;
        }
        throw std::invalid_argument(fmt::format("No registry entry found for identifier: {}", id));
    }

    /**
     * @brief Apply a function to each stored element.
     *
     * Iterates over all stored elements and applies the provided function to them.
     * The iteration continues until the function returns false or all elements
     * have been processed.
     *
     * @param func A callable object taking a const reference to an element
     *             and returning a boolean. Returning false stops the iteration.
     */
    virtual void forEach(std::function<bool(const T &)> func) const = 0;
};
}  // namespace endstone

#define ENDSTONE_REGISTRY_TYPE(type)                            \
    static constexpr auto RegistryType = #type;                 \
                                                                \
    static const type *get(Identifier<type> id)                 \
    {                                                           \
        return detail::getServer().getRegistry<type>().get(id); \
    }
