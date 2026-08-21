# Stub pieces the bindings cannot express, mostly generics. Because they live
# here rather than in the committed .pyi files, generation never consumes its
# own output.
#
# Queries are re.search, so anchor every one. A body is emitted verbatim, so the
# blank line that separates a block from the next query is part of it.

^endstone\.__prefix__$:
    \import typing
    \from ._version import __version__
    \export __version__
    _T = typing.TypeVar("_T")

^endstone\.Identifier$:
    class Identifier(typing.Generic[_T]):
        """
        Represents a namespaced identifier consisting of a namespace and a key.
        """
        def __init__(self, id: str) -> None: ...
        @property
        def namespace(self) -> str:
            """
            The namespace component of this identifier.
            """
        @property
        def key(self) -> str:
            """
            The key component of this identifier.
            """
        def __hash__(self) -> int: ...
        def __eq__(self, other: object) -> bool: ...
        def __ne__(self, other: object) -> bool: ...

^endstone\.Registry$:
    class Registry(typing.Generic[_T]):
        """
        Presents a registry
        """
        def get(self, id: Identifier[_T] | str) -> _T | None:
            """
            Get the object by its key.
            """
        def get_or_throw(self, id: Identifier[_T] | str) -> _T:
            """
            Get the object by its key or throw if missing.
            """
        def __getitem__(self, id: Identifier[_T] | str) -> _T: ...
        def __iter__(self) -> typing.Iterator[_T]: ...
        def __contains__(self, id: Identifier[_T] | str) -> bool: ...
        def __len__(self) -> int: ...

^endstone\.Server\.get_registry$:
    def get_registry(self, type: type[_T]) -> Registry[_T]:
        \doc

# GameRule is generic in the value type, which nothing carries at runtime -- a
# constant is just its identifier there. Only the base is injected, so the class
# body still generates; the int-valued rules are listed because the value type
# cannot be recovered from the constant, and everything else is a flag.
^endstone\.GameRule\.__bases__$:
    typing.Generic[_T]

^endstone\.GameRule\.id$:
    @property
    def id(self) -> Identifier[GameRule[_T]]:
        \doc

^endstone\.GameRule\.get$:
    @typing.overload
    @staticmethod
    def get(name: Identifier[GameRule[_T]]) -> GameRule[_T] | None: ...
    @typing.overload
    @staticmethod
    def get(name: str) -> GameRule[typing.Any] | None:
        \doc

^endstone\.GameRule\.(?P<name>FUNCTION_COMMAND_LIMIT|MAX_COMMAND_CHAIN_LENGTH|PLAYERS_SLEEPING_PERCENTAGE|PLAYER_WAYPOINTS|RANDOM_TICK_SPEED|SPAWN_RADIUS)$:
    \name: Identifier[GameRule[int]] = \value
^endstone\.GameRule\.(?P<name>[A-Z][A-Z0-9_]*)$:
    \name: Identifier[GameRule[bool]] = \value
# Identifier class constants. The bindings expose them as plain values, so the
# generic parameter is lost; the owners are listed because a query cannot tell
# these apart from enum members, which are ALL_CAPS too. The annotation types the
# constant, the id spells out what is in it. No trailing blank line, so the
# constants stay packed together.
^endstone\.\w+\.(?P<owner>ActorType|Attribute|BlockType|Dimension|EffectType|Enchantment|ItemType|PotionType)\.(?P<name>[A-Z][A-Z0-9_]*)$:
    \from endstone import Identifier
    \name: Identifier[\owner] = \value
^endstone\.level\.__prefix__$:
    \import typing
    \from endstone import GameRule, Identifier
    _T = typing.TypeVar("_T")

# The bindings take a bare Identifier<GameRule>, so the value type has to come
# back from the rule. The fallback is str alone: widening it to Identifier[GameRule]
# would swallow the typed overload and lose the value check on assignment.
^endstone\.level\.Level\.get_game_rule$:
    @typing.overload
    def get_game_rule(self, rule: Identifier[GameRule[_T]]) -> _T: ...
    @typing.overload
    def get_game_rule(self, rule: str) -> bool | int | float:
        \doc

^endstone\.level\.Level\.set_game_rule$:
    @typing.overload
    def set_game_rule(self, rule: Identifier[GameRule[_T]], value: _T) -> None: ...
    @typing.overload
    def set_game_rule(self, rule: str, value: bool | int | float) -> None:
        \doc

^endstone\.event\.__prefix__$:
    \import typing
    _F = typing.TypeVar("_F", bound=(typing.Callable[..., None]))

^endstone\.event\.event_handler$:
    @typing.overload
    def event_handler(func: _F, /) -> _F: ...
    @typing.overload
    def event_handler(*, priority: EventPriority = ..., ignore_cancelled: bool = ...) -> typing.Callable[[_F], _F]: ...

# Bound only so the explode block list is mutable in place; ALIASES maps every
# reference to list[Block], so the class itself must not appear in the stubs.
^endstone\.block\.BlockList$:

