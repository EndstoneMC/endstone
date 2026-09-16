# Stub pieces with no pybind11 counterpart. Because they live here rather than
# in the committed .pyi files, generation never consumes its own output.
#
# Queries are re.search, so anchor every one.

^endstone\.__prefix__$:
    \import typing
    \from ._version import __version__
    \export __version__, Registry
    _T = typing.TypeVar("_T")

# The three concrete registries are presented as one generic Registry below.
^endstone\.(BlockType|Enchantment|ItemType)Registry$:

^endstone\.Server\.get_registry$:
    def get_registry(self, type: typing.Type[_T]) -> Registry[_T]:
        \doc

^endstone\.__suffix__$:
    class Registry(typing.Generic[_T]):
        """
        Presents a registry
        """
        def get(self, key: str) -> _T | None:
            """
            Get the object by its key.
            """
            ...
        def get_or_throw(self, key: str) -> _T:
            """
            Get the object by its key or throw if missing.
            """
            ...
        def __getitem__(self, key: str) -> _T: ...
        def __iter__(self) -> typing.Iterator[_T]: ...
        def __contains__(self, key: str) -> bool: ...
