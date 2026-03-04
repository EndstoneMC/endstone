import lazy_loader as lazy

__getattr__, __dir__, __all__ = lazy.attach(
    "endstone._python",
    submod_attrs={
        "inventory": [
            "BookMeta",
            "BookMetaGeneration",
            "CrossbowMeta",
            "EquipmentSlot",
            "Inventory",
            "ItemFactory",
            "ItemMeta",
            "ItemStack",
            "ItemType",
            "MapMeta",
            "PlayerInventory",
            "WritableBookMeta",
        ],
    },
)
