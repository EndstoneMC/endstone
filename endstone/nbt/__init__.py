import lazy_loader as lazy

__getattr__, __dir__, __all__ = lazy.attach(
    "endstone._python",
    submod_attrs={
        "nbt": [
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
        ],
    },
)
