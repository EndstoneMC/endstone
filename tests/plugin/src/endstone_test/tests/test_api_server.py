import logging
import tomllib
import uuid
from pathlib import Path

import pytest
from endstone import Server
from endstone.command import CommandSenderWrapper
from endstone.metrics import Metrics, MetricsBase, MetricsConfig
from endstone.permissions import (
    Permission,
    PermissionAttachment,
    PermissionAttachmentInfo,
    PermissionDefault,
    PermissionLevel,
)
from endstone.plugin import Plugin, Service, ServicePriority

FIXTURE = "test_fixture"
SHIPPED_CONFIG = {"greeting": "hello", "limit": 3}


def unique(prefix: str) -> str:
    return f"{prefix}.{uuid.uuid4().hex}"


class QuietMetrics(MetricsBase):
    def __init__(self, config: MetricsConfig):
        super().__init__("server-implementation", config.server_uuid, 1, False, False, False)

    @property
    def enabled(self) -> bool:
        return super().enabled

    @property
    def service_enabled(self) -> bool:
        return super().service_enabled


def test_metrics_config_creates_its_file_once(tmp_path: Path) -> None:
    file = tmp_path / "bstats" / "config.toml"
    file.parent.mkdir()
    first = MetricsConfig(file, True)
    assert first.did_exist_before is False
    assert file.exists()
    assert first.enabled is True
    saved = tomllib.loads(file.read_text(encoding="utf-8"))
    assert saved["enabled"] is True
    assert uuid.UUID(saved["server-uuid"]) == first.server_uuid

    second = MetricsConfig(file, False)
    assert second.did_exist_before is True
    assert second.server_uuid == first.server_uuid
    assert second.enabled is True


def test_metrics_config_default_enabled_seeds_new_files_only(tmp_path: Path) -> None:
    disabled = MetricsConfig(tmp_path / "off.toml", False)
    assert disabled.did_exist_before is False
    assert disabled.enabled is False
    assert MetricsConfig(tmp_path / "off.toml", True).enabled is False


def test_metrics_base_is_abstract() -> None:
    with pytest.raises(TypeError):
        MetricsBase("p", uuid.uuid4(), 1, False, False, False)


def test_metrics_base_defaults(tmp_path: Path, caplog: pytest.LogCaptureFixture) -> None:
    metrics = QuietMetrics(MetricsConfig(tmp_path / "config.toml", False))
    assert metrics.enabled is None
    assert metrics.service_enabled is None

    data = {"kept": 1}
    metrics.append_platform_data(data)
    metrics.append_service_data(data)
    assert data == {"kept": 1}

    ran = []
    metrics.submit_task(lambda: ran.append(1))
    assert ran == [1]

    with caplog.at_level(logging.INFO):
        metrics.log_info("metrics base info")
        error = ValueError("boom")
        metrics.log_error("metrics base error", error)
    by_message = {record.getMessage(): record for record in caplog.records}
    assert by_message["metrics base info"].levelno == logging.INFO
    failure = by_message["metrics base error"]
    assert failure.levelno == logging.WARNING
    assert failure.exc_info is not None and failure.exc_info[1] is error
    metrics.shutdown()


def test_metrics_platform_data_describes_the_server(server: Server, plugin: Plugin) -> None:
    metrics = Metrics(plugin, 1)
    try:
        data: dict = {}
        metrics.append_platform_data(data)
        assert data["playerAmount"] == len(server.online_players)
        assert data["onlineMode"] == server.online_mode
        assert data["bukkitName"] == server.name
        assert data["bukkitVersion"] == f"{server.version} (MC: {server.minecraft_version})"
        assert data["osName"] and data["osArch"]
        assert isinstance(data["coreCount"], (int, type(None)))

        service: dict = {}
        metrics.append_service_data(service)
        assert service == {"pluginVersion": plugin._get_description().version}
    finally:
        metrics.shutdown()


