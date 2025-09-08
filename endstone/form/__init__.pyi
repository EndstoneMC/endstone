from __future__ import annotations

import collections.abc
import typing

from endstone import Player
from endstone.lang import Translatable

__all__: list[str] = [
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

class ActionForm:
    """
    Represents a form with buttons that let the player take action.
    """

    def __init__(
        self,
        title: str | Translatable = "",
        content: str | Translatable = "",
        buttons: collections.abc.Sequence[Button | Divider | Header | Label] | None = None,
        on_submit: collections.abc.Callable[[Player, typing.SupportsInt], None] = None,
        on_close: collections.abc.Callable[[Player], None] = None,
    ) -> None: ...
    def add_button(
        self,
        text: str | Translatable,
        icon: str | None = None,
        on_click: collections.abc.Callable[[Player], None] = None,
    ) -> ActionForm:
        """
        Adds a button to the form.
        """

    def add_divider(self) -> ActionForm:
        """
        Adds a divider to the form.
        """

    def add_header(self, text: str | Translatable) -> ActionForm:
        """
        Adds a header to the form.
        """

    def add_label(self, text: str | Translatable) -> ActionForm:
        """
        Adds a label to the form.
        """

    @property
    def content(self) -> str | Translatable:
        """
        Gets or sets the content of the form.
        """

    @content.setter
    def content(self, arg1: str | Translatable) -> ActionForm: ...
    @property
    def controls(
        self,
    ) -> list[Button | Divider | Header | Label]:
        """
        Gets or sets the controls of the action form.
        """

    @controls.setter
    def controls(
        self,
        arg1: collections.abc.Sequence[Button | Divider | Header | Label],
    ) -> ActionForm: ...
    @property
    def on_close(self) -> collections.abc.Callable[[Player], None]:
        """
        Gets or sets the on close callback.
        """

    @on_close.setter
    def on_close(self, arg1: collections.abc.Callable[[Player], None]) -> ActionForm: ...
    @property
    def on_submit(self) -> collections.abc.Callable[[Player, typing.SupportsInt], None]:
        """
        Gets or sets the on submit callback.
        """

    @on_submit.setter
    def on_submit(self, arg1: collections.abc.Callable[[Player, typing.SupportsInt], None]) -> ActionForm: ...
    @property
    def title(self) -> str | Translatable:
        """
        Gets or sets the title of the form.
        """

    @title.setter
    def title(self, arg1: str | Translatable) -> ActionForm: ...

class Button:
    """
    Represents a button with text and an optional icon.
    """

    def __init__(
        self,
        text: str | Translatable = "",
        icon: str | None = None,
        on_click: collections.abc.Callable[[Player], None] = None,
    ) -> None: ...
    @property
    def icon(self) -> str | None:
        """
        Gets or sets the icon path or URL of the button
        """

    @icon.setter
    def icon(self, arg1: str) -> Button: ...
    @property
    def on_click(self) -> collections.abc.Callable[[Player], None]:
        """
        Gets or sets the on click callback.
        """

    @on_click.setter
    def on_click(self, arg1: collections.abc.Callable[[Player], None]) -> Button: ...
    @property
    def text(self) -> str | Translatable:
        """
        Gets or sets the text of the button
        """

    @text.setter
    def text(self, arg1: str | Translatable) -> Button: ...

class Divider:
    """
    Represents a divider.
    """

    def __init__(self) -> None: ...

class Dropdown:
    """
    Represents a dropdown with a set of predefined options.
    """

    def __init__(
        self,
        label: str | Translatable = "",
        options: collections.abc.Sequence[str] | None = None,
        default_index: typing.SupportsInt | None = None,
    ) -> None: ...
    def add_option(self, option: str) -> Dropdown:
        """
        Adds a new option to the dropdown.
        """

    @property
    def default_index(self) -> int | None:
        """
        Gets or sets the optional default index of the dropdown.
        """

    @default_index.setter
    def default_index(self, arg1: typing.SupportsInt | None) -> Dropdown: ...
    @property
    def label(self) -> str | Translatable:
        """
        Gets or sets the label of the dropdown.
        """

    @label.setter
    def label(self, arg1: str | Translatable) -> Dropdown: ...
    @property
    def options(self) -> list[str]:
        """
        Gets or sets the options of the dropdown.
        """

    @options.setter
    def options(self, arg1: collections.abc.Sequence[str]) -> Dropdown: ...

class Header:
    """
    Represents a header with a label.
    """

    def __init__(self, label: str | Translatable = "") -> None: ...
    @property
    def label(self) -> str | Translatable:
        """
        Gets or sets the label of the header.
        """

    @label.setter
    def label(self, arg1: str | Translatable) -> Header: ...

class Label:
    """
    Represents a text label.
    """

    def __init__(self, text: str | Translatable = "") -> None: ...
    @property
    def text(self) -> str | Translatable:
        """
        Gets or sets the text of the label.
        """

    @text.setter
    def text(self, arg1: str | Translatable) -> Label: ...

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
        on_submit: collections.abc.Callable[[Player, typing.SupportsInt], None] = None,
        on_close: collections.abc.Callable[[Player], None] = None,
    ) -> None: ...
    @property
    def button1(self) -> str | Translatable:
        """
        Gets or sets the text of button1.
        """

    @button1.setter
    def button1(self, arg1: str | Translatable) -> MessageForm: ...
    @property
    def button2(self) -> str | Translatable:
        """
        Gets or sets the text of button2.
        """

    @button2.setter
    def button2(self, arg1: str | Translatable) -> MessageForm: ...
    @property
    def content(self) -> str | Translatable:
        """
        Gets or sets the content of the form.
        """

    @content.setter
    def content(self, arg1: str | Translatable) -> MessageForm: ...
    @property
    def on_close(self) -> collections.abc.Callable[[Player], None]:
        """
        Gets or sets the on close callback.
        """

    @on_close.setter
    def on_close(self, arg1: collections.abc.Callable[[Player], None]) -> MessageForm: ...
    @property
    def on_submit(self) -> collections.abc.Callable[[Player, typing.SupportsInt], None]:
        """
        Gets or sets the on submit callback.
        """

    @on_submit.setter
    def on_submit(self, arg1: collections.abc.Callable[[Player, typing.SupportsInt], None]) -> MessageForm: ...
    @property
    def title(self) -> str | Translatable:
        """
        Gets or sets the title of the form.
        """

    @title.setter
    def title(self, arg1: str | Translatable) -> MessageForm: ...

