"""
Classes relating to permissions of players.
"""

from __future__ import annotations

import collections.abc
import enum
import typing

from endstone.plugin import Plugin

__all__: list[str] = [
    "Permissible",
    "Permission",
    "PermissionAttachment",
    "PermissionAttachmentInfo",
    "PermissionDefault",
    "PermissionLevel",
]

class Permissible:
    """
    Represents an object that may become a server operator and can be assigned permissions.
    """

    @typing.overload
    def add_attachment(self, plugin: Plugin, name: str, value: bool) -> PermissionAttachment:
        """
        Adds a new PermissionAttachment.
        """

    @typing.overload
    def add_attachment(self, plugin: Plugin) -> PermissionAttachment:
        """
        Adds a new PermissionAttachment.
        """

    @typing.overload
    def has_permission(self, name: str) -> bool:
        """
        Checks if a permissions is available by name.
        """

    @typing.overload
    def has_permission(self, perm: Permission) -> bool:
        """
        Checks if a permissions is available by permission.
        """

    @typing.overload
    def is_permission_set(self, name: str) -> bool:
        """
        Checks if a permissions is set by name.
        """

    @typing.overload
    def is_permission_set(self, perm: Permission) -> bool:
        """
        Checks if a permissions is set by permission.
        """

    def recalculate_permissions(self) -> None:
        """
        Recalculates the permissions.
        """

    def remove_attachment(self, attachment: PermissionAttachment) -> None:
        """
        Removes a given PermissionAttachment.
        """

    @property
    def effective_permissions(self) -> set[PermissionAttachmentInfo]:
        """
        Gets effective permissions.
        """

    @property
    def permission_level(self) -> PermissionLevel:
        """
        Gets the permission level of this object
        """

class Permission:
    """
    Represents a unique permission that may be attached to a Permissible
    """

    def __init__(
        self,
        name: str,
        description: str | None = None,
        default: PermissionDefault | None = None,
        children: dict[str, bool] | None = None,
        *args,
        **kwargs,
    ) -> None: ...
    @typing.overload
    def add_parent(self, name: str, value: bool) -> Permission:
        """
        Adds this permission to the specified parent permission.
        """

    @typing.overload
    def add_parent(self, perm: Permission, value: bool) -> None:
        """
        Adds this permission to the specified parent permission.
        """

    def recalculate_permissibles(self) -> None:
        """
        Recalculates all Permissibles that contain this permission.
        """

    @property
    def children(self) -> dict[str, bool]:
        """
        Gets the children of this permission.
        """

    @property
    def default(self) -> PermissionDefault:
        """
        The default value of this permission.
        """

    @default.setter
    def default(self, arg1: PermissionDefault) -> None: ...
    @property
    def description(self) -> str:
        """
        The brief description of this permission
        """

    @description.setter
    def description(self, arg1: str) -> None: ...
    @property
    def name(self) -> str:
        """
        Gets the unique fully qualified name of this Permission.
        """

    @property
    def permissibles(self) -> set[Permissible]:
        """
        Gets a set containing every Permissible that has this permission.
        """

class PermissionAttachment:
    """
    Holds information about a permission attachment on a Permissible object
    """

    def __init__(self, plugin: Plugin, permissible: Permissible) -> None: ...
    def remove(self) -> bool:
        """
        Removes this attachment from its registered Permissible.
        """

    @typing.overload
    def set_permission(self, name: str, value: bool) -> None:
        """
        Sets a permission to the given value, by its fully qualified name.
        """

    @typing.overload
    def set_permission(self, perm: Permission, value: bool) -> None:
        """
        Sets a permission to the given value.
        """

    @typing.overload
    def unset_permission(self, name: str) -> None:
        """
        Removes the specified permission from this attachment by name.
        """

    @typing.overload
    def unset_permission(self, perm: Permission) -> None:
        """
        Removes the specified permission from this attachment.
        """

    @property
    def permissible(self) -> Permissible:
        """
        Gets the Permissible that this is attached to.
        """

    @property
    def permissions(self) -> dict[str, bool]:
        """
        Gets a copy of all set permissions and values contained within this attachment.
        """

    @property
    def plugin(self) -> Plugin:
        """
        Gets the plugin responsible for this attachment.
        """

    @property
    def removal_callback(self) -> collections.abc.Callable[[PermissionAttachment], None]:
        """
        The callback to be called when this attachment is removed.
        """

    @removal_callback.setter
    def removal_callback(self, arg1: collections.abc.Callable[[PermissionAttachment], None]) -> None: ...

class PermissionAttachmentInfo:
    """
    Holds information on a permission and which PermissionAttachment provides it
    """

    def __init__(
        self, permissible: Permissible, permission: str, attachment: PermissionAttachment, value: bool
    ) -> None: ...
    @property
    def attachment(self) -> PermissionAttachment:
        """
        Gets the attachment providing this permission.
        """

    @property
    def permissible(self) -> Permissible:
        """
        Get the permissible this is attached to
        """

    @property
    def permission(self) -> str:
        """
        Gets the permission being set
        """

    @property
    def value(self) -> bool:
        """
        Gets the value of this permission
        """

class PermissionDefault(enum.Enum):
    """
    Represents the possible default values for permissions
    """

    CONSOLE: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.CONSOLE: 4>
    FALSE: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.FALSE: 1>
    NOT_OP: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.NOT_OP: 3>
    OP: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.OP: 2>
    TRUE: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.TRUE: 0>

class PermissionLevel(enum.IntEnum):
    CONSOLE: typing.ClassVar[PermissionLevel]  # value = <PermissionLevel.CONSOLE: 2>
    DEFAULT: typing.ClassVar[PermissionLevel]  # value = <PermissionLevel.DEFAULT: 0>
    OP: typing.ClassVar[PermissionLevel]  # value = <PermissionLevel.OP: 1>

    @classmethod
    def __new__(cls, value): ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
