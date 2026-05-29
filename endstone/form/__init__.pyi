import collections

from endstone import Player
from endstone.lang import Translatable

__all__ = [
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
]

class Dropdown:
    """
    Represents a dropdown with a set of predefined options.
    """
    def __init__(
        self, label: str | Translatable = "", options: list[str] | None = None, default_index: int | None = None
    ) -> None: ...
    @property
    def label(self) -> str | Translatable:
        """
        The label of the dropdown.
        """
        ...
    @label.setter
    def label(self, arg1: str | Translatable) -> Dropdown: ...
    @property
    def options(self) -> list[str]:
        """
        The options of the dropdown.
        """
        ...
    @options.setter
    def options(self, arg1: list[str]) -> Dropdown: ...
    @property
    def default_index(self) -> int | None:
        """
        The optional default index of the dropdown.
        """
        ...
    @default_index.setter
    def default_index(self, arg1: int | None) -> Dropdown: ...
    def add_option(self, option: str) -> Dropdown:
        """
        Adds a new option to the dropdown.
        """
        ...

class Label:
    """
    Represents a text label.
    """
    def __init__(self, text: str | Translatable = "") -> None: ...
    @property
    def text(self) -> str | Translatable:
        """
        The text of the label.
        """
        ...
    @text.setter
    def text(self, arg1: str | Translatable) -> Label: ...

class Slider:
    """
    Represents a slider with a label.
    """
    def __init__(
        self,
        label: str | Translatable = "",
        min: float = 0,
        max: float = 100,
        step: float = 20,
        default_value: float | None = None,
    ) -> None: ...
    @property
    def label(self) -> str | Translatable:
        """
        The label of the slider.
        """
        ...
    @label.setter
    def label(self, arg1: str | Translatable) -> Slider: ...
    @property
    def min(self) -> float:
        """
        The minimum value of the slider.
        """
        ...
    @min.setter
    def min(self, arg1: float) -> Slider: ...
    @property
    def max(self) -> float:
        """
        The maximum value of the slider.
        """
        ...
    @max.setter
    def max(self, arg1: float) -> Slider: ...
    @property
    def step(self) -> float:
        """
        The step size of the slider.
        """
        ...
    @step.setter
    def step(self, arg1: float) -> Slider: ...
    @property
    def default_value(self) -> float | None:
        """
        The optional default value of the slider.
        """
        ...
    @default_value.setter
    def default_value(self, arg1: float | None) -> Slider: ...

class StepSlider:
    """
    Represents a step slider with a set of predefined options.
    """
    def __init__(
        self, label: str | Translatable = "", options: list[str] | None = None, default_index: int | None = None
    ) -> None: ...
    @property
    def label(self) -> str | Translatable:
        """
        The label of the step slider.
        """
        ...
    @label.setter
    def label(self, arg1: str | Translatable) -> Dropdown: ...
    @property
    def options(self) -> list[str]:
        """
        The options of the step slider.
        """
        ...
    @options.setter
    def options(self, arg1: list[str]) -> Dropdown: ...
    @property
    def default_index(self) -> int | None:
        """
        The optional default index of the step slider.
        """
        ...
    @default_index.setter
    def default_index(self, arg1: int | None) -> Dropdown: ...
    def add_option(self, option: str) -> Dropdown:
        """
        Adds a new option to the step slider.
        """
        ...

class TextInput:
    """
    Represents a text input field.
    """
    def __init__(
        self, label: str | Translatable = "", placeholder: str | Translatable = "", default_value: str | None = None
    ) -> None: ...
    @property
    def label(self) -> str | Translatable:
        """
        The label of the text input field.
        """
        ...
    @label.setter
    def label(self, arg1: str | Translatable) -> TextInput: ...
    @property
    def placeholder(self) -> str | Translatable:
        """
        The placeholder of the text input field.
        """
        ...
    @placeholder.setter
    def placeholder(self, arg1: str | Translatable) -> TextInput: ...
    @property
    def default_value(self) -> str | None:
        """
        The optional default text of the text input field.
        """
        ...
    @default_value.setter
    def default_value(self, arg1: str | None) -> TextInput: ...

