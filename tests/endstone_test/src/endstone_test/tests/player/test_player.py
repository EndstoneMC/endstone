from endstone import GameMode, Player, Server


def test_player_in_online_players(player: Player, server: Server):
    assert player in server.online_players
    assert server.get_player(player.name) is player
    assert server.get_player(player.unique_id) is player


def test_player_experience(player: Player):
    current_exp_lvl = player.exp_level
    current_exp_progress = player.exp_progress

    player.give_exp_levels(2)
    assert player.exp_level == current_exp_lvl + 2

    assert 0.0 <= player.exp_progress <= 1.0
    player.exp_level = current_exp_lvl + 1
    assert player.exp_level == current_exp_lvl + 1

    player.exp_progress = 1.0 - player.exp_progress
    assert abs(player.exp_progress + current_exp_progress - 1.0) <= 0.00001

    player.exp_level = current_exp_lvl
    player.exp_progress = current_exp_progress
    assert player.exp_level == current_exp_lvl
    assert player.exp_progress == current_exp_progress


def test_player_health(player: Player):
    assert player.max_health == 20
    current_health = player.health
    assert 0 <= current_health <= player.max_health

    player.health = 11
    assert player.health == 11

    player.health = current_health
    assert player.health == current_health


def test_player_attributes(player: Player):
    assert abs(player.fly_speed - 0.05) <= 0.00001
    assert abs(player.walk_speed - 0.10) <= 0.00001


def test_player_skin(player: Player):
    skin = player.skin
    assert skin.image.shape[2] == 4
    if skin.cape_image is not None:
        assert skin.cape_image.shape[2] == 4


def test_player_game_mode(player: Player):
    current_game_mode = player.game_mode
    player.game_mode = GameMode.SPECTATOR
    assert player.game_mode == GameMode.SPECTATOR
    player.game_mode = current_game_mode
    assert player.game_mode == current_game_mode


def test_player_get_tags(player: Player, server: Server):
    server.dispatch_command(server.command_sender, f'tag "{player.name}" add test_tag')
    assert "test_tag" in player.scoreboard_tags
    server.dispatch_command(
        server.command_sender, f'tag "{player.name}" remove test_tag'
    )
    assert "test_tag" not in player.scoreboard_tags


def test_player_add_tag(player: Player, server: Server):
    assert player.add_scoreboard_tag("test_tag")
    assert "test_tag" in player.scoreboard_tags

    assert not player.add_scoreboard_tag("test_tag")
    server.dispatch_command(
        server.command_sender, f'tag "{player.name}" remove test_tag'
    )


def test_player_remove_tag(player: Player, server: Server):
    server.dispatch_command(server.command_sender, f'tag "{player.name}" add test_tag')
    assert "test_tag" in player.scoreboard_tags

    assert player.remove_scoreboard_tag("test_tag")
    assert "test_tag" not in player.scoreboard_tags

    assert not player.remove_scoreboard_tag("test_tag")


def test_player_set_name_tag(player: Player):
    assert player.name_tag == player.name

    player.name_tag = "Test"
    assert player.name_tag == "Test"

    player.name_tag = ""
    assert player.name_tag == ""

    player.name_tag = player.name
