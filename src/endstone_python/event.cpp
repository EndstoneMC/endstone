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

#include "endstone/event/event.h"

#include <utility>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "endstone/event/event_priority.h"
#include "endstone/event/server/server_load_event.h"

namespace py = pybind11;

namespace endstone::detail {

void init_event(py::module &m)
{
    py::enum_<EventPriority>(
        m, "EventPriority",
        "Listeners are called in following order: LOWEST -> LOW -> NORMAL -> HIGH -> HIGHEST -> MONITOR")
        .value("LOWEST", EventPriority::Lowest,
               "Event call is of very low importance and should be run first, to allow other plugins to further "
               "customise the outcome")
        .value("LOW", EventPriority::Low, "Event call is of low importance")
        .value("NORMAL", EventPriority::Normal,
               " Event call is neither important nor unimportant, and may be run normally")
        .value("HIGH", EventPriority::High, "Event call is of high importance")
        .value("HIGHEST", EventPriority::Highest,
               "Event call is critical and must have the final say in what happens to the event")
        .value("MONITOR", EventPriority::Monitor,
               "Event is listened to purely for monitoring the outcome of an event. No modifications to the event "
               "should be made under this priority.");

    auto event = py::class_<Event>(m, "Event")
                     .def_property_readonly("event_name", &Event::getEventName,
                                            "Gets a user-friendly identifier for this event.")
                     .def_property_readonly("cancellable", &Event::isCancellable,
                                            "Whether the event can be cancelled by a plugin or the server.")
                     .def_property("cancelled", &Event::isCancelled, &Event::setCancelled,
                                   "The cancellation state of this event. A cancelled event will not be executed in "
                                   "the server, but will still pass to other plugins")
                     .def("asynchronous", &Event::isAsynchronous);

    py::enum_<Event::Result>(event, "Result")
        .value("DENY", Event::Result::DENY)
        .value("DEFAULT", Event::Result::DEFAULT)
        .value("ALLOW", Event::Result::ALLOW)
        .export_values();

    auto server_load_event = py::class_<ServerLoadEvent, Event>(m, "ServerLoadEvent");
    py::enum_<ServerLoadEvent::LoadType>(server_load_event, "LoadType")
        .value("STARTUP", ServerLoadEvent::LoadType::Startup)
        .export_values();
    server_load_event.def(py::init<ServerLoadEvent::LoadType>())
        .def_property_readonly("type", &ServerLoadEvent::getType)
        .def_property_readonly("event_name", &ServerLoadEvent::getEventName);
}

}  // namespace endstone::detail
