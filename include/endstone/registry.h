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

#include <concepts>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

namespace endstone {

/**
 * @brief Concept that requires T to provide a key via getKey().
 *
 * @tparam T Type that must implement getKey() returning std::string_view.
 */
template <typename T>
concept Keyed = requires(const T &t) {
    {
        t.getKey()
    } -> std::convertible_to<std::string_view>;
};

/**
 * @brief Abstract registry interface for keyed objects.
 *
 * Provides lookup by key (nullable or exception-throwing) and iteration.
 *
 * @tparam T Must satisfy Keyed, i.e. implement getKey().
 */
template <Keyed T>
class Registry {
public:
    using key_type = std::string_view;
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;

protected:
    /// Underlying storage type
    using storage_type = std::unordered_map<key_type, T>;

public:
    /// Iterator over stored elements
    using iterator = typename storage_type::iterator;
    /// Const iterator over stored elements
    using const_iterator = typename storage_type::const_iterator;

    virtual ~Registry() = default;

    /**
     * @brief Get the object by its key.
     *
     * Returns a raw pointer to the object, or nullptr if not found.
     *
     * @param key Non-null key to look up.
     * @return T* Pointer to the object, or nullptr if it does not exist.
     */
    virtual T *get(key_type key) noexcept = 0;

    /**
     * @brief Const overload of get().
     *
     * @param key Non-null key to look up.
     * @return const T* Pointer to the object, or nullptr if it does not exist.
     */
    virtual const T *get(key_type key) const noexcept = 0;

    /**
     * @brief Get the object by its key or throw if missing.
     *
     * @param key Key of the object to retrieve.
     * @return T& Reference to the object with the given key.
     * @throws std::invalid_argument if no object with the given key exists.
     */
    virtual T &getOrThrow(key_type key)
    {
        if (auto *p = get(key)) {
            return *p;
        }
        throw std::invalid_argument(std::string{"No object with key: "} + std::string{key});
    }

    /**
     * @brief Const overload of getOrThrow().
     *
     * @param key Key of the object to retrieve.
     * @return const T& Const reference to the object with the given key.
     * @throws std::invalid_argument if no object with the given key exists.
     */
    virtual const T &getOrThrow(key_type key) const
    {
        if (auto *p = get(key)) {
            return *p;
        }
        throw std::invalid_argument(std::string{"No object with key: "} + std::string{key});
    }

    /**
     * @brief Returns iterator to beginning of registry items.
     *
     * Enables range-based for loops.
     *
     * @return iterator Begin iterator.
     */
    virtual iterator begin() = 0;

    /**
     * @brief Returns iterator to end of registry items.
     *
     * @return iterator End iterator.
     */
    virtual iterator end() = 0;

    /**
     * @brief Const begin iterator.
     *
     * @return const_iterator Const begin iterator.
     */
    virtual const_iterator begin() const = 0;

    /**
     * @brief Const end iterator.
     *
     * @return const_iterator Const end iterator.
     */
    virtual const_iterator end() const = 0;
};
}  // namespace endstone
