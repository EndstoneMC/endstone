import lazy_loader as lazy

__getattr__, __dir__, __all__ = lazy.attach(
    "endstone._python",
    submod_attrs={
        "debug": ["DebugBox", "DebugSphere", "DebugCircle", "DebugLine", "DebugArrow", "DebugText"],
    },
)
