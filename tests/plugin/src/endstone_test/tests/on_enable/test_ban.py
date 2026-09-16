from endstone import Server


def test_ban(server: Server):
    player_name = "player name 123"
    server.ban_list.add_ban(player_name, reason="reason is here")
    assert server.ban_list.is_banned(player_name)
    ban_entry = server.ban_list.get_ban_entry(player_name)
    assert ban_entry.name == player_name
    assert ban_entry.reason == "reason is here"
    server.dispatch_command(server.command_sender, f'unban "{player_name}"')
    assert not server.ban_list.is_banned(player_name)


def test_unban(server: Server):
    player_name = "player name 123"
    server.dispatch_command(
        server.command_sender, f'ban "{player_name}" :hello: reason is here '
    )
    assert server.ban_list.is_banned(player_name)
    ban_entry = server.ban_list.get_ban_entry(player_name)
    assert ban_entry.name == player_name
    assert ban_entry.reason == ":hello: reason is here"
    server.ban_list.remove_ban(player_name)
    assert not server.ban_list.is_banned(player_name)
