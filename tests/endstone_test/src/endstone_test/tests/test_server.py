import re
import uuid
from datetime import datetime, timezone
from pathlib import Path

import pytest
from endstone import Server
from endstone.plugin import Plugin

# =============================================================================
# Fixtures
# =============================================================================


@pytest.fixture(scope="session")
def server_properties(plugin: Plugin) -> dict[str, str]:
    """Parse server.properties file and return as a dictionary."""
    properties_file = Path(
        plugin.data_folder, "..", "..", "server.properties"
    ).resolve()
    properties = {}

    with properties_file.open(mode="r") as file:
        for line in file:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            splits = line.split("=", 1)
            if len(splits) == 2:
                properties[splits[0]] = splits[1]

    return properties


# =============================================================================
# Section 1: Server Information Tests
# =============================================================================


def test_server_name(server: Server) -> None:
    """Verify server name is 'Endstone'."""
    assert server.name == "Endstone"


def test_server_version(server: Server) -> None:
    """Verify server version is a non-empty string."""
    assert isinstance(server.version, str)
    assert len(server.version) > 0


def test_minecraft_version(server: Server) -> None:
    """Verify minecraft_version follows expected format (e.g., '1.21.50')."""
    version = server.minecraft_version
    assert isinstance(version, str)
    assert len(version) > 0
    # Should match pattern like X.Y.Z or X.Y.Z.W
    assert re.match(r"^\d+\.\d+\.\d+(\.\d+)?$", version)


def test_protocol_version(server: Server) -> None:
    """Verify protocol_version is a positive integer."""
    protocol = server.protocol_version
    assert isinstance(protocol, int)
    assert protocol > 0


# =============================================================================
# Section 2: Server Components Tests
# =============================================================================


def test_logger_accessible(server: Server) -> None:
    """Verify logger is accessible and usable."""
    logger = server.logger
    assert logger is not None
    # Should be able to log without errors
    logger.info("Test log message from test_server.py")


def test_language_accessible(server: Server) -> None:
    """Verify language interface is accessible."""
    language = server.language
    assert language is not None


def test_plugin_manager(server: Server) -> None:
    """Verify plugin_manager is accessible and functional."""
    pm = server.plugin_manager
    assert pm is not None
    # Should have at least the test plugin loaded
    plugins = pm.plugins
    assert isinstance(plugins, list)
    assert len(plugins) > 0


def test_get_plugin_command_valid(server: Server) -> None:
    """Test get_plugin_command with a known command."""
    # 'test' command is registered by endstone_test plugin
    cmd = server.get_plugin_command("test")
    assert cmd is not None
    assert cmd.name == "test"


def test_get_plugin_command_invalid(server: Server) -> None:
    """Test get_plugin_command with non-existent command returns None."""
    cmd = server.get_plugin_command("nonexistent_command_xyz")
    assert cmd is None


def test_command_sender(server: Server) -> None:
    """Verify command_sender is accessible."""
    sender = server.command_sender
    assert sender is not None
    # Console command sender should have a name
    assert sender.name == "Server"


def test_dispatch_command(server: Server) -> None:
    """Test dispatching commands through the server."""
    # scriptevent command should return True on success
    result = server.dispatch_command(
        server.command_sender, "scriptevent endstone:test Hello World!"
    )
    assert result is True


def test_scheduler_accessible(server: Server) -> None:
    """Verify scheduler is accessible."""
    scheduler = server.scheduler
    assert scheduler is not None


def test_service_manager(server: Server) -> None:
    """Verify service_manager is accessible."""
    sm = server.service_manager
    assert sm is not None


# =============================================================================
# Section 3: Level Tests
# =============================================================================


def test_level_accessible(server: Server) -> None:
    """Verify level is accessible and not None."""
    level = server.level
    assert level is not None
    # Level should have a name
    assert level.name is not None


# =============================================================================
# Section 4: Player Management Tests
# =============================================================================


def test_online_players(server: Server) -> None:
    """Verify online_players returns a list."""
    players = server.online_players
    assert isinstance(players, list)
    # May be empty if no players connected


def test_max_players(server: Server) -> None:
    """Test getting and setting max players."""
    # Set max players using command
    server.dispatch_command(server.command_sender, "setmaxplayers 5")
    assert server.max_players == 5

    # Set max players directly
    server.max_players = 100
    assert server.max_players == 100


