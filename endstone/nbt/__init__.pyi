"""
Classes relating to the NBT data format.
"""

import collections
import typing

__all__ = [
    "ByteArrayTag",
    "ByteTag",
    "CompoundTag",
    "DoubleTag",
    "FloatTag",
    "IntArrayTag",
    "IntTag",
    "ListTag",
    "LongTag",
    "ShortTag",
    "StringTag",
    "Tag",
    "load",
]

class Tag:
    """
    Base class for all NBT tag variants.
    """

class ByteTag(Tag):
    """
    An NBT tag holding an unsigned 8-bit integer value.
    """
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, value: int) -> None: ...
    @property
    def value(self) -> int:
        """
        The underlying value of this tag.
        """
        ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __str__(self) -> str: ...
    def __repr__(self) -> str: ...
    def __int__(self) -> int: ...
    def dump(
        self, name: str | None = None, byte_order: typing.Literal["little", "big"] = "little", network: bool = False
    ) -> bytes:
        """
        Serialize this tag to binary NBT format.

        Args:
            name: Optional root tag name. If `None`, an empty name is used.
            byte_order: Byte order for encoding, either `'little'` (Bedrock) or `'big'` (Java).
            network: If `True`, use Bedrock network varint encoding for lengths and Int/Long values.

        Returns:
            bytes: The binary NBT data.
        """
        ...

class ShortTag(Tag):
    """
    An NBT tag holding a signed 16-bit integer value.
    """
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, value: int) -> None: ...
    @property
    def value(self) -> int:
        """
        The underlying value of this tag.
        """
        ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __str__(self) -> str: ...
    def __repr__(self) -> str: ...
    def __int__(self) -> int: ...
    def dump(
        self, name: str | None = None, byte_order: typing.Literal["little", "big"] = "little", network: bool = False
    ) -> bytes:
        """
        Serialize this tag to binary NBT format.

        Args:
            name: Optional root tag name. If `None`, an empty name is used.
            byte_order: Byte order for encoding, either `'little'` (Bedrock) or `'big'` (Java).
            network: If `True`, use Bedrock network varint encoding for lengths and Int/Long values.

        Returns:
            bytes: The binary NBT data.
        """
        ...

class IntTag(Tag):
    """
    An NBT tag holding a signed 32-bit integer value.
    """
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, value: int) -> None: ...
    @property
    def value(self) -> int:
        """
        The underlying value of this tag.
        """
        ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __str__(self) -> str: ...
    def __repr__(self) -> str: ...
    def __int__(self) -> int: ...
    def dump(
        self, name: str | None = None, byte_order: typing.Literal["little", "big"] = "little", network: bool = False
    ) -> bytes:
        """
        Serialize this tag to binary NBT format.

        Args:
            name: Optional root tag name. If `None`, an empty name is used.
            byte_order: Byte order for encoding, either `'little'` (Bedrock) or `'big'` (Java).
            network: If `True`, use Bedrock network varint encoding for lengths and Int/Long values.

        Returns:
            bytes: The binary NBT data.
        """
        ...

class LongTag(Tag):
    """
    An NBT tag holding a signed 64-bit integer value.
    """
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, value: int) -> None: ...
    @property
    def value(self) -> int:
        """
        The underlying value of this tag.
        """
        ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __str__(self) -> str: ...
    def __repr__(self) -> str: ...
    def __int__(self) -> int: ...
    def dump(
        self, name: str | None = None, byte_order: typing.Literal["little", "big"] = "little", network: bool = False
    ) -> bytes:
        """
        Serialize this tag to binary NBT format.

        Args:
            name: Optional root tag name. If `None`, an empty name is used.
            byte_order: Byte order for encoding, either `'little'` (Bedrock) or `'big'` (Java).
            network: If `True`, use Bedrock network varint encoding for lengths and Int/Long values.

        Returns:
            bytes: The binary NBT data.
        """
        ...

class FloatTag(Tag):
    """
    An NBT tag holding a 32-bit floating-point value.
    """
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, value: float) -> None: ...
    @property
    def value(self) -> float:
        """
        The underlying value of this tag.
        """
        ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __str__(self) -> str: ...
    def __repr__(self) -> str: ...
    def __float__(self) -> float: ...
    def dump(
        self, name: str | None = None, byte_order: typing.Literal["little", "big"] = "little", network: bool = False
    ) -> bytes:
        """
        Serialize this tag to binary NBT format.

        Args:
            name: Optional root tag name. If `None`, an empty name is used.
            byte_order: Byte order for encoding, either `'little'` (Bedrock) or `'big'` (Java).
            network: If `True`, use Bedrock network varint encoding for lengths and Int/Long values.

        Returns:
            bytes: The binary NBT data.
        """
        ...