class ModalForm:
    """
    Represents a modal form with controls.
    """

    def __init__(
        self,
        title: str | Translatable = "",
        controls: collections.abc.Sequence[
            Dropdown | Label | Slider | StepSlider | TextInput | Toggle | Divider | Header
        ]
        | None = None,
        submit_button: str | Translatable | None = None,
        icon: str | None = None,
        on_submit: collections.abc.Callable[[Player, str], None] = None,
        on_close: collections.abc.Callable[[Player], None] = None,
    ) -> None: ...
    def add_control(
        self,
        control: Dropdown | Label | Slider | StepSlider | TextInput | Toggle | Divider | Header,
    ) -> ModalForm:
        """
        Adds a control to the form.
        """

    @property
    def controls(
        self,
    ) -> list[Dropdown | Label | Slider | StepSlider | TextInput | Toggle | Divider | Header]:
        """
        Gets or sets the controls of the modal form.
        """

    @controls.setter
    def controls(
        self,
        arg1: collections.abc.Sequence[Dropdown | Label | Slider | StepSlider | TextInput | Toggle | Divider | Header],
    ) -> ModalForm: ...
    @property
    def icon(self) -> str | None:
        """
        Gets or sets the icon of the form.
        """

    @icon.setter
    def icon(self, arg1: str | None) -> ModalForm: ...
    @property
    def on_close(self) -> collections.abc.Callable[[Player], None]:
        """
        Gets or sets the on close callback.
        """

    @on_close.setter
    def on_close(self, arg1: collections.abc.Callable[[Player], None]) -> ModalForm: ...
    @property
    def on_submit(self) -> collections.abc.Callable[[Player, str], None]:
        """
        Gets or sets the on submit callback.
        """

    @on_submit.setter
    def on_submit(self, arg1: collections.abc.Callable[[Player, str], None]) -> ModalForm: ...
    @property
    def submit_button(self) -> str | Translatable | None:
        """
        Gets or sets the submit button message of the form.
        """

    @submit_button.setter
    def submit_button(self, arg1: str | Translatable | None) -> ModalForm: ...
    @property
    def title(self) -> str | Translatable:
        """
        Gets or sets the title of the form.
        """

    @title.setter
    def title(self, arg1: str | Translatable) -> ModalForm: ...

