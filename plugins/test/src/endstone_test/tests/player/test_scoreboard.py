import pytest
from endstone import Player, Server
from endstone.scoreboard import Scoreboard


@pytest.fixture
def scoreboard(server: Server) -> Scoreboard:
    return server.scoreboard


def test_set_scoreboard(player: Player, server: Server):
    assert player.scoreboard is server.scoreboard

    new_scoreboard = server.create_scoreboard()
    player.scoreboard = new_scoreboard
    assert player.scoreboard is new_scoreboard
    assert player.scoreboard is not server.scoreboard

    player.scoreboard = server.scoreboard
    assert player.scoreboard is server.scoreboard


def test_scoreboard_value(
    player: Player, server: Server, scoreboard: Scoreboard
) -> None:
    server.dispatch_command(
        server.command_sender, "scoreboard objectives add test_objective dummy"
    )
    objective = scoreboard.get_objective("test_objective")
    assert objective is not None

    server.dispatch_command(
        server.command_sender, f"scoreboard players set {player.name} test_objective 3"
    )
    score = objective.get_score(player)
    assert score.is_score_set
    assert score.value == 3

    server.dispatch_command(
        server.command_sender, "scoreboard objectives remove test_objective"
    )