def test_get_player_by_name(server: Server) -> None:
    """Test get_player with invalid name returns None."""
    assert server.get_player("nonexistent_player_xyz") is None


def test_get_player_by_uuid(server: Server) -> None:
    """Test get_player with invalid UUID returns None."""
    # Create a random UUID that shouldn't match any player
    random_uuid = uuid.uuid4()
    assert server.get_player(random_uuid) is None


# =============================================================================
# Section 5: Network Configuration Tests
# =============================================================================


def test_server_port(server: Server, server_properties: dict[str, str]) -> None:
    """Verify server port matches server.properties."""
    expected_port = int(server_properties["server-port"])
    assert server.port == expected_port


def test_server_port_v6(server: Server, server_properties: dict[str, str]) -> None:
    """Verify server IPv6 port matches server.properties."""
    expected_port = int(server_properties["server-portv6"])
    assert server.port_v6 == expected_port


def test_online_mode(server: Server, server_properties: dict[str, str]) -> None:
    """Verify online mode matches server.properties."""
    expected_online_mode = server_properties["online-mode"].lower() == "true"
    assert server.online_mode == expected_online_mode


# =============================================================================
# Section 6: Performance Metrics Tests
# =============================================================================


def test_current_mspt(server: Server) -> None:
    """Verify current_mspt returns a non-negative float."""
    mspt = server.current_mspt
    assert isinstance(mspt, float)
    assert mspt >= 0


def test_average_mspt(server: Server) -> None:
    """Verify average_mspt returns a non-negative float."""
    mspt = server.average_mspt
    assert isinstance(mspt, float)
    assert mspt >= 0


def test_current_tps(server: Server) -> None:
    """Verify current_tps returns a valid TPS value (0-20)."""
    tps = server.current_tps
    assert isinstance(tps, float)
    assert 0 <= tps <= 20


def test_average_tps(server: Server) -> None:
    """Verify average_tps returns a valid TPS value (0-20)."""
    tps = server.average_tps
    assert isinstance(tps, float)
    assert 0 <= tps <= 20


def test_current_tick_usage(server: Server) -> None:
    """Verify current_tick_usage returns a non-negative float."""
    usage = server.current_tick_usage
    assert isinstance(usage, float)
    assert usage >= 0


def test_average_tick_usage(server: Server) -> None:
    """Verify average_tick_usage returns a non-negative float."""
    usage = server.average_tick_usage
    assert isinstance(usage, float)
    assert usage >= 0


def test_start_time(server: Server) -> None:
    """Verify start_time returns a datetime in the past."""
    start = server.start_time
    assert isinstance(start, datetime)
    # Start time should be in the past
    now = datetime.now(timezone.utc)
    # Handle both timezone-aware and naive datetimes
    if start.tzinfo is None:
        start = start.replace(tzinfo=timezone.utc)
    assert start <= now


# =============================================================================
# Section 7: Item Factory Tests
# =============================================================================


def test_item_factory(server: Server) -> None:
    """Verify item_factory is accessible."""
    factory = server.item_factory
    assert factory is not None


# =============================================================================
# Section 8: Map Tests
# =============================================================================


def test_get_map_invalid(server: Server) -> None:
    """Test get_map with non-existent map ID returns None."""
    # Using a very large ID that shouldn't exist
    result = server.get_map(999999)
    assert result is None


def test_create_map(server: Server) -> None:
    """Test creating a new map."""
    level = server.level
    assert level is not None
    dimension = level.get_dimension("overworld")
    assert dimension is not None
    view = server.create_map(dimension)
    assert view.id != -1, f"Expected valid map id, got {view.id}"
    assert view.scale == 3, f"Expected scale 3, got {view.scale}"
    assert view.center_x == view.center_z, f"Expected center_x == center_z, got {view.center_x} != {view.center_z}"
    assert view.center_x == 448, f"Expected center_x 448, got {view.center_x}"


# =============================================================================
# Section 9: Broadcast Tests
# =============================================================================


def test_broadcast_message(server: Server) -> None:
    """Test broadcasting a message (should not crash)."""
    # This should work even with no players online
    server.broadcast_message("Test broadcast from test_server.py")
    # If we get here without exception, the test passes