class DoubleTag(Tag):
    """
    An NBT tag holding a 64-bit floating-point value.
    """
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, value: float) -> None: ...
    @property
    def value(self) -> float:
        """
        The underlying value of this tag.
        """
        ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __str__(self) -> str: ...
    def __repr__(self) -> str: ...
    def __float__(self) -> float: ...
    def dump(
        self, name: str | None = None, byte_order: typing.Literal["little", "big"] = "little", network: bool = False
    ) -> bytes:
        """
        Serialize this tag to binary NBT format.

        Args:
            name: Optional root tag name. If `None`, an empty name is used.
            byte_order: Byte order for encoding, either `'little'` (Bedrock) or `'big'` (Java).
            network: If `True`, use Bedrock network varint encoding for lengths and Int/Long values.

        Returns:
            bytes: The binary NBT data.
        """
        ...

class ByteArrayTag(Tag):
    """
    An NBT tag holding an array of unsigned 8-bit integers.
    """
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, iterable: collections.abc.Iterable) -> None: ...
    @typing.overload
    def __init__(self, buffer: collections.abc.Buffer) -> None: ...
    def __buffer__(self, flags) -> None:
        """
        Return a buffer object that exposes the underlying memory of the object.
        """
        ...
    def __release_buffer__(self, buffer) -> None:
        """
        Release the buffer object that exposes the underlying memory of the object.
        """
        ...
    def clear(self) -> None:
        """
        Removes all values from this tag.
        """
        ...
    def append(self, value: int) -> None:
        """
        Appends a value to the end of this tag.

        Args:
            value: The value to append.
        """
        ...
    def extend(self, iterable: collections.abc.Iterable) -> None:
        """
        Appends all values from an iterable to the end of this tag.

        Args:
            iterable: An iterable of values to append.
        """
        ...
    def __len__(self) -> int: ...
    def __bool__(self) -> bool: ...
    def __getitem__(self, arg0: int) -> int: ...
    def __setitem__(self, arg0: int, arg1: int) -> None: ...
    def __iter__(self) -> collections.abc.Iterator[int]: ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __str__(self) -> str: ...
    def __repr__(self) -> str: ...
    def __bytes__(self) -> bytes: ...
    def dump(
        self, name: str | None = None, byte_order: typing.Literal["little", "big"] = "little", network: bool = False
    ) -> bytes:
        """
        Serialize this tag to binary NBT format.

        Args:
            name: Optional root tag name. If `None`, an empty name is used.
            byte_order: Byte order for encoding, either `'little'` (Bedrock) or `'big'` (Java).
            network: If `True`, use Bedrock network varint encoding for lengths and Int/Long values.

        Returns:
            bytes: The binary NBT data.
        """
        ...

class StringTag(Tag):
    """
    An NBT tag holding a string value.
    """
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, value: str) -> None: ...
    @property
    def value(self) -> str:
        """
        The underlying value of this tag.
        """
        ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __str__(self) -> str: ...
    def __repr__(self) -> str: ...
    def dump(
        self, name: str | None = None, byte_order: typing.Literal["little", "big"] = "little", network: bool = False
    ) -> bytes:
        """
        Serialize this tag to binary NBT format.

        Args:
            name: Optional root tag name. If `None`, an empty name is used.
            byte_order: Byte order for encoding, either `'little'` (Bedrock) or `'big'` (Java).
            network: If `True`, use Bedrock network varint encoding for lengths and Int/Long values.

        Returns:
            bytes: The binary NBT data.
        """
        ...

class IntArrayTag(Tag):
    """
    An NBT tag holding an array of signed 32-bit integers.
    """
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, iterable: collections.abc.Iterable) -> None: ...
    def clear(self) -> None:
        """
        Removes all values from this tag.
        """
        ...
    def append(self, value: int) -> None:
        """
        Appends a value to the end of this tag.

        Args:
            value: The value to append.
        """
        ...
    def extend(self, iterable: collections.abc.Iterable) -> None:
        """
        Appends all values from an iterable to the end of this tag.

        Args:
            iterable: An iterable of values to append.
        """
        ...
    def __len__(self) -> int: ...
    def __bool__(self) -> bool: ...
    def __getitem__(self, arg0: int) -> int: ...
    def __setitem__(self, arg0: int, arg1: int) -> None: ...
    def __iter__(self) -> collections.abc.Iterator[int]: ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __str__(self) -> str: ...
    def __repr__(self) -> str: ...
    def dump(
        self, name: str | None = None, byte_order: typing.Literal["little", "big"] = "little", network: bool = False
    ) -> bytes:
        """
        Serialize this tag to binary NBT format.

        Args:
            name: Optional root tag name. If `None`, an empty name is used.
            byte_order: Byte order for encoding, either `'little'` (Bedrock) or `'big'` (Java).
            network: If `True`, use Bedrock network varint encoding for lengths and Int/Long values.

        Returns:
            bytes: The binary NBT data.
        """
        ...

