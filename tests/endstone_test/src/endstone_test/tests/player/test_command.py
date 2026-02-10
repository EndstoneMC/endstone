from endstone import Player, Server


def test_run_command(player: Player):
    assert player.perform_command("help")


def test_run_command_as_server(player: Player, server: Server):
    server.dispatch_command(
        server.command_sender,
        f'execute as "{player.name}" run scriptevent endstone:test Hello World!',
    )
