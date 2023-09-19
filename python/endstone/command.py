# noinspection PyProtectedMember
from endstone._bindings import Command, CommandSender, CommandExecutorBase

__all__ = ["Command", "CommandSender", "CommandExecutor"]


class CommandExecutor(CommandExecutorBase):
    def on_command(self, sender: CommandSender, command: Command, label: str, args: list[str]) -> bool:
        return False
