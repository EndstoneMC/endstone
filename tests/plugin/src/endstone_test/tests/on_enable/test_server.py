from pathlib import Path

import tomlkit
from endstone import Server
from endstone.cli import _properties as properties
from endstone.level import Dimension
from endstone.plugin import Plugin


def test_get_dimension(server: Server) -> None:
    level = server.level

    for dimension in level.dimensions:
        assert level.get_dimension(dimension.name) is dimension

    assert level.get_dimension("overworld").type == Dimension.OVERWORLD
    assert level.get_dimension("nether").type == Dimension.NETHER
    assert level.get_dimension("the_end").type == Dimension.THE_END
    assert level.get_dimension("nonexistent") is None


def test_dispatch_command(server: Server) -> None:
    assert server.dispatch_command(
        server.command_sender, "scriptevent endstone:test Hello World!"
    )


def test_max_players(server: Server) -> None:
    # get max players
    server.dispatch_command(server.command_sender, "setmaxplayers 5")
    assert server.max_players == 5
    # set max players
    server.max_players = 100
    assert server.max_players == 100


def test_server_properties(plugin: Plugin, server: Server) -> None:
    server_folder = Path(plugin.data_folder, "..", "..").resolve()
    with (server_folder / "server.properties").open(
        encoding="utf-8", newline=""
    ) as file:
        props = properties.load(file)
    with (server_folder / "endstone.toml").open(encoding="utf-8") as file:
        ipv6 = tomlkit.load(file).get("network", {}).get("ipv6", False)

    assert props.get_bool("online-mode") == server.online_mode
    assert props.get_int("server-port") == server.port
    if props.get("transport", "nethernet") == "nethernet":
        assert props.get_int("server-port") == server.port_v6
    elif ipv6:
        assert props.get_int("server-portv6") == server.port_v6
    else:
        assert server.port_v6 == 0


def test_get_player(server: Server):
    assert server.get_player("nonexistent") is None
