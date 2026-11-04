from endstone import Player


def test_player_score_tag(player: Player) -> None:
    name_tag = player.name_tag
    try:
        player.score_tag = "12 points"
        assert player.score_tag == "12 points"
        assert player.name_tag == name_tag
        player.score_tag = ""
        assert player.score_tag == ""
    finally:
        player.score_tag = ""
