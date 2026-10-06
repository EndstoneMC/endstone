import pytest
from endstone import Server
from endstone.scoreboard import Criteria, Scoreboard


@pytest.fixture
def scoreboard(server: Server) -> Scoreboard:
    return server.scoreboard


@pytest.fixture(scope="function", autouse=True)
def before_each_after_each(server: Server, scoreboard: Scoreboard):
    # Before each test run
    objective = scoreboard.get_objective("test_objective")
    if objective is not None:
        server.dispatch_command(
            server.command_sender, "scoreboard objectives remove test_objective"
        )

    objective = scoreboard.get_objective("test_objective")
    assert objective is None

    yield

    # After each test run
    objective = scoreboard.get_objective("test_objective")
    if objective is not None:
        server.dispatch_command(
            server.command_sender, "scoreboard objectives remove test_objective"
        )
    objective = scoreboard.get_objective("test_objective")
    assert objective is None


def test_add_objective(server: Server, scoreboard: Scoreboard) -> None:
    objective = scoreboard.add_objective("test_objective", Criteria.DUMMY)
    assert objective is not None
    assert objective.criteria.name == "dummy"
    assert objective.display_name == objective.name
    assert objective.display_slot is None
    assert objective.sort_order is None
    assert objective.name == "test_objective"
    assert objective.scoreboard is scoreboard


def test_remove_objective(server: Server, scoreboard: Scoreboard) -> None:
    server.dispatch_command(
        server.command_sender, "scoreboard objectives add test_objective dummy"
    )
    objective = scoreboard.get_objective("test_objective")
    assert objective is not None
    objective.unregister()


def test_list_objectives(server: Server, scoreboard: Scoreboard) -> None:
    server.dispatch_command(
        server.command_sender, "scoreboard objectives add test_objective dummy"
    )
    objective = scoreboard.get_objective("test_objective")
    assert objective is not None
    assert objective in scoreboard.objectives


def test_scoreboard_value(server: Server, scoreboard: Scoreboard) -> None:
    server.dispatch_command(
        server.command_sender, "scoreboard objectives add test_objective dummy"
    )
    objective = scoreboard.get_objective("test_objective")
    assert objective is not None

    server.dispatch_command(
        server.command_sender, "scoreboard players set test_player test_objective 3"
    )
    score = objective.get_score("test_player")
    assert score.is_score_set
    assert score.value == 3

    score = objective.get_score("non_existent_player")
    assert not score.is_score_set
