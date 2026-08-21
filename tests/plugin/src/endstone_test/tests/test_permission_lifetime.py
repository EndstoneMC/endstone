import gc

import pytest
from endstone import Server
from endstone.command import CommandSenderWrapper
from endstone.plugin import Plugin

PERMISSION = "endstone_test.lifetime"


def test_subscription_drops_collected_permissible(server: Server):
    plugin_manager = server.plugin_manager
    wrapper = CommandSenderWrapper(server.command_sender)
    plugin_manager.subscribe_to_permission(PERMISSION, wrapper)
    assert wrapper in plugin_manager.get_permission_subscriptions(PERMISSION)

    del wrapper
    gc.collect()
    assert plugin_manager.get_permission_subscriptions(PERMISSION) == set()
    server.broadcast("nobody is listening", PERMISSION)


def test_subscription_keeps_live_permissible(server: Server):
    plugin_manager = server.plugin_manager
    wrapper = CommandSenderWrapper(server.command_sender)
    plugin_manager.subscribe_to_permission(PERMISSION, wrapper)
    gc.collect()
    assert wrapper in plugin_manager.get_permission_subscriptions(PERMISSION)

    plugin_manager.unsubscribe_from_permission(PERMISSION, wrapper)
    assert plugin_manager.get_permission_subscriptions(PERMISSION) == set()


def test_subscribe_rejects_none(server: Server):
    with pytest.raises(TypeError):
        server.plugin_manager.subscribe_to_permission(PERMISSION, None)


def test_attachment_survives_removal(server: Server, plugin: Plugin):
    sender = server.command_sender
    attachment = sender.add_attachment(plugin, "endstone_test.granted", True)
    assert attachment.permissible is sender
    assert sender.has_permission("endstone_test.granted")

    assert sender.remove_attachment(attachment) is True
    assert not sender.has_permission("endstone_test.granted")
    assert sender.remove_attachment(attachment) is False
    assert attachment.permissions == {"endstone_test.granted": True}


def test_add_attachment_rejects_empty_name(server: Server, plugin: Plugin):
    with pytest.raises(ValueError):
        server.command_sender.add_attachment(plugin, "", True)
