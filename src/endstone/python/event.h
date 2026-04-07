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

#include <fmt/format.h>
#include <pybind11/pybind11.h>

#include "endstone/event/cancellable.h"
#include "endstone/event/event.h"

namespace py = pybind11;

namespace endstone::python {

class PyEvent : public Event, public ICancellable {
public:
    using Event::Event;
    [[nodiscard]] std::string getEventName() const override
    {
        py::gil_scoped_acquire gil;
        const auto self = py::cast(this);
        const auto type = py::type::of(self);
        auto module = type.attr("__module__").cast<std::string>();
        auto qualname = type.attr("__qualname__").cast<std::string>();
        if (module.starts_with("endstone.")) {
            return qualname;
        }
        return fmt::format("{}.{}", module, qualname);
    }

    [[nodiscard]] bool isCancelled() const override
    {
        py::gil_scoped_acquire gil;
        return py::getattr(py::cast(this), "_cancelled", py::bool_(false)).cast<bool>();
    }

    void setCancelled(bool cancel) override
    {
        py::gil_scoped_acquire gil;
        py::setattr(py::cast(this), "_cancelled", py::bool_(cancel));
    }

    void cancel() override { setCancelled(true); }

private:
    [[nodiscard]] bool isCancellable() const override
    {
        py::gil_scoped_acquire gil;
        const py::object cls = py::module_::import("endstone.event").attr("Cancellable");
        return py::isinstance(py::cast(this), cls);
    }
};

}  // namespace endstone::python