def test_attachment_constructor_builds_a_detached_attachment(server: Server, plugin: Plugin) -> None:
    console = server.command_sender
    attachment = PermissionAttachment(plugin, console)
    name = unique("endstone_test.detached")
    assert attachment.plugin.name == plugin.name
    assert attachment.permissible is console
    assert attachment.permissions == {}

    attachment.set_permission(name, True)
    assert attachment.permissions == {name: True}
    assert not console.is_permission_set(name)
    assert attachment.remove() is False


def test_attachment_removal_callback_runs_on_removal(server: Server, plugin: Plugin) -> None:
    console = server.command_sender
    attachment = console.add_attachment(plugin, unique("endstone_test.callback"), True)
    assert attachment.removal_callback is None

    removed = []
    attachment.removal_callback = removed.append
    assert attachment.removal_callback is not None
    assert console.remove_attachment(attachment) is True
    assert len(removed) == 1
    assert removed[0].permissions == attachment.permissions
    assert console.remove_attachment(attachment) is False
    assert len(removed) == 1


def test_attachment_removal_callback_runs_on_remove(server: Server, plugin: Plugin) -> None:
    attachment = server.command_sender.add_attachment(plugin, unique("endstone_test.callback"), True)
    removed = []
    attachment.removal_callback = lambda a: removed.append(a.permissions)
    assert attachment.remove() is True
    assert removed == [attachment.permissions]


def test_attachment_removal_callback_can_be_cleared(server: Server, plugin: Plugin) -> None:
    attachment = server.command_sender.add_attachment(plugin, unique("endstone_test.callback"), True)
    removed = []
    attachment.removal_callback = removed.append
    attachment.removal_callback = None
    assert attachment.removal_callback is None
    assert attachment.remove() is True
    assert removed == []


def test_attachment_info_holds_what_it_was_built_with(server: Server, plugin: Plugin) -> None:
    console = server.command_sender
    name = unique("endstone_test.info")
    attachment = console.add_attachment(plugin, name, True)
    try:
        info = PermissionAttachmentInfo(console, name, attachment, False)
        assert info.permissible is console
        assert info.permission == name
        assert info.attachment is attachment
        assert info.value is False

        orphan = PermissionAttachmentInfo(console, name, None, True)
        assert orphan.attachment is None
        assert orphan.value is True
    finally:
        attachment.remove()


def test_attachment_info_matches_the_effective_permissions(server: Server, plugin: Plugin) -> None:
    console = server.command_sender
    name = unique("endstone_test.effective")
    attachment = console.add_attachment(plugin, name, True)
    try:
        info = next(i for i in console.effective_permissions if i.permission == name)
        assert info.permissible is console
        assert info.value is True
        assert info.attachment is attachment
        rebuilt = PermissionAttachmentInfo(console, info.permission, info.attachment, info.value)
        assert (rebuilt.permissible, rebuilt.permission, rebuilt.attachment, rebuilt.value) == (
            info.permissible,
            info.permission,
            info.attachment,
            info.value,
        )
    finally:
        attachment.remove()


@pytest.fixture()
def fixture_plugin(server: Server):
    plugin = server.plugin_manager.get_plugin(FIXTURE)
    if plugin is None:
        pytest.skip(f"{FIXTURE} is not loaded")
    folder = Path(plugin.data_folder)
    created = not folder.exists()
    folder.mkdir(exist_ok=True)
    plugin._config = None
    yield plugin
    plugin._config = None
    for leftover in (folder / "config.toml", folder / "locale" / "en.toml"):
        leftover.unlink(missing_ok=True)
    if (folder / "locale").is_dir():
        (folder / "locale").rmdir()
    if created and folder.is_dir() and not any(folder.iterdir()):
        folder.rmdir()


def read_config(plugin: Plugin) -> dict:
    return tomllib.loads((Path(plugin.data_folder) / "config.toml").read_text(encoding="utf-8"))


