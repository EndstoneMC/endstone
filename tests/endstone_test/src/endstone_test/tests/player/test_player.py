import re
import uuid

import pytest
from endstone import GameMode, Player, Server

# =============================================================================
# Section 1: Identity & Online Status
# =============================================================================


def test_player_in_online_players(player: Player, server: Server):
    """Verify the player appears in the server's online player list and is retrievable by name and UUID."""
    assert player in server.online_players
    assert server.get_player(player.name) is player
    assert server.get_player(player.unique_id) is player


def test_player_xuid(player: Player):
    """Verify the player has a non-empty Xbox User ID."""
    assert isinstance(player.xuid, str)
    assert len(player.xuid) > 0


def test_player_unique_id(player: Player):
    """Verify the player's unique_id is a valid UUID instance."""
    assert isinstance(player.unique_id, uuid.UUID)


# =============================================================================
# Section 2: Health
# =============================================================================


def test_player_health(player: Player):
    """Verify health can be read, written, and restored."""
    current_health = player.health
    assert 0 <= current_health <= player.max_health

    player.health = 11
    assert player.health == 11

    player.health = current_health
    assert player.health == current_health


def test_player_max_health(player: Player):
    """Verify max_health can be read, written, and restored."""
    current_max = player.max_health
    assert current_max > 0

    player.max_health = 30
    assert player.max_health == 30

    player.max_health = current_max
    assert player.max_health == current_max


def test_player_set_health_out_of_range(player: Player):
    """Verify setting health outside [0, max_health] raises ValueError."""
    with pytest.raises(ValueError):
        player.health = -1
    with pytest.raises(ValueError):
        player.health = player.max_health + 1


def test_player_set_max_health_invalid(player: Player):
    """Verify setting max_health to zero or negative raises ValueError."""
    with pytest.raises(ValueError):
        player.max_health = 0
    with pytest.raises(ValueError):
        player.max_health = -1


# =============================================================================
# Section 3: Experience
# =============================================================================


def test_player_give_exp_levels(player: Player):
    """Verify give_exp_levels increments the player's experience level."""
    current_exp_lvl = player.exp_level
    assert current_exp_lvl >= 0

    player.give_exp_levels(2)
    assert player.exp_level == current_exp_lvl + 2

    player.exp_level = current_exp_lvl


def test_player_exp_level(player: Player):
    """Verify exp_level can be read, written, and restored."""
    current_exp_lvl = player.exp_level
    assert current_exp_lvl >= 0

    player.exp_level = current_exp_lvl + 1
    assert player.exp_level == current_exp_lvl + 1

    player.exp_level = current_exp_lvl
    assert player.exp_level == current_exp_lvl


def test_player_exp_progress(player: Player):
    """Verify exp_progress can be read, written, and restored."""
    current_exp_progress = player.exp_progress
    assert 0.0 <= current_exp_progress <= 1.0

    player.exp_progress = 1.0 - current_exp_progress
    assert abs(player.exp_progress - (1.0 - current_exp_progress)) <= 0.00001

    player.exp_progress = current_exp_progress
    assert player.exp_progress == current_exp_progress


def test_player_total_exp(player: Player):
    """Verify total experience points is a non-negative integer."""
    assert isinstance(player.total_exp, int)
    assert player.total_exp >= 0


def test_player_set_exp_progress_out_of_range(player: Player):
    """Verify setting exp_progress outside [0.0, 1.0] raises ValueError."""
    with pytest.raises(ValueError):
        player.exp_progress = -0.1
    with pytest.raises(ValueError):
        player.exp_progress = 1.1


def test_player_set_exp_level_negative(player: Player):
    """Verify setting a negative exp_level raises ValueError."""
    with pytest.raises(ValueError):
        player.exp_level = -1


# =============================================================================
# Section 4: Attributes & Movement
# =============================================================================


def test_player_attributes(player: Player):
    """Verify default fly and walk speed values."""
    assert abs(player.fly_speed - 0.05) <= 0.00001
    assert abs(player.walk_speed - 0.10) <= 0.00001


def test_player_is_sneaking(player: Player):
    """Verify is_sneaking returns a boolean."""
    assert isinstance(player.is_sneaking, bool)


