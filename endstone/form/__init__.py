import lazy_loader as lazy

__getattr__, __dir__, __all__ = lazy.attach(
    "endstone._python",
    submod_attrs={
        "form": [
            "ActionForm",
            "Button",
            "Divider",
            "Dropdown",
            "Header",
            "Label",
            "MessageForm",
            "ModalForm",
            "Slider",
            "StepSlider",
            "TextInput",
            "Toggle",
        ],
    },
)
