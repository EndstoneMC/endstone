import lazy_loader as lazy

__getattr__, __dir__, __all__ = lazy.attach(
    "endstone._python",
    submod_attrs={
        "map": ["MapCanvas", "MapRenderer", "MapView"],
    },
)
