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

#include <pybind11/detail/internals.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "endstone/registry.h"

namespace py = pybind11;

namespace endstone::python {

struct PyIdentifier {
    std::string namespace_;
    std::string key_;

    PyIdentifier() = default;

    PyIdentifier(std::string ns, std::string key) : namespace_(std::move(ns)), key_(std::move(key))
    {
        if (namespace_.empty() || key_.empty()) {
            throw std::invalid_argument("Identifier namespace and key must not be empty.");
        }
    }

    explicit PyIdentifier(const std::string &full)
    {
        if (full.empty()) {
            throw std::invalid_argument("Identifier string must not be empty.");
        }
        const auto pos = full.rfind(':');
        if (pos == std::string::npos) {
            namespace_ = "minecraft";
            key_ = full;
        }
        else {
            namespace_ = full.substr(0, pos);
            key_ = full.substr(pos + 1);
        }
        if (namespace_.empty() || key_.empty()) {
            throw std::invalid_argument("Identifier namespace and key must not be empty.");
        }
    }

    [[nodiscard]] std::string str() const { return namespace_ + ":" + key_; }

    bool operator==(const PyIdentifier &other) const
    {
        return namespace_ == other.namespace_ && key_ == other.key_;
    }
};

class PyRegistry {
public:
    explicit PyRegistry(const IRegistry &registry) : registry_(registry) {}

    [[nodiscard]] py::object get(const std::string &id) const
    {
        if (const auto *p = registry_.get0(id)) {
            return cast(p);
        }
        return py::none();
    }

    [[nodiscard]] py::object getOrThrow(const std::string &id) const
    {
        if (const auto *p = registry_.get0(id)) {
            return cast(p);
        }
        throw py::key_error(fmt::format("No registry entry found for identifier: {}", id));
    }

    [[nodiscard]] py::iterator iter() const
    {
        py::list items;
        registry_.forEach0([this, &items](const void *p) {
            items.append(cast(p));
            return true;
        });
        return py::iter(items);
    }

    [[nodiscard]] bool contains(const std::string &id) const { return registry_.get0(id) != nullptr; }

    [[nodiscard]] std::size_t size() const { return registry_.size(); }

private:
    [[nodiscard]] py::object cast(const void *p) const
    {
        auto *tinfo = py::detail::get_type_info(registry_.getTypeId());
        return py::reinterpret_steal<py::object>(
            py::detail::type_caster_generic::cast(p, py::return_value_policy::reference, {}, tinfo, nullptr, nullptr));
    }

    const IRegistry &registry_;
};
};  // namespace endstone::python
