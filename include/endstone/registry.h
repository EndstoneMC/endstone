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

namespace endstone {

/**
 * @brief Abstract registry interface for keyed objects.
 *
 * Provides lookup by key (nullable or exception-throwing) and iteration.
 *
 * @tparam T Must satisfy Keyed, i.e. implement getKey().
 */
template <typename T>
class Registry {
public:
    virtual ~Registry() = default;

    /**
     * @brief Get the object by its key.
     *
     * Returns a raw pointer to the object, or nullptr if not found.
     *
     * @param key Non-null key to look up.
     * @return T* Pointer to the object, or nullptr if it does not exist.
     */
    virtual T *get(const std::string & key) noexcept = 0;

    /**
     * @brief Const overload of get().
     *
     * @param key Non-null key to look up.
     * @return const T* Pointer to the object, or nullptr if it does not exist.
     */
    virtual const T *get(const std::string & key) const noexcept = 0;

    /**
     * @brief Get the object by its key or throw if missing.
     *
     * @param key Key of the object to retrieve.
     * @return T& Reference to the object with the given key.
     * @throws std::invalid_argument if no object with the given key exists.
     */
    virtual T &getOrThrow(const std::string & key)
    {
        if (auto *p = get(key)) {
            return *p;
        }
        throw std::invalid_argument(fmt::format("No registry entry found for key: {}", key));
    }

    /**
     * @brief Const overload of getOrThrow().
     *
     * @param key Key of the object to retrieve.
     * @return const T& Const reference to the object with the given key.
     * @throws std::invalid_argument if no object with the given key exists.
     */
    virtual const T &getOrThrow(const std::string & key) const
    {
        if (auto *p = get(key)) {
            return *p;
        }
        throw std::invalid_argument(fmt::format("No registry entry found for key: {}", key));
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
