"""
Classes relating to permissions of players.
"""

import collections
import enum
import typing

from endstone.plugin import Plugin

__all__ = [
    "Permissible",
    "Permission",
    "PermissionAttachment",
    "PermissionAttachmentInfo",
    "PermissionDefault",
    "PermissionLevel",
]

class PermissionDefault(enum.Enum):
    """
    Represents the possible default values for permissions
    """

    TRUE = 0
    FALSE = 1
    OP = 2
    NOT_OP = 3
    CONSOLE = 4

class PermissionLevel(enum.IntEnum):
    DEFAULT = 0
    OP = 1
    CONSOLE = 2

class Permissible:
    """
    Represents an object that may become a server operator and can be assigned permissions.
    """
    @property
    def permission_level(self) -> PermissionLevel:
        """
        The permission level of this object.
        """
        ...
    @typing.overload
    def is_permission_set(self, name: str) -> bool:
        """
        Checks if this object contains an override for the specified permission, by fully qualified name.

        Args:
            name: Name of the permission.

        Returns:
            `True` if the permission is set, `False` otherwise.
        """
        ...
    @typing.overload
    def is_permission_set(self, perm: Permission) -> bool:
        """
        Checks if this object contains an override for the specified `Permission`.

        Args:
            perm: Permission to check.

        Returns:
            `True` if the permission is set, `False` otherwise.
        """
        ...
    @typing.overload
    def has_permission(self, name: str) -> bool:
        """
        Gets the value of the specified permission, if set.

        If a permission override is not set on this object, the default value of the permission will be returned.

        Args:
            name: Name of the permission.

        Returns:
            Value of the permission.
        """
        ...
    @typing.overload
    def has_permission(self, perm: Permission) -> bool:
        """
        Gets the value of the specified permission, if set.

        If a permission override is not set on this object, the default value of the permission will be returned.

        Args:
            perm: Permission to get.

        Returns:
            Value of the permission.
        """
        ...
    @typing.overload
    def add_attachment(self, plugin: Plugin, name: str, value: bool) -> PermissionAttachment:
        """
        Adds a new `PermissionAttachment` with a single permission by name and value.

        Args:
            plugin: Plugin responsible for this attachment; must not be disabled.
            name: Name of the permission to attach.
            value: Value of the permission.

        Returns:
            The `PermissionAttachment` that was just created.
        """
        ...
    @typing.overload
    def add_attachment(self, plugin: Plugin) -> PermissionAttachment:
        """
        Adds a new empty `PermissionAttachment` to this object.

        Args:
            plugin: Plugin responsible for this attachment; must not be disabled.

        Returns:
            The `PermissionAttachment` that was just created.
        """
        ...
    def remove_attachment(self, attachment: PermissionAttachment) -> bool:
        """
        Removes the given `PermissionAttachment` from this object.

        Args:
            attachment: Attachment to remove.

        Returns:
            `True` if the specified attachment was removed successfully, `False` when it isn't part of this object.
        """
        ...
    def recalculate_permissions(self) -> None:
        """
        Recalculates the permissions for this object, if the attachments have changed values.

        This should very rarely need to be called from a plugin.
        """
        ...
    @property
    def effective_permissions(self) -> set[PermissionAttachmentInfo]:
        """
        A set containing all the permissions currently in effect by this object.
        """
        ...

