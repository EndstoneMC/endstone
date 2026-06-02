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

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {

void init_scheduler(py::module &m)
{
    py::class_<Task, std::shared_ptr<Task>>(m, "Task", "Represents a task being executed by the scheduler.")
        .def_property_readonly("task_id", &Task::getTaskId, "The task id number.")
        .def_property_readonly("owner", &Task::getOwner, py::return_value_policy::reference,
                               "The `Plugin` that owns this task.")
        .def_property_readonly("is_sync", &Task::isSync, "`True` if the task is run by the server thread.")
        .def_property_readonly("is_cancelled", &Task::isCancelled, "`True` if this task has been cancelled.")
        .def("cancel", &Task::cancel, "Attempts to cancel this task.");

    py::classh<Scheduler>(m, "Scheduler", "Represents a scheduler that executes various tasks.")
        .def("run_task", &Scheduler::runTaskTimer, py::arg("plugin"), py::arg("task"), py::arg("delay") = 0,
             py::arg("period") = 0, py::return_value_policy::reference, R"doc(
    Returns a task that will be executed repeatedly (and synchronously) until cancelled, starting after the
    specified number of server ticks.

    Args:
        plugin: The reference to the plugin scheduling the task.
        task: The task to be run.
        delay: The ticks to wait before running the task.
        period: The ticks to wait between runs.

    Returns:
        A `Task` that contains the id number (`None` if task is empty).
)doc")
        .def("cancel_task", &Scheduler::cancelTask, py::arg("id"), R"doc(
    Removes task from scheduler.

    Args:
        id: Id number of task to be removed.
)doc")
        .def("cancel_tasks", &Scheduler::cancelTasks, py::arg("plugin"), R"doc(
    Removes all tasks associated with a particular plugin from the scheduler.

    Args:
        plugin: Owner of tasks to be removed.
)doc")
        .def("is_running", &Scheduler::isRunning, py::arg("id"), R"doc(
    Check if the task is currently running.

    Args:
        id: The id of the task to check.

    Returns:
        `True` if the task is currently running.
)doc")
        .def("is_queued", &Scheduler::isQueued, py::arg("id"), R"doc(
    Check if the task is queued to be run later.

    Args:
        id: The id of the task to check.

    Returns:
        `True` if the task is queued to be run.
)doc")
        .def("get_pending_tasks", &Scheduler::getPendingTasks, py::return_value_policy::reference_internal, R"doc(
    Returns a list of all pending tasks.

    The ordering of the tasks is NOT related to their order of execution.
)doc");
}

}  // namespace endstone::python
