"""
Classes relating to letting plugins run code at specific time intervals.
"""

import collections
import typing

from endstone.plugin import Plugin

__all__ = ["Scheduler", "Task"]

class Task:
    """
    Represents a task being executed by the scheduler
    """
    @property
    def task_id(self) -> int:
        """
        Returns the task id.
        """
        ...
    @property
    def owner(self) -> Plugin:
        """
        Returns the Plugin that owns the task.
        """
        ...
    @property
    def is_sync(self) -> bool:
        """
        Returns true if the task is run by server thread.
        """
        ...
    @property
    def is_cancelled(self) -> bool:
        """
        Returns true if the task has been cancelled.
        """
        ...
    def cancel(self) -> None:
        """
        Attempts to cancel this task.
        """
        ...

class Scheduler:
    """
    Represents a scheduler that executes various tasks
    """
    def run_task(
        self, plugin: Plugin, task: collections.abc.Callable[[], None], delay: int = 0, period: int = 0
    ) -> Task:
        """
        Returns a task that will be executed synchronously
        """
        ...
    def cancel_task(self, id: int) -> None:
        """
        Removes task from scheduler.
        """
        ...
    def cancel_tasks(self, plugin: Plugin) -> None:
        """
        Removes all tasks associated with a particular plugin from the scheduler.
        """
        ...
    def is_running(self, id: int) -> bool:
        """
        Check if the task currently running.
        """
        ...
    def is_queued(self, id: int) -> bool:
        """
        Check if the task queued to be run later.
        """
        ...
    def get_pending_tasks(self) -> list[Task]:
        """
        Returns a vector of all pending tasks.
        """
        ...