def test_save_default_config_copies_the_packaged_default(fixture_plugin: Plugin) -> None:
    target = Path(fixture_plugin.data_folder) / "config.toml"
    target.unlink(missing_ok=True)
    fixture_plugin.save_default_config()
    assert read_config(fixture_plugin) == SHIPPED_CONFIG
    assert dict(fixture_plugin.config) == SHIPPED_CONFIG


def test_save_default_config_keeps_an_existing_file(fixture_plugin: Plugin) -> None:
    target = Path(fixture_plugin.data_folder) / "config.toml"
    target.parent.mkdir(exist_ok=True)
    target.write_text('greeting = "mine"\n', encoding="utf-8")
    fixture_plugin.save_default_config()
    assert read_config(fixture_plugin) == {"greeting": "mine"}


def test_config_is_cached_until_reloaded(fixture_plugin: Plugin) -> None:
    target = Path(fixture_plugin.data_folder) / "config.toml"
    target.parent.mkdir(exist_ok=True)
    target.write_text("limit = 1\n", encoding="utf-8")
    assert fixture_plugin.config["limit"] == 1

    target.write_text("limit = 2\n", encoding="utf-8")
    assert fixture_plugin.config["limit"] == 1
    assert fixture_plugin.reload_config()["limit"] == 2
    assert fixture_plugin.config["limit"] == 2


def test_save_config_writes_changes_back(fixture_plugin: Plugin) -> None:
    target = Path(fixture_plugin.data_folder) / "config.toml"
    target.parent.mkdir(exist_ok=True)
    target.write_text("# kept\nlimit = 1\n", encoding="utf-8")
    fixture_plugin.config["limit"] = 5
    fixture_plugin.config["added"] = "yes"
    fixture_plugin.save_config()
    assert read_config(fixture_plugin) == {"limit": 5, "added": "yes"}
    assert target.read_text(encoding="utf-8").startswith("# kept")


def test_save_config_without_a_loaded_config_writes_nothing(fixture_plugin: Plugin) -> None:
    target = Path(fixture_plugin.data_folder) / "config.toml"
    target.unlink(missing_ok=True)
    fixture_plugin.save_config()
    assert not target.exists()


def test_reload_config_without_a_file_raises(fixture_plugin: Plugin) -> None:
    (Path(fixture_plugin.data_folder) / "config.toml").unlink(missing_ok=True)
    with pytest.raises(FileNotFoundError):
        fixture_plugin.reload_config()


def test_save_resources_replace_flag(fixture_plugin: Plugin) -> None:
    target = Path(fixture_plugin.data_folder) / "config.toml"
    target.unlink(missing_ok=True)
    fixture_plugin.save_resources("config.toml")
    assert read_config(fixture_plugin) == SHIPPED_CONFIG

    target.write_text("limit = 99\n", encoding="utf-8")
    fixture_plugin.save_resources("config.toml")
    assert read_config(fixture_plugin) == {"limit": 99}
    fixture_plugin.save_resources("config.toml", False)
    assert read_config(fixture_plugin) == {"limit": 99}

    fixture_plugin.save_resources("config.toml", replace=True)
    assert read_config(fixture_plugin) == SHIPPED_CONFIG


def test_save_resources_keeps_the_relative_path(fixture_plugin: Plugin) -> None:
    fixture_plugin.save_resources("locale/en.toml")
    copied = Path(fixture_plugin.data_folder) / "locale" / "en.toml"
    assert tomllib.loads(copied.read_text(encoding="utf-8")) == {"name": "English"}


def test_save_resources_rejects_a_missing_resource(fixture_plugin: Plugin) -> None:
    with pytest.raises(FileNotFoundError):
        fixture_plugin.save_resources("no_such_resource.toml")
    assert not (Path(fixture_plugin.data_folder) / "no_such_resource.toml").exists()


def default_names(server: Server, level: PermissionLevel) -> list[str]:
    return [p.name for p in server.plugin_manager.get_default_permissions(level)]