def test_player_is_sprinting(player: Player):
    """Verify is_sprinting returns a boolean."""
    assert isinstance(player.is_sprinting, bool)


def test_player_allow_flight(player: Player):
    """Verify allow_flight can be toggled and restored."""
    original = player.allow_flight
    player.allow_flight = not original
    assert player.allow_flight == (not original)
    player.allow_flight = original
    assert player.allow_flight == original


def test_player_is_flying(player: Player):
    """Verify is_flying can be toggled and restored when flight is allowed."""
    original_flight = player.allow_flight
    original_flying = player.is_flying

    player.allow_flight = True
    player.is_flying = not original_flying
    assert player.is_flying == (not original_flying)

    player.is_flying = original_flying
    player.allow_flight = original_flight


# =============================================================================
# Section 5: Game Mode
# =============================================================================


def test_player_game_mode(player: Player):
    """Verify game mode can be changed and restored."""
    current_game_mode = player.game_mode
    player.game_mode = GameMode.SPECTATOR
    assert player.game_mode == GameMode.SPECTATOR
    player.game_mode = current_game_mode
    assert player.game_mode == current_game_mode


# =============================================================================
# Section 6: Appearance
# =============================================================================


def test_player_skin(player: Player):
    """Verify player skin image has 4 channels (RGBA)."""
    skin = player.skin
    assert skin.image.shape[2] == 4
    if skin.cape_image is not None:
        assert skin.cape_image.shape[2] == 4


def test_player_set_name_tag(player: Player):
    """Verify name tag can be changed and restored."""
    assert player.name_tag == player.name

    player.name_tag = "Test"
    assert player.name_tag == "Test"

    player.name_tag = ""
    assert player.name_tag == ""

    player.name_tag = player.name


# =============================================================================
# Section 7: Scoreboard Tags
# =============================================================================


def test_player_get_tags(player: Player, server: Server):
    """Verify scoreboard tags added via commands are visible on the player."""
    server.dispatch_command(server.command_sender, f'tag "{player.name}" add test_tag')
    assert "test_tag" in player.scoreboard_tags
    server.dispatch_command(
        server.command_sender, f'tag "{player.name}" remove test_tag'
    )
    assert "test_tag" not in player.scoreboard_tags


def test_player_add_tag(player: Player, server: Server):
    """Verify add_scoreboard_tag adds a tag and rejects duplicates."""
    assert player.add_scoreboard_tag("test_tag")
    assert "test_tag" in player.scoreboard_tags

    assert not player.add_scoreboard_tag("test_tag")
    server.dispatch_command(
        server.command_sender, f'tag "{player.name}" remove test_tag'
    )


def test_player_remove_tag(player: Player, server: Server):
    """Verify remove_scoreboard_tag removes an existing tag and returns False for missing tags."""
    server.dispatch_command(server.command_sender, f'tag "{player.name}" add test_tag')
    assert "test_tag" in player.scoreboard_tags

    assert player.remove_scoreboard_tag("test_tag")
    assert "test_tag" not in player.scoreboard_tags

    assert not player.remove_scoreboard_tag("test_tag")


# =============================================================================
# Section 8: Device & Network Info
# =============================================================================


def test_player_address(player: Player):
    """Verify the player's socket address has a hostname and port."""
    addr = player.address
    assert isinstance(addr.hostname, str)
    assert len(addr.hostname) > 0
    assert isinstance(addr.port, int)
    assert addr.port > 0


def test_player_ping(player: Player):
    """Verify ping is a non-negative integer (milliseconds)."""
    assert isinstance(player.ping, int)
    assert player.ping >= 0


def test_player_locale(player: Player):
    """Verify the player's locale is a non-empty string."""
    assert isinstance(player.locale, str)
    assert len(player.locale) > 0


def test_player_device_os(player: Player):
    """Verify the player's device OS is a non-empty string."""
    assert isinstance(player.device_os, str)
    assert len(player.device_os) > 0


def test_player_device_id(player: Player):
    """Verify the player's device ID is a string."""
    assert isinstance(player.device_id, str)


def test_player_game_version(player: Player):
    """Verify the player's game version matches the X.Y.Z format."""
    version = player.game_version
    assert isinstance(version, str)
    assert re.match(r"^\d+\.\d+\.\d+", version), f"Unexpected version format: {version}"
