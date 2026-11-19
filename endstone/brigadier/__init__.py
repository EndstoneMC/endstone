import lazy_loader as lazy

__getattr__, __dir__, __all__ = lazy.attach(
    "endstone._python",
    submod_attrs={
        "brigadier": [
            "ArgumentCommandNode",
            "ArgumentType",
            "BoolArgumentType",
            "CommandContext",
            "CommandDispatcher",
            "CommandNode",
            "CommandSyntaxException",
            "DoubleArgumentType",
            "FloatArgumentType",
            "IntegerArgumentType",
            "LiteralArgumentBuilder",
            "LiteralCommandNode",
            "RequiredArgumentBuilder",
            "RootCommandNode",
            "StringArgumentType",
        ],
    },
)
