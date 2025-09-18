"""
Classes relating to letting plugins run code at specific time intervals.
"""

from __future__ import annotations

import collections.abc

from endstone.plugin import Plugin

__all__: list[str] = ["Scheduler", "Task"]

class Scheduler:
    """
    Represents a scheduler that executes various tasks
    """

    def cancel_task(self, id: int) -> None:
        """
        Removes task from scheduler.
        """

    def cancel_tasks(self, plugin: Plugin) -> None:
        """
        Removes all tasks associated with a particular plugin from the scheduler.
        """

    def get_pending_tasks(self) -> list[Task]:
        """
        Returns a vector of all pending tasks.
        """

    def is_queued(self, id: int) -> bool:
        """
        Check if the task queued to be run later.
        """

    def is_running(self, id: int) -> bool:
        """
        Check if the task currently running.
        """

    def run_task(
        self,
        plugin: Plugin,
        task: collections.abc.Callable[[], None],
        delay: int = 0,
        period: int = 0,
    ) -> Task:
        """
        Returns a task that will be executed synchronously
        """

class Task:
    """
    Represents a task being executed by the scheduler
    """

    def cancel(self) -> None:
        """
        Attempts to cancel this task.
        """

    @property
    def is_cancelled(self) -> bool:
        """
        Returns true if the task has been cancelled.
        """

    @property
    def is_sync(self) -> bool:
        """
        Returns true if the task is run by server thread.
        """

    @property
    def owner(self) -> Plugin:
        """
        Returns the Plugin that owns the task.
        """

    @property
    def task_id(self) -> int:
        """
        Returns the task id.
        """