class Permission:
    """
    Represents a unique permission that may be attached to a `Permissible`
    """
    def __init__(
        self,
        name: str,
        description: str | None = None,
        default: PermissionDefault | None = None,
        children: collections.abc.Mapping[str, bool] | None = None,
        *args,
        **kwargs,
    ) -> None: ...
    @property
    def name(self) -> str:
        """
        The unique fully qualified name of this `Permission`.
        """
        ...
    @property
    def children(self) -> dict[str, bool]:
        """
        The children of this permission.

        If you change this map in any form, you must call `recalculate_permissibles()` to recalculate all `Permissibles`.
        """
        ...
    @property
    def default(self) -> PermissionDefault:
        """
        The default value of this permission.
        """
        ...
    @default.setter
    def default(self, arg1: PermissionDefault) -> None: ...
    @property
    def description(self) -> str:
        """
        A brief description of this permission.
        """
        ...
    @description.setter
    def description(self, arg1: str) -> None: ...
    @property
    def permissibles(self) -> set[Permissible]:
        """
        A set containing every `Permissible` that has this permission.
        """
        ...
    def recalculate_permissibles(self) -> None:
        """
        Recalculates all `Permissibles` that contain this permission.

        This should be called after modifying the children, and is automatically called after modifying the default value.
        """
        ...
    @typing.overload
    def add_parent(self, name: str, value: bool) -> Permission:
        """
        Adds this permission to the specified parent permission.

        If the parent permission does not exist, it will be created and registered.

        Args:
            name: Name of the parent permission.
            value: The value to set this permission to.

        Returns:
            The parent permission that was created or loaded.
        """
        ...
    @typing.overload
    def add_parent(self, perm: Permission, value: bool) -> None:
        """
        Adds this permission to the specified parent permission.

        Args:
            perm: Parent permission to register with.
            value: The value to set this permission to.
        """
        ...

class PermissionAttachment:
    """
    Holds information about a permission attachment on a `Permissible` object.
    """
    def __init__(self, plugin: Plugin, permissible: Permissible) -> None: ...
    @property
    def plugin(self) -> Plugin:
        """
        The plugin responsible for this permission attachment.
        """
        ...
    def remove(self) -> bool:
        """
        Removes this attachment from its registered `Permissible`.

        Returns:
            `True` if the attachment was removed successfully, `False` if it did not exist.
        """
        ...
    @property
    def permissible(self) -> Permissible:
        """
        The `Permissible` that this is attached to.
        """
        ...
    @property
    def permissions(self) -> dict[str, bool]:
        """
        A copy of all set permissions and values contained within this attachment.

        This map may be modified but will not affect the attachment, as it is a copy.
        """
        ...
    @typing.overload
    def set_permission(self, name: str, value: bool) -> None:
        """
        Sets a permission to the given value, by its fully qualified name.

        Args:
            name: Name of the permission.
            value: New value of the permission.
        """
        ...
    @typing.overload
    def set_permission(self, perm: Permission, value: bool) -> None:
        """
        Sets a permission to the given value.

        Args:
            perm: Permission to set.
            value: New value of the permission.
        """
        ...
    @typing.overload
    def unset_permission(self, name: str) -> None:
        """
        Removes the specified permission from this attachment.

        If the permission does not exist in this attachment, nothing will happen.

        Args:
            name: Name of the permission to remove.
        """
        ...
    @typing.overload
    def unset_permission(self, perm: Permission) -> None:
        """
        Removes the specified permission from this attachment.

        If the permission does not exist in this attachment, nothing will happen.

        Args:
            perm: Permission to remove.
        """
        ...
    @property
    def removal_callback(self) -> collections.abc.Callable[[PermissionAttachment], None]:
        """
        The callback to be called when this attachment is removed. May be empty.
        """
        ...
    @removal_callback.setter
    def removal_callback(self, arg1: collections.abc.Callable[[PermissionAttachment], None]) -> None: ...

class PermissionAttachmentInfo:
    """
    Holds information on a permission and which `PermissionAttachment` provides it.
    """
    def __init__(
        self, permissible: Permissible, permission: str, attachment: PermissionAttachment, value: bool
    ) -> None: ...
    @property
    def permissible(self) -> Permissible:
        """
        The `Permissible` this permission is for.
        """
        ...
    @property
    def permission(self) -> str:
        """
        The name of the permission being set.
        """
        ...
    @property
    def attachment(self) -> PermissionAttachment:
        """
        The attachment providing this permission.

        This may be `None` for default permissions (usually parent permissions).
        """
        ...
    @property
    def value(self) -> bool:
        """
        The value of this permission.
        """
        ...
