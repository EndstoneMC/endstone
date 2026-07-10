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
#include <format>
#include <functional>
#include <stdexcept>
#include <string>

#include "detail.h"
#include "identifier.h"
#include "server.h"

namespace endstone {
namespace python {
class PyRegistry;
}

/**
 * Type-erased base interface for registries.
 *
 * Allows registries of any element type to be handled uniformly without knowing the
 * concrete element type. Prefer the typed Registry interface in plugin code.
 */
class IRegistry {
public:
    virtual ~IRegistry() = default;

    /**
     * Gets the number of entries in this registry.
     *
     * @return the entry count
     */
    [[nodiscard]] virtual std::size_t size() const = 0;

protected:
    friend class python::PyRegistry;
    [[nodiscard]] virtual const void *get0(std::string_view id) const noexcept = 0;
    virtual void forEach0(std::function<bool(const void *)> func) const = 0;
    [[nodiscard]] virtual const std::type_info &getTypeId() const noexcept = 0;
};

/**
 * Abstract registry interface for objects with identifiers.
 *
 * Provides lookup by identifier (nullable or exception-throwing) and iteration.
 *
 * @tparam T Must implement getId().
 */
template <typename T>
class Registry : public IRegistry {
public:
    /**
     * CRTP base for registry-backed types identified by an Identifier.
     *
     * Provides getId(), getTranslationKey() pure virtual declarations,
     * comparison operators, implicit conversion to Id, and a static get() method.
     */
    class Type {
    public:
        /**
         * The identifier type used to look up this object in its registry.
         */
        using Id = Identifier<T>;

        virtual ~Type() = default;

        /**
         * Return the identifier of this object.
         *
         * @return this object's identifier
         */
        [[nodiscard]] virtual Id getId() const = 0;

        /**
         * Get the translation key, suitable for use in a translation component.
         *
         * @return the translation key
         */
        [[nodiscard]] virtual std::string getTranslationKey() const = 0;

        /**
         * Look up a registry entry by identifier.
         *
         * @param id Identifier to look up.
         * @return Pointer to the entry, or nullptr if not found.
         */
        static const T *get(Id id) { return detail::getServer().getRegistry<T>().get(id); }

        bool operator==(const Id &other) const { return getId() == other; }
        bool operator!=(const Id &other) const { return !(*this == other); }
        bool operator==(const T &other) const { return getId() == other.getId(); }
        bool operator!=(const T &other) const { return !(*this == other); }
        operator Id() const { return getId(); }
    };

    /**
     * Get the object by its identifier.
     *
     * Returns a raw pointer to the object, or nullptr if not found.
     *
     * @param id Identifier to look up.
     * @return T* Pointer to the object, or nullptr if it does not exist.
     */
    virtual T *get(Identifier<T> id) noexcept = 0;

    /**
     * Get the object by its identifier.
     *
     * Returns a raw pointer to the object, or nullptr if not found.
     *
     * @param id Non-null identifier to look up.
     * @return const T* Pointer to the object, or nullptr if it does not exist.
     */
    virtual const T *get(Identifier<T> id) const noexcept = 0;

    /**
     * Get the object by its identifier or throw if missing.
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
        throw std::invalid_argument(std::format("No registry entry found for identifier: {}", id));
    }

    /**
     * Get the object by its identifier or throw if missing.
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
        throw std::invalid_argument(std::format("No registry entry found for identifier: {}", id));
    }

    /**
     * Apply a function to each stored element.
     *
     * Iterates over all stored elements and applies the provided function to them.
     * The iteration continues until the function returns false or all elements
     * have been processed.
     *
     * @param func A callable object taking a const reference to an element
     *             and returning a boolean. Returning false stops the iteration.
     */
    virtual void forEach(std::function<bool(const T &)> func) const = 0;

    [[nodiscard]] std::size_t size() const override = 0;

private:
    [[nodiscard]] const void *get0(std::string_view id) const noexcept override { return get(Identifier<T>(id)); }

    void forEach0(std::function<bool(const void *)> func) const override
    {
        forEach([&func](const T &elem) { return func(&elem); });
    }

    [[nodiscard]] const std::type_info &getTypeId() const noexcept override { return typeid(T); }
};
}  // namespace endstone

template <typename T>
    requires requires(const T &t) {
        { t.getId() } -> std::convertible_to<endstone::Identifier<T>>;
    }
struct std::formatter<T> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const T &val, FormatContext &ctx) const -> format_context::iterator
    {
        return std::format_to(ctx.out(), "{}", val.getId());
    }
};
