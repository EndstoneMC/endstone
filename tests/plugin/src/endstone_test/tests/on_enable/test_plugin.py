from endstone.plugin import Plugin

from endstone_test import EndstoneTest


def test_instance(plugin: Plugin):
    assert plugin is not None
    assert isinstance(plugin, EndstoneTest)
