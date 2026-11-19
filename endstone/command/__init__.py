import lazy_loader as lazy

__getattr__, __dir__, __all__ = lazy.attach(
    "endstone._python",
    submod_attrs={
        "command": [
            "ActorArgumentType",
            "ActorSelectorArgumentResolver",
            "ArgumentTypes",
            "BasicCommand",
            "BlockCommandSender",
            "BlockPositionArgumentType",
            "BlockPositionResolver",
            "Command",
            "CommandExecutor",
            "CommandMap",
            "CommandSender",
            "CommandSenderWrapper",
            "CommandSourceStack",
            "Commands",
            "ConsoleCommandSender",
            "DimensionArgumentType",
            "FinePositionArgumentType",
            "FinePositionResolver",
            "PlayerArgumentType",
            "PlayerSelectorArgumentResolver",
            "ProxiedCommandSender",
            "ResourceArgumentType",
            "SoftEnum",
            "SoftEnumArgumentType",
        ],
    },
)
