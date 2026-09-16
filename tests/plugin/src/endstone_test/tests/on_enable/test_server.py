from pathlib import Path

from endstone import Server
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
    properties_file = Path(
        plugin.data_folder, "..", "..", "server.properties"
    ).resolve()
    tests_passed = 0
    with properties_file.open(mode="r") as file:
        for line in file:
            splits = line.strip().split("=", 1)
            if len(splits) != 2:
                continue
            key, value = splits
            match key:
                case "online-mode":
                    assert (value.lower() == "true") == server.online_mode
                    tests_passed += 1
                case "server-port":
                    assert int(value) == server.port
                    tests_passed += 1
                case "server-portv6":
                    assert int(value) == server.port_v6
                    tests_passed += 1

    assert tests_passed == 3


def test_get_player(server: Server):
    assert server.get_player("nonexistent") is None