def test_permissions_property_lists_every_registered_permission(server: Server) -> None:
    manager = server.plugin_manager
    name = unique("endstone_test.listed")
    assert name not in {p.name for p in manager.permissions}
    added = manager.add_permission(Permission(name, "listed by test"))
    try:
        listed = {p.name: p for p in manager.permissions}
        assert listed[name] is added
        assert "minecraft.command.kick" in listed
        assert all(manager.get_permission(n) is p for n, p in listed.items())
    finally:
        manager.remove_permission(name)
    assert name not in {p.name for p in manager.permissions}


@pytest.mark.parametrize(
    ("default", "levels"),
    [
        (PermissionDefault.TRUE, {PermissionLevel.DEFAULT, PermissionLevel.OP, PermissionLevel.CONSOLE}),
        (PermissionDefault.FALSE, set()),
        (PermissionDefault.OP, {PermissionLevel.OP, PermissionLevel.CONSOLE}),
        (PermissionDefault.NOT_OP, {PermissionLevel.DEFAULT}),
    ],
)
def test_recalculate_permission_defaults_files_a_permission_by_default(
    server: Server, default: PermissionDefault, levels: set[PermissionLevel]
) -> None:
    manager = server.plugin_manager
    name = unique("endstone_test.recalc")
    perm = manager.add_permission(Permission(name, "recalc", default))
    try:
        for _ in range(2):
            manager.recalculate_permission_defaults(perm)
            for level in PermissionLevel:
                assert default_names(server, level).count(name) == (1 if level in levels else 0), (default, level)
    finally:
        manager.remove_permission(name)


def test_recalculate_permission_defaults_moves_a_permission_to_its_new_default(server: Server) -> None:
    manager = server.plugin_manager
    name = unique("endstone_test.recalc")
    perm = manager.add_permission(Permission(name, "recalc", PermissionDefault.FALSE))
    try:
        assert all(name not in default_names(server, level) for level in PermissionLevel)
        perm.default = PermissionDefault.NOT_OP
        manager.recalculate_permission_defaults(perm)
        assert default_names(server, PermissionLevel.DEFAULT).count(name) == 1
        assert name not in default_names(server, PermissionLevel.OP)
        perm.default = PermissionDefault.OP
        manager.recalculate_permission_defaults(perm)
        assert name not in default_names(server, PermissionLevel.DEFAULT)
        assert default_names(server, PermissionLevel.OP).count(name) == 1
        assert default_names(server, PermissionLevel.CONSOLE).count(name) == 1
    finally:
        manager.remove_permission(name)


def test_recalculate_permission_defaults_ignores_an_unregistered_permission(server: Server) -> None:
    perm = Permission(unique("endstone_test.stranger"), "never added", PermissionDefault.TRUE)
    server.plugin_manager.recalculate_permission_defaults(perm)
    assert all(perm.name not in default_names(server, level) for level in PermissionLevel)


def test_default_perm_subscriptions_are_kept_per_level(server: Server) -> None:
    manager = server.plugin_manager
    first = CommandSenderWrapper(server.command_sender)
    second = CommandSenderWrapper(server.command_sender)
    levels = list(PermissionLevel)
    before = {level: manager.get_default_perm_subscriptions(level) for level in levels}
    try:
        manager.subscribe_to_default_perms(PermissionLevel.OP, first)
        manager.subscribe_to_default_perms(PermissionLevel.OP, first)
        manager.subscribe_to_default_perms(PermissionLevel.DEFAULT, second)
        assert manager.get_default_perm_subscriptions(PermissionLevel.OP) == before[PermissionLevel.OP] | {first}
        assert manager.get_default_perm_subscriptions(PermissionLevel.DEFAULT) == before[PermissionLevel.DEFAULT] | {
            second
        }
        assert manager.get_default_perm_subscriptions(PermissionLevel.CONSOLE) == before[PermissionLevel.CONSOLE]

        manager.unsubscribe_from_default_perms(PermissionLevel.DEFAULT, first)
        assert first in manager.get_default_perm_subscriptions(PermissionLevel.OP)
        manager.unsubscribe_from_default_perms(PermissionLevel.OP, first)
        assert first not in manager.get_default_perm_subscriptions(PermissionLevel.OP)
        manager.unsubscribe_from_default_perms(PermissionLevel.OP, first)
        assert second in manager.get_default_perm_subscriptions(PermissionLevel.DEFAULT)
    finally:
        for level in levels:
            manager.unsubscribe_from_default_perms(level, first)
            manager.unsubscribe_from_default_perms(level, second)
    assert {level: manager.get_default_perm_subscriptions(level) for level in levels} == before


