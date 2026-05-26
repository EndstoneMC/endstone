

# File registry.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**registry.h**](registry_8h.md)

[Go to the documentation of this file](registry_8h.md)


```C++
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
#include <functional>
#include <stdexcept>
#include <string>

#include <fmt/format.h>

#include "detail.h"
#include "identifier.h"
#include "server.h"

namespace endstone {
namespace python {
class PyRegistry;
}

class IRegistry {
public:
    virtual ~IRegistry() = default;
    [[nodiscard]] virtual std::size_t size() const = 0;

protected:
    friend class python::PyRegistry;
    [[nodiscard]] virtual const void *get0(std::string_view id) const noexcept = 0;
    virtual void forEach0(std::function<bool(const void *)> func) const = 0;
    [[nodiscard]] virtual const std::type_info &getTypeId() const noexcept = 0;
};

template <typename T>
class Registry : public IRegistry {
public:
    class Type {
    public:
        using Id = Identifier<T>;

        virtual ~Type() = default;

        [[nodiscard]] virtual Id getId() const = 0;

        [[nodiscard]] virtual std::string getTranslationKey() const = 0;

        static const T *get(Id id)
        {
            return detail::getServer().getRegistry<T>().get(id);
        }

        bool operator==(const Id &other) const { return getId() == other; }
        bool operator!=(const Id &other) const { return !(*this == other); }
        bool operator==(const T &other) const { return getId() == other.getId(); }
        bool operator!=(const T &other) const { return !(*this == other); }
        operator Id() const { return getId(); }
    };

    virtual T *get(Identifier<T> id) noexcept = 0;

    virtual const T *get(Identifier<T> id) const noexcept = 0;

    virtual T &getOrThrow(Identifier<T> id)
    {
        if (auto *p = get(id)) {
            return *p;
        }
        throw std::invalid_argument(fmt::format("No registry entry found for identifier: {}", id));
    }

    virtual const T &getOrThrow(Identifier<T> id) const
    {
        if (auto *p = get(id)) {
            return *p;
        }
        throw std::invalid_argument(fmt::format("No registry entry found for identifier: {}", id));
    }

    virtual void forEach(std::function<bool(const T &)> func) const = 0;

    [[nodiscard]] std::size_t size() const override = 0;

private:
    [[nodiscard]] const void *get0(std::string_view id) const noexcept override
    {
        return get(Identifier<T>(id));
    }

    void forEach0(std::function<bool(const void *)> func) const override
    {
        forEach([&func](const T &elem) { return func(&elem); });
    }

    [[nodiscard]] const std::type_info &getTypeId() const noexcept override { return typeid(T); }
};
}  // namespace endstone

template <typename T>
    requires requires(const T &t) { { t.getId() } -> std::convertible_to<endstone::Identifier<T>>; }
struct fmt::formatter<T> : formatter<string_view> {
    template <typename FormatContext>
    auto format(const T &val, FormatContext &ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "{}", val.getId());
    }
};
```


