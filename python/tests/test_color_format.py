import importlib

import pytest


@pytest.fixture
def cls():
    module = importlib.import_module("endstone.util")
    return getattr(module, "ColorFormat")


def test_color_format(cls):
    assert getattr(cls, "BLACK") == "§0"
    assert getattr(cls, "DARK_BLUE") == "§1"
    assert getattr(cls, "DARK_GREEN") == "§2"
    assert getattr(cls, "DARK_AQUA") == "§3"
    assert getattr(cls, "DARK_RED") == "§4"
    assert getattr(cls, "DARK_PURPLE") == "§5"
    assert getattr(cls, "GOLD") == "§6"
    assert getattr(cls, "GRAY") == "§7"
    assert getattr(cls, "DARK_GRAY") == "§8"
    assert getattr(cls, "BLUE") == "§9"
    assert getattr(cls, "GREEN") == "§a"
    assert getattr(cls, "AQUA") == "§b"
    assert getattr(cls, "RED") == "§c"
    assert getattr(cls, "LIGHT_PURPLE") == "§d"
    assert getattr(cls, "YELLOW") == "§e"
    assert getattr(cls, "WHITE") == "§f"
    assert getattr(cls, "MINECOIN_GOLD") == "§g"
    assert getattr(cls, "MATERIAL_QUARTZ") == "§h"
    assert getattr(cls, "MATERIAL_IRON") == "§i"
    assert getattr(cls, "MATERIAL_NETHERITE") == "§j"
    assert getattr(cls, "MATERIAL_REDSTONE") == "§m"
    assert getattr(cls, "MATERIAL_COPPER") == "§n"
    assert getattr(cls, "MATERIAL_GOLD") == "§p"
    assert getattr(cls, "MATERIAL_EMERALD") == "§q"
    assert getattr(cls, "MATERIAL_DIAMOND") == "§s"
    assert getattr(cls, "MATERIAL_LAPIS") == "§t"
    assert getattr(cls, "MATERIAL_AMETHYST") == "§u"

    assert getattr(cls, "OBFUSCATED") == "§k"
    assert getattr(cls, "BOLD") == "§l"
    assert getattr(cls, "ITALIC") == "§o"
    assert getattr(cls, "RESET") == "§r"
