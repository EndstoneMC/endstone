import typing

import lazy_loader as lazy

# The JSON shapes the API exchanges, so a plugin can name them the way the C++ side does. The
# bindings cannot declare these, so src/endstone/python/stubgen.pat repeats them for the stubs.
JsonValue: typing.TypeAlias = "JsonObject | JsonArray | str | int | float | bool | None"
JsonObject: typing.TypeAlias = "dict[str, JsonValue]"
JsonArray: typing.TypeAlias = "list[JsonValue]"

__getattr__, __dir__, __all__ = lazy.attach(
    __name__,
    submod_attrs={
        "_python": [
            "ColorFormat",
            "GameMode",
            "GameRule",
            "Identifier",
            "Input",
            "Logger",
            "OfflinePlayer",
            "Player",
            "Registry",
            "Rotation",
            "Server",
            "Skin",
            "__minecraft_version__",
            "actor",
            "attribute",
            "ban",
            "block",
            "boss",
            "command",
            "damage",
            "enchantments",
            "event",
            "form",
            "inventory",
            "lang",
            "level",
            "map",
            "metrics",
            "nbt",
            "permissions",
            "plugin",
            "potion",
            "scheduler",
            "scoreboard",
            "util",
        ],
        "_version": ["__version__"],
    },
)

__all__.extend(["JsonArray", "JsonObject", "JsonValue"])