class Slider:
    """
    Represents a slider with a label.
    """

    def __init__(
        self,
        label: str | Translatable = "",
        min: typing.SupportsFloat = 0,
        max: typing.SupportsFloat = 100,
        step: typing.SupportsFloat = 20,
        default_value: typing.SupportsFloat | None = None,
    ) -> None: ...
    @property
    def default_value(self) -> float | None:
        """
        Gets or sets the optional default value of the slider.
        """

    @default_value.setter
    def default_value(self, arg1: typing.SupportsFloat | None) -> Slider: ...
    @property
    def label(self) -> str | Translatable:
        """
        Gets or sets the label of the slider.
        """

    @label.setter
    def label(self, arg1: str | Translatable) -> Slider: ...
    @property
    def max(self) -> float:
        """
        Gets or sets the maximum value of the slider.
        """

    @max.setter
    def max(self, arg1: typing.SupportsFloat) -> Slider: ...
    @property
    def min(self) -> float:
        """
        Gets or sets the minimum value of the slider.
        """

    @min.setter
    def min(self, arg1: typing.SupportsFloat) -> Slider: ...
    @property
    def step(self) -> float:
        """
        Gets or sets the step size of the slider.
        """

    @step.setter
    def step(self, arg1: typing.SupportsFloat) -> Slider: ...

class StepSlider:
    """
    Represents a step slider with a set of predefined options.
    """

    def __init__(
        self,
        label: str | Translatable = "",
        options: collections.abc.Sequence[str] | None = None,
        default_index: typing.SupportsInt | None = None,
    ) -> None: ...
    def add_option(self, option: str) -> Dropdown:
        """
        Adds a new option to the step slider.
        """

    @property
    def default_index(self) -> int | None:
        """
        Gets or sets the optional default index of the step slider.
        """

    @default_index.setter
    def default_index(self, arg1: typing.SupportsInt | None) -> Dropdown: ...
    @property
    def label(self) -> str | Translatable:
        """
        Gets or sets the label of the step slider.
        """

    @label.setter
    def label(self, arg1: str | Translatable) -> Dropdown: ...
    @property
    def options(self) -> list[str]:
        """
        Gets or sets the options of the step slider.
        """

    @options.setter
    def options(self, arg1: collections.abc.Sequence[str]) -> Dropdown: ...

class TextInput:
    """
    Represents a text input field.
    """

    def __init__(
        self,
        label: str | Translatable = "",
        placeholder: str | Translatable = "",
        default_value: str | None = None,
    ) -> None: ...
    @property
    def default_value(self) -> str | None:
        """
        Gets or sets the optional default text of the text input field.
        """

    @default_value.setter
    def default_value(self, arg1: str | None) -> TextInput: ...
    @property
    def label(self) -> str | Translatable:
        """
        Gets or sets the label of the text input field.
        """

    @label.setter
    def label(self, arg1: str | Translatable) -> TextInput: ...
    @property
    def placeholder(self) -> str | Translatable:
        """
        Gets or sets the placeholder of the text input field.
        """

    @placeholder.setter
    def placeholder(self, arg1: str | Translatable) -> TextInput: ...

class Toggle:
    """
    Represents a toggle button with a label.
    """

    def __init__(self, label: str | Translatable = "", default_value: bool = False) -> None: ...
    @property
    def default_value(self) -> bool:
        """
        Gets or sets the value of the toggle.
        """

    @default_value.setter
    def default_value(self, arg1: bool) -> Toggle: ...
    @property
    def label(self) -> str | Translatable:
        """
        Gets or sets the label of the toggle.
        """

    @label.setter
    def label(self, arg1: str | Translatable) -> Toggle: ...
