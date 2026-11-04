import uuid

import pytest
from endstone import Server
from endstone.command import CommandSenderWrapper
from endstone.permissions import Permission, PermissionDefault
from endstone.plugin import Plugin


def unique(prefix: str = "apichildren") -> str:
    return f"{prefix}.{uuid.uuid4().hex}"


@pytest.fixture
def registered(server: Server):
    manager = server.plugin_manager
    added: list[str] = []

    def register(perm: Permission) -> Permission:
        result = manager.add_permission(perm)
        added.append(result.name)
        return result

    yield register
    for name in added:
        manager.remove_permission(name)


@pytest.fixture
def holder(server: Server, plugin: Plugin, registered):
    parent = registered(Permission(unique("apichildren.parent"), default=PermissionDefault.FALSE))
    sender = CommandSenderWrapper(server.command_sender)
    attachment = sender.add_attachment(plugin, parent.name, True)
    yield parent, sender
    attachment.remove()


def test_children_is_an_opaque_mapping_that_accepts_a_dict():
    a = unique()
    perm = Permission(unique(), children={a: True})
    assert type(perm.children).__name__ == "PermissionMap"
    assert not isinstance(perm.children, dict)
    assert dict(perm.children) == {a: True}
    assert Permission(unique(), children=perm.children).children == {a: True}


def test_attachment_permissions_is_a_dict_copy(server: Server, plugin: Plugin):
    name = unique("apichildren.attached")
    sender = CommandSenderWrapper(server.command_sender)
    attachment = sender.add_attachment(plugin, name, True)
    try:
        permissions = attachment.permissions
        assert isinstance(permissions, dict)
        assert permissions.get(name) is True
        assert len(permissions) == 1
        assert permissions[name] is True
        assert dict(permissions.items()) == {name: True}
        permissions[unique()] = False
        del permissions[name]
        assert attachment.permissions == {name: True}
    finally:
        attachment.remove()


def test_mapping_views_and_truthiness():
    a, b = unique(), unique()
    children = Permission(unique(), children={a: True, b: False}).children
    assert bool(children)
    assert not Permission(unique()).children
    assert len(children.keys()) == 2 and a in children.keys() and unique() not in children.keys()
    assert len(children.values()) == 2
    assert len(children.items()) == 2
    assert a in repr(children)


def test_mapping_reads_the_permission_children():
    a, b = unique(), unique()
    perm = Permission(unique(), children={a: True, b: False})
    children = perm.children
    assert len(children) == 2
    assert a in children and b in children
    assert unique() not in children
    assert set(iter(children)) == {a, b}
    assert set(children.keys()) == {a, b}
    assert sorted(children.values()) == [False, True]
    assert dict(children.items()) == {a: True, b: False}
    assert children[a] is True and children[b] is False
    with pytest.raises(KeyError):
        _ = children[unique()]
    assert children == {a: True, b: False}
    assert children != {a: True}


def test_edits_through_one_view_are_seen_by_another():
    a, b, c = unique(), unique(), unique()
    perm = Permission(unique(), children={a: True})
    perm.children[b] = False
    perm.children[c] = True
    assert perm.children == {a: True, b: False, c: True}
    del perm.children[a]
    del perm.children[b]
    with pytest.raises(KeyError):
        del perm.children[unique()]
    assert perm.children == {c: True}
    del perm.children[c]
    assert len(perm.children) == 0


def child_name(registered) -> str:
    return registered(Permission(unique("apichildren.child"), default=PermissionDefault.FALSE)).name


def test_removing_a_child_revokes_it_after_recalculate(holder, registered):
    parent, sender = holder
    child = child_name(registered)
    parent.children[child] = True
    parent.recalculate_permissibles()
    assert sender.has_permission(child)
    del parent.children[child]
    parent.recalculate_permissibles()
    assert not sender.has_permission(child)


def test_clearing_children_revokes_them_after_recalculate(holder, registered):
    parent, sender = holder
    first, second = child_name(registered), child_name(registered)
    parent.children[first] = True
    parent.children[second] = True
    parent.recalculate_permissibles()
    assert sender.has_permission(first) and sender.has_permission(second)
    del parent.children[first]
    del parent.children[second]
    parent.recalculate_permissibles()
    assert not sender.has_permission(first)
    assert not sender.has_permission(second)


def test_a_false_child_denies_after_recalculate(holder, registered):
    parent, sender = holder
    child = child_name(registered)
    parent.children[child] = True
    parent.recalculate_permissibles()
    assert sender.has_permission(child)
    parent.children[child] = False
    parent.recalculate_permissibles()
    assert sender.is_permission_set(child)
    assert not sender.has_permission(child)
