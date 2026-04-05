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

#include <type_traits>
#include <typeinfo>

namespace endstone {

/**
 * @brief Base class providing runtime type identification and safe casting.
 *
 * All endstone types that support runtime type narrowing (e.g., Permissible, ItemMeta)
 * inherit from Object. Subclasses implement getClassTypeId() and isInstanceOf(),
 * and the type hierarchy is registered with entt::meta at startup.
 */
class Object {
public:
    virtual ~Object() = default;

    /**
     * @internal For internal use only. Prefer is<T>() and as<T>() instead.
     */
    [[nodiscard]] virtual const std::type_info &getClassTypeId() const = 0;

    /**
     * @internal For internal use only. Prefer is<T>() and as<T>() instead.
     */
    [[nodiscard]] virtual bool isInstanceOf(const std::type_info &target) const = 0;

    /**
     * @brief Attempts to cast this object to the given type T.
     *
     * Returns nullptr if this object is not an instance of T (or a subclass of T).
     *
     * @tparam T Target type to cast to (must derive from Object)
     * @return Pointer to this as T, or nullptr if the cast is invalid
     */
    template <typename T>
        requires std::is_base_of_v<Object, T>
    T *as()
    {
        if (isInstanceOf(typeid(T))) {
            return static_cast<T *>(this);
        }
        return nullptr;
    }

    /**
     * @copydoc as()
     */
    template <typename T>
        requires std::is_base_of_v<Object, T>
    const T *as() const
    {
        if (isInstanceOf(typeid(T))) {
            return static_cast<const T *>(this);
        }
        return nullptr;
    }

    /**
     * @brief Checks if this object is an instance of the given type T (or a subclass of T).
     *
     * @tparam T Type to check against (must derive from Object)
     * @return True if this object is an instance of T
     */
    template <typename T>
        requires std::is_base_of_v<Object, T>
    [[nodiscard]] bool is() const
    {
        return isInstanceOf(typeid(T));
    }
};

}  // namespace endstone
