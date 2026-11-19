import pytest
from endstone._python.plugin import Plugin as NativePlugin

from endstone.command import CommandMap, Commands, SoftEnum
from endstone.plugin import Plugin

# ==== SoftEnum ====


@pytest.mark.parametrize("name", ["name", "values"])
def test_soft_enum_property(name: str) -> None:
    """Verify SoftEnum exposes the getter as a read-only property."""
    prop = SoftEnum.__dict__[name]
    assert isinstance(prop, property)
    assert prop.fset is None


@pytest.mark.parametrize("name", ["set_values", "add_values", "remove_values"])
def test_soft_enum_mutator(name: str) -> None:
    """Verify SoftEnum binds each mutator as a method."""
    assert callable(getattr(SoftEnum, name))
    assert not isinstance(SoftEnum.__dict__[name], property)


# ==== Registrar ====


@pytest.mark.parametrize("name", ["register_command", "register_soft_enum"])
def test_plugin_registrar_method(name: str) -> None:
    """Verify the native Plugin binds each registrar method as a method."""
    assert callable(getattr(NativePlugin, name))
    assert not isinstance(NativePlugin.__dict__[name], property)


def test_plugin_commands_metadata_survives() -> None:
    """Verify a plugin's `commands` class attribute still reads as its metadata dict."""

    class MetadataPlugin(Plugin):
        commands = {"warp": {"description": "Teleports to a warp."}}

    assert "commands" not in NativePlugin.__dict__
    assert MetadataPlugin().commands == {"warp": {"description": "Teleports to a warp."}}


@pytest.mark.parametrize("name", ["literal", "argument"])
def test_commands_builder_factory(name: str) -> None:
    """Verify Commands keeps its static node builder factories."""
    assert callable(getattr(Commands, name))


def test_command_map_get_soft_enum() -> None:
    """Verify CommandMap binds get_soft_enum."""
    assert callable(CommandMap.get_soft_enum)
