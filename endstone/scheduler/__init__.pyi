"""
Classes relating to letting plugins run code at specific time intervals.
"""

import collections

from endstone.plugin import Plugin

__all__ = [
    "Scheduler",
    "Task",
]

class Task:
    """
    Represents a task being executed by the scheduler.
    """
    @property
    def task_id(self) -> int:
        """
        The task id number.
        """
        ...
    @property
    def owner(self) -> Plugin:
        """
        The Plugin that owns this task.
        """
        ...
    @property
    def is_sync(self) -> bool:
        """
        True if the task is run by the server thread.
        """
        ...
    @property
    def is_cancelled(self) -> bool:
        """
        True if this task has been cancelled.
        """
        ...
    def cancel(self) -> None:
        """
        Attempts to cancel this task.
        """
        ...

class Scheduler:
    """
    Represents a scheduler that executes various tasks.
    """
    def run_task(
        self, plugin: Plugin, task: collections.abc.Callable[[], None], delay: int = 0, period: int = 0
    ) -> Task:
        """
        Returns a task that will be executed repeatedly (and synchronously) until cancelled, starting after the
        specified number of server ticks.

        Args:
            plugin: The reference to the plugin scheduling the task.
            task: The task to be run.
            delay: The ticks to wait before running the task.
            period: The ticks to wait between runs.

        Returns:
            A Task that contains the id number (None if task is empty).
        """
        ...
    def cancel_task(self, id: int) -> None:
        """
        Removes task from scheduler.

        Args:
            id: Id number of task to be removed.
        """
        ...
    def cancel_tasks(self, plugin: Plugin) -> None:
        """
        Removes all tasks associated with a particular plugin from the scheduler.

        Args:
            plugin: Owner of tasks to be removed.
        """
        ...
    def is_running(self, id: int) -> bool:
        """
        Check if the task is currently running.

        Args:
            id: The id of the task to check.

        Returns:
            True if the task is currently running.
        """
        ...
    def is_queued(self, id: int) -> bool:
        """
        Check if the task is queued to be run later.

        Args:
            id: The id of the task to check.

        Returns:
            True if the task is queued to be run.
        """
        ...
    def get_pending_tasks(self) -> list[Task]:
        """
        Returns a list of all pending tasks.

        The ordering of the tasks is NOT related to their order of execution.
        """
        ...