def test_default_perm_subscribe_rejects_none(server: Server) -> None:
    with pytest.raises(TypeError):
        server.plugin_manager.subscribe_to_default_perms(PermissionLevel.OP, None)


def test_plugin_loader_describes_itself(server: Server, plugin: Plugin) -> None:
    loader = plugin.plugin_loader
    assert loader.server is server
    filters = loader.plugin_file_filters
    assert filters == ["\\.whl"]
    assert isinstance(filters, list)
    assert loader.server.plugin_manager.get_plugin(plugin.name) is plugin


class Provider(Service):
    def __init__(self, label: str):
        Service.__init__(self)
        self.label = label


def test_service_manager_returns_the_highest_priority_provider(server: Server, plugin: Plugin) -> None:
    services = server.service_manager
    name = unique("endstone_test.service")
    low = Provider("low")
    high = Provider("high")
    normal = Provider("normal")
    try:
        assert services.load(name) is None
        services.register(name, low, plugin, ServicePriority.LOW)
        assert services.load(name) is low
        services.register(name, high, plugin, ServicePriority.HIGHEST)
        services.register(name, normal, plugin, ServicePriority.NORMAL)
        assert services.load(name) is high
        services.unregister(name, high)
        assert services.load(name) is normal
        services.unregister(name, normal)
        assert services.load(name) is low
        services.unregister(name, low)
        assert services.load(name) is None
    finally:
        services.unregister_all(plugin)


def test_service_manager_unregisters_a_provider_everywhere(server: Server, plugin: Plugin) -> None:
    services = server.service_manager
    first, second = unique("endstone_test.service"), unique("endstone_test.service")
    shared = Provider("shared")
    other = Provider("other")
    try:
        services.register(first, shared, plugin, ServicePriority.NORMAL)
        services.register(second, shared, plugin, ServicePriority.NORMAL)
        services.register(second, other, plugin, ServicePriority.LOWEST)
        services.unregister(shared)
        assert services.load(first) is None
        assert services.load(second) is other
    finally:
        services.unregister_all(plugin)


def test_service_manager_unregister_all_drops_every_provider_of_a_plugin(server: Server, plugin: Plugin) -> None:
    services = server.service_manager
    names = [unique("endstone_test.service") for _ in range(3)]
    try:
        for name in names:
            services.register(name, Provider(name), plugin, ServicePriority.NORMAL)
        assert all(services.load(name) is not None for name in names)
        services.unregister_all(plugin)
        assert all(services.load(name) is None for name in names)
    finally:
        services.unregister_all(plugin)


def test_service_manager_unregister_of_an_unknown_provider_is_quiet(server: Server) -> None:
    services = server.service_manager
    stray = Provider("stray")
    services.unregister(stray)
    services.unregister(unique("endstone_test.service"), stray)


def test_save_resources_creates_the_data_folder(fixture_plugin: Plugin) -> None:
    folder = Path(fixture_plugin.data_folder)
    for leftover in folder.rglob("*"):
        if leftover.is_file():
            leftover.unlink()
    for leftover in sorted(folder.rglob("*"), reverse=True):
        leftover.rmdir()
    folder.rmdir()
    fixture_plugin.save_resources("locale/en.toml")
    assert (folder / "locale" / "en.toml").is_file()
