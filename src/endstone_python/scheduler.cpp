// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include "endstone/scheduler/scheduler.h"

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "endstone/scheduler/task.h"

namespace py = pybind11;

namespace endstone::detail {

void init_scheduler(py::module &m)
{
    py::class_<Task, std::shared_ptr<Task>>(m, "Task", "Represents a task being executed by the scheduler")
        .def_property_readonly("task_id", &Task::getTaskId, "Returns the task id.")
        .def_property_readonly("owner", &Task::getOwner, py::return_value_policy::reference,
                               "Returns the Plugin that owns the task.")
        .def_property_readonly("is_sync", &Task::isSync, "Returns true if the task is run by server thread.")
        .def_property_readonly("is_cancelled", &Task::isCancelled, "Returns true if the task has been cancelled.")
        .def("cancel", &Task::cancel, "Attempts to cancel this task.");

    py::class_<Scheduler>(m, "Scheduler", "Represents a scheduler that executes various tasks")
        .def("run_task", &Scheduler::runTask, py::arg("plugin"), py::arg("task"),
             "Returns a task that will be executed synchronously on the next server tick.",
             py::return_value_policy::reference)
        .def("run_task_later", &Scheduler::runTaskLater, py::arg("plugin"), py::arg("task"), py::arg("delay"),
             "Returns a task that will be executed synchronously after the specified number of server ticks.",
             py::return_value_policy::reference)
        .def("run_task_timer", &Scheduler::runTaskTimer, py::arg("plugin"), py::arg("task"), py::arg("delay"),
             py::arg("period"),
             "Returns a task that will be executed repeatedly until cancelled, starting after the specified number of "
             "server ticks.",
             py::return_value_policy::reference)
        .def("cancel_task", &Scheduler::cancelTask, py::arg("id"), "Removes task from scheduler.")
        .def("cancel_tasks", &Scheduler::cancelTasks, py::arg("plugin"),
             "Removes all tasks associated with a particular plugin from the scheduler.")
        .def("is_running", &Scheduler::isRunning, py::arg("id"), "Check if the task currently running.")
        .def("is_queued", &Scheduler::isQueued, py::arg("id"), "Check if the task queued to be run later.")
        .def("get_pending_tasks", &Scheduler::getPendingTasks, "Returns a vector of all pending tasks.",
             py::return_value_policy::reference_internal);
}

}  // namespace endstone::detail
