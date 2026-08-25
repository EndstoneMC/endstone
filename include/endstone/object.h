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

#include <cstddef>
#include <cstring>
#include <functional>
#include <type_traits>
#include <typeinfo>

namespace endstone {

/**
 * A type identity that stays comparable across shared library boundaries.
 *
 * libc++ compares `std::type_info` by address, and a type named from more than one shared library has one typeinfo
 * per module once those modules are built with hidden visibility, as endstone and its plugins are. Comparing the
 * mangled name instead lets a type named in a plugin match the same type named in the runtime.
 */
class ClassInfo {
public:
    ClassInfo(const std::type_info &info) noexcept : info_(&info) {}  // NOLINT(*-explicit-constructor)

    /**
     * The mangled name of the type.
     */
    [[nodiscard]] const char *name() const noexcept { return info_->name(); }

    /**
     * The underlying `std::type_info`.
     */
    [[nodiscard]] const std::type_info &info() const noexcept { return *info_; }

    bool operator==(const ClassInfo &other) const noexcept
    {
        return info_ == other.info_ || std::strcmp(name(), other.name()) == 0;
    }

private:
    const std::type_info *info_;
};

/**
 * Base class providing runtime type identification and safe casting.
 *
 * All endstone types that support runtime type narrowing (e.g., Permissible, ItemMeta)
 * inherit from Object. Subclasses implement getClassInfo() and isInstanceOf(),
 * and the type hierarchy is registered with entt::meta at startup.
 */
class Object {
public:
    virtual ~Object() = default;

    /**
     * @internal For internal use only. Prefer is<T>() and as<T>() instead.
     */
    [[nodiscard]] virtual ClassInfo getClassInfo() const = 0;

    /**
     * @internal For internal use only. Prefer is<T>() and as<T>() instead.
     */
    [[nodiscard]] virtual bool isInstanceOf(ClassInfo target) const = 0;

    /**
     * Attempts to cast this object to the given type T.
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
     * Checks if this object is an instance of the given type T (or a subclass of T).
     *
     * @tparam T Type to check against (must derive from Object)
     * @return `true` if this object is an instance of T
     */
    template <typename T>
        requires std::is_base_of_v<Object, T>
    [[nodiscard]] bool is() const
    {
        return isInstanceOf(typeid(T));
    }
};

}  // namespace endstone

template <>
struct std::hash<endstone::ClassInfo> {
    std::size_t operator()(const endstone::ClassInfo &info) const noexcept
    {
        std::size_t hash = 5381;
        for (const auto *ptr = info.name(); *ptr != '\0'; ++ptr) {
            hash = hash * 33 ^ static_cast<unsigned char>(*ptr);
        }
        return hash;
    }
};
