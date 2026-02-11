import lazy_loader as lazy

__minecraft_version__ = "1.26.0"

__getattr__, __dir__, __all__ = lazy.attach(
    __name__,
    submod_attrs={
        "_python": [
            "ColorFormat",
            "EnchantmentRegistry",
            "GameMode",
            "ItemRegistry",
            "Logger",
            "OfflinePlayer",
            "Player",
            "Server",
            "Skin",
            "actor",
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
            "permissions",
            "plugin",
            "scheduler",
            "scoreboard",
            "util",
        ],
        "_version": ["__version__"],
    },
)

__all__.extend(["__minecraft_version__"])