class Toggle:
    """
    Represents a toggle button with a label.
    """
    def __init__(self, label: str | Translatable = "", default_value: bool = False) -> None: ...
    @property
    def label(self) -> str | Translatable:
        """
        The label of the toggle.
        """
        ...
    @label.setter
    def label(self, arg1: str | Translatable) -> Toggle: ...
    @property
    def default_value(self) -> bool:
        """
        The default value of the toggle.
        """
        ...
    @default_value.setter
    def default_value(self, arg1: bool) -> Toggle: ...

class Divider:
    """
    Represents a divider.
    """
    def __init__(self) -> None: ...

class Header:
    """
    Represents a header with a label.
    """
    def __init__(self, label: str | Translatable = "") -> None: ...
    @property
    def label(self) -> str | Translatable:
        """
        The label of the header.
        """
        ...
    @label.setter
    def label(self, arg1: str | Translatable) -> Header: ...

class Button:
    """
    Represents a button with text and an optional icon.
    """
    def __init__(
        self,
        text: str | Translatable = "",
        icon: str | None = None,
        on_click: collections.abc.Callable[[Player], None] | None = None,
    ) -> None: ...
    @property
    def text(self) -> str | Translatable:
        """
        The text of the button.
        """
        ...
    @text.setter
    def text(self, arg1: str | Translatable) -> Button: ...
    @property
    def icon(self) -> str | None:
        """
        The path or URL to the icon file.
        """
        ...
    @icon.setter
    def icon(self, arg1: str) -> Button: ...
    @property
    def on_click(self) -> collections.abc.Callable[[Player], None]:
        """
        The on click callback of the button.
        """
        ...
    @on_click.setter
    def on_click(self, arg1: collections.abc.Callable[[Player], None]) -> Button: ...

class MessageForm:
    """
    Represents a form with two buttons.
    """
    def __init__(
        self,
        title: str | Translatable = "",
        content: str | Translatable = "",
        button1: str | Translatable = "",
        button2: str | Translatable = "",
        on_submit: collections.abc.Callable[[Player, int], None] | None = None,
        on_close: collections.abc.Callable[[Player], None] | None = None,
    ) -> None: ...
    @property
    def title(self) -> str | Translatable:
        """
        The title of the form.
        """
        ...
    @title.setter
    def title(self, arg1: str | Translatable) -> MessageForm: ...
    @property
    def on_submit(self) -> collections.abc.Callable[[Player, int], None]:
        """
        The on submit callback of the form.
        """
        ...
    @on_submit.setter
    def on_submit(self, arg1: collections.abc.Callable[[Player, int], None]) -> MessageForm: ...
    @property
    def on_close(self) -> collections.abc.Callable[[Player], None]:
        """
        The on close callback of the form.
        """
        ...
    @on_close.setter
    def on_close(self, arg1: collections.abc.Callable[[Player], None]) -> MessageForm: ...
    @property
    def content(self) -> str | Translatable:
        """
        The content of the form.
        """
        ...
    @content.setter
    def content(self, arg1: str | Translatable) -> MessageForm: ...
    @property
    def button1(self) -> str | Translatable:
        """
        The text of button1.
        """
        ...
    @button1.setter
    def button1(self, arg1: str | Translatable) -> MessageForm: ...
    @property
    def button2(self) -> str | Translatable:
        """
        The text of button2.
        """
        ...
    @button2.setter
    def button2(self, arg1: str | Translatable) -> MessageForm: ...

