import pytest
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


@pytest.mark.parametrize("reason", [None, "reason", ":hello: reason is here"])
def test_unban(server: Server, reason: str | None):
    player_name = "player name 123"
    assert not server.ban_list.is_banned(player_name)

    suffix = f" {reason}" if reason is not None else ""
    try:
        server.dispatch_command(server.command_sender, f'ban "{player_name}"{suffix}')
        assert server.ban_list.is_banned(player_name)
        ban_entry = server.ban_list.get_ban_entry(player_name)
        assert ban_entry.name == player_name
        assert ban_entry.reason == (reason or "Banned by an operator.")
    finally:
        server.ban_list.remove_ban(player_name)
    assert not server.ban_list.is_banned(player_name)


@pytest.mark.parametrize("reason", [None, "reason", "reason with spaces"])
def test_ban_ip_reason(server: Server, reason: str | None):
    address = "192.0.2.123"
    assert not server.ip_ban_list.is_banned(address)
    suffix = f" {reason}" if reason is not None else ""
    try:
        server.dispatch_command(server.command_sender, f"ban-ip {address}{suffix}")
        assert server.ip_ban_list.is_banned(address)
        assert server.ip_ban_list.get_ban_entry(address).reason == (reason or "Banned by an operator.")
    finally:
        server.ip_ban_list.remove_ban(address)
    assert not server.ip_ban_list.is_banned(address)
