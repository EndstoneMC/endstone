import lazy_loader as lazy

from endstone.command._tree import command

__getattr__, __dir__, __all__ = lazy.attach(
    "endstone._python",
    submod_attrs={
        "command": [
            "ArgumentCommandNode",
            "ArgumentKind",
            "ArgumentType",
            "BlockCommandSender",
            "BuiltinArgumentType",
            "Command",
            "CommandContext",
            "CommandError",
            "CommandExecutor",
            "CommandMap",
            "CommandNode",
            "CommandSender",
            "CommandSenderWrapper",
            "Commands",
            "ConsoleCommandSender",
            "LiteralArgumentBuilder",
            "LiteralCommandNode",
            "ProxiedCommandSender",
            "RequiredArgumentBuilder",
        ],
    },
)

__all__ = [*__all__, "arguments", "command"]
