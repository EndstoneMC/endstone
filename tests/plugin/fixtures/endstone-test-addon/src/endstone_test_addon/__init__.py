from endstone.command import Command, CommandSender
from endstone.plugin import Plugin


class TestAddon(Plugin):
    api_version = "0.12"

    commands = {
        "addoncmd": {
            "description": "A command of a plugin that another plugin enables early",
            "usages": ["/addoncmd [word: str]"],
        },
    }

    def __init__(self):
        super().__init__()
        self.received: list[list[str]] = []

    def on_command(self, sender: CommandSender, command: Command, args: list[str]) -> bool:
        self.received.append(list(args))
        return True


__all__ = ["TestAddon"]