class ActionForm:
    """
    Represents a form with buttons that let the player take action.
    """
    def __init__(
        self,
        title: str | Translatable = "",
        content: str | Translatable = "",
        buttons: list[Button | Divider | Header | Label] | None = None,
        on_submit: collections.abc.Callable[[Player, int], None] | None = None,
        on_close: collections.abc.Callable[[Player], None] | None = None,
    ) -> None: ...
    @property
    def title(self) -> str | Translatable:
        """
        The title of the form.
        """
        ...
    @title.setter
    def title(self, arg1: str | Translatable) -> ActionForm: ...
    @property
    def on_submit(self) -> collections.abc.Callable[[Player, int], None]:
        """
        The on submit callback of the form.
        """
        ...
    @on_submit.setter
    def on_submit(self, arg1: collections.abc.Callable[[Player, int], None]) -> ActionForm: ...
    @property
    def on_close(self) -> collections.abc.Callable[[Player], None]:
        """
        The on close callback of the form.
        """
        ...
    @on_close.setter
    def on_close(self, arg1: collections.abc.Callable[[Player], None]) -> ActionForm: ...
    @property
    def content(self) -> str | Translatable:
        """
        The content of the form.
        """
        ...
    @content.setter
    def content(self, arg1: str | Translatable) -> ActionForm: ...
    def add_button(
        self,
        text: str | Translatable,
        icon: str | None = None,
        on_click: collections.abc.Callable[[Player], None] | None = None,
    ) -> ActionForm:
        """
        Adds a button to the form.

        Args:
            text: The text of the button.
            icon: The path or URL to the icon image.
            on_click: The on click callback of the button.

        Returns:
            A reference to the current form.
        """
        ...
    def add_label(self, text: str | Translatable) -> ActionForm:
        """
        Adds a label to the form.

        Args:
            text: The text of the label.

        Returns:
            A reference to the current form.
        """
        ...
    def add_header(self, text: str | Translatable) -> ActionForm:
        """
        Adds a header to the form.

        Args:
            text: The text of the header.

        Returns:
            A reference to the current form.
        """
        ...
    def add_divider(self) -> ActionForm:
        """
        Adds a divider to the form.

        Returns:
            A reference to the current form.
        """
        ...
    @property
    def controls(self) -> list[Button | Divider | Header | Label]:
        """
        The controls of the action form.
        """
        ...
    @controls.setter
    def controls(self, arg1: list[Button | Divider | Header | Label]) -> ActionForm: ...

class ModalForm:
    """
    Represents a modal form with controls.
    """
    def __init__(
        self,
        title: str | Translatable = "",
        controls: list[Dropdown | Label | Slider | StepSlider | TextInput | Toggle | Divider | Header] | None = None,
        submit_button: str | Translatable | None = None,
        icon: str | None = None,
        on_submit: collections.abc.Callable[[Player, str], None] | None = None,
        on_close: collections.abc.Callable[[Player], None] | None = None,
    ) -> None: ...
    def add_control(
        self, control: Dropdown | Label | Slider | StepSlider | TextInput | Toggle | Divider | Header
    ) -> ModalForm:
        """
        Adds a control to the form.
        """
        ...
    @property
    def title(self) -> str | Translatable:
        """
        The title of the form.
        """
        ...
    @title.setter
    def title(self, arg1: str | Translatable) -> ModalForm: ...
    @property
    def on_submit(self) -> collections.abc.Callable[[Player, str], None]:
        """
        The on submit callback of the form.
        """
        ...
    @on_submit.setter
    def on_submit(self, arg1: collections.abc.Callable[[Player, str], None]) -> ModalForm: ...
    @property
    def on_close(self) -> collections.abc.Callable[[Player], None]:
        """
        The on close callback of the form.
        """
        ...
    @on_close.setter
    def on_close(self, arg1: collections.abc.Callable[[Player], None]) -> ModalForm: ...
    @property
    def controls(self) -> list[Dropdown | Label | Slider | StepSlider | TextInput | Toggle | Divider | Header]:
        """
        The controls of the modal form.
        """
        ...
    @controls.setter
    def controls(
        self, arg1: list[Dropdown | Label | Slider | StepSlider | TextInput | Toggle | Divider | Header]
    ) -> ModalForm: ...
    @property
    def icon(self) -> str | None:
        """
        The path or URL to the icon file.
        """
        ...
    @icon.setter
    def icon(self, arg1: str | None) -> ModalForm: ...
    @property
    def submit_button(self) -> str | Translatable | None:
        """
        The submit button text of the form.
        """
        ...
    @submit_button.setter
    def submit_button(self, arg1: str | Translatable | None) -> ModalForm: ...
