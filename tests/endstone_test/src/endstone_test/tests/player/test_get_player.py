import uuid

from endstone import Player, Server


def test_get_player_by_name(server: Server, player: Player) -> None:
    found = server.get_player(player.name)
    assert found is not None
    assert found.unique_id == player.unique_id


def test_get_player_by_uuid(server: Server, player: Player) -> None:
    found = server.get_player(player.unique_id)
    assert found is not None
    assert found.name == player.name


def test_get_player_missing_name_returns_none(server: Server) -> None:
    # Nullable<Player> maps a null handle to None on the Python side.
    assert server.get_player("this_player_does_not_exist") is None


def test_get_player_missing_uuid_returns_none(server: Server) -> None:
    assert server.get_player(uuid.UUID(int=0)) is None


def test_get_player_handle_is_valid(server: Server, player: Player) -> None:
    # While the player is connected, the looked-up handle is valid.
    found = server.get_player(player.unique_id)
    assert found is not None
    assert found.is_valid