class ListTag(Tag):
    """
    An NBT list tag of homogeneously-typed tags.
    """
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, iterable: collections.abc.Iterable) -> None: ...
    def __len__(self) -> int: ...
    def __bool__(self) -> bool: ...
    def __str__(self) -> str: ...
    def __repr__(self) -> str: ...
    def __getitem__(self, arg0: int) -> Tag: ...
    def __setitem__(self, arg0: int, arg1: Tag) -> None: ...
    def __delitem__(self, arg0: int) -> None: ...
    def __iter__(self) -> collections.abc.Iterator[Tag]: ...
    def clear(self) -> None:
        """
        Removes all tags from this list.
        """
        ...
    def append(self, tag: Tag) -> None:
        """
        Appends a tag to the end of this list.

        Args:
            tag: The tag to append. It must have the same type as the existing elements.
        """
        ...
    def extend(self, iterable: collections.abc.Iterable) -> None:
        """
        Appends all tags from an iterable to the end of this list.

        Args:
            iterable: An iterable of tags to append. Each tag must have the same type as the existing elements.
        """
        ...
    def pop(self, index: int = -1) -> Tag:
        """
        Removes and returns the tag at the given index.

        Args:
            index: The index of the tag to remove. Defaults to the last tag.

        Returns:
            The removed tag.
        """
        ...
    def to_list(self) -> list:
        """
        Converts this list to a plain Python list of native values.
        """
        ...
    def size(self) -> int:
        """
        The number of tags in this list.
        """
        ...
    def empty(self) -> bool:
        """
        Returns `True` if this list contains no tags.
        """
        ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def dump(
        self, name: str | None = None, byte_order: typing.Literal["little", "big"] = "little", network: bool = False
    ) -> bytes:
        """
        Serialize this tag to binary NBT format.

        Args:
            name: Optional root tag name. If `None`, an empty name is used.
            byte_order: Byte order for encoding, either `'little'` (Bedrock) or `'big'` (Java).
            network: If `True`, use Bedrock network varint encoding for lengths and Int/Long values.

        Returns:
            bytes: The binary NBT data.
        """
        ...

class CompoundTag(Tag):
    """
    A named NBT compound tag mapping string keys to tags.
    """
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, mapping: dict) -> None: ...
    def __len__(self) -> int: ...
    def __bool__(self) -> bool: ...
    def __contains__(self, key: str) -> bool: ...
    def __getitem__(self, arg0: str) -> Tag: ...
    def __setitem__(self, arg0: str, arg1: Tag) -> None: ...
    def setdefault(self, key: str, default: Tag) -> Tag:
        """
        Returns the tag for the given key, inserting a default if the key is not present.

        Args:
            key: The key to look up.
            default: The tag to insert and return if the key is not present.

        Returns:
            The tag associated with the key, or the newly inserted default.
        """
        ...
    def clear(self) -> None:
        """
        Removes all entries from this compound tag.
        """
        ...
    @typing.overload
    def pop(self, key: str) -> Tag:
        """
        Removes the entry with the given key and returns its tag.

        Args:
            key: The key to remove.

        Returns:
            The tag that was associated with the key.

        Raises:
            KeyError: If the key is not present.
        """
        ...
    @typing.overload
    def pop(self, key: str, default: object = None) -> object:
        """
        Removes the entry with the given key and returns its tag, or a default if the key is not present.

        Args:
            key: The key to remove.
            default: The value to return if the key is not present.

        Returns:
            The tag that was associated with the key, or the default if the key is not present.
        """
        ...
    def __iter__(self) -> collections.abc.Iterator[str]: ...
    def keys(self) -> collections.abc.Iterator[str]:
        """
        Returns an iterator over the keys in this compound tag.
        """
        ...
    def values(self) -> collections.abc.Iterator[Tag]:
        """
        Returns an iterator over the tags in this compound tag.
        """
        ...
    def items(self) -> collections.abc.Iterator[tuple[str, Tag]]:
        """
        Returns an iterator over the (key, tag) pairs in this compound tag.
        """
        ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def to_dict(self) -> dict:
        """
        Converts this compound tag to a plain Python dict of native values.
        """
        ...
    def __str__(self) -> str: ...
    def __repr__(self) -> str: ...
    def dump(
        self, name: str | None = None, byte_order: typing.Literal["little", "big"] = "little", network: bool = False
    ) -> bytes:
        """
        Serialize this tag to binary NBT format.

        Args:
            name: Optional root tag name. If `None`, an empty name is used.
            byte_order: Byte order for encoding, either `'little'` (Bedrock) or `'big'` (Java).
            network: If `True`, use Bedrock network varint encoding for lengths and Int/Long values.

        Returns:
            bytes: The binary NBT data.
        """
        ...

def load(data: bytes, byte_order: typing.Literal["little", "big"] = "little", network: bool = False) -> tuple[Tag, str]:
    """
    Deserialize binary NBT data into a tag.

    Args:
        data: Binary NBT data (bytes).
        byte_order: Byte order of the binary data, either `'little'` (Bedrock) or `'big'` (Java).
        network: If `True`, expect Bedrock network varint encoding.

    Returns:
        tuple[Tag, str]: A tuple of (tag, name) where name is the root tag name.
    """
    ...
