import lazy_loader as lazy

from endstone._internal.version import __version__

__minecraft_version__ = "1.21.102"

__getattr__, __dir__, __all__ = lazy.attach(
    __name__,
    submod_attrs={
        "_python": [
            "__version__",
            "__minecraft_version__",
            "ColorFormat",
            "EnchantmentRegistry",
            "GameMode",
            "ItemRegistry",
            "Logger",
            "OfflinePlayer",
            "Player",
            "Server",
            "Skin",
        ],
    },
)

__all__.extend(
    [
        "__version__",
        "__minecraft_version__",
    ]
)
