from __future__ import annotations

import typing

__all__: list[str] = ["Language", "Translatable"]

class Language:
    """
    Represents the interface for translating text into different languages.
    """

    @typing.overload
    def translate(
        self, text: str, params: collections.abc.Sequence[str] | None = None, locale: str | None = None
    ) -> str:
        """
        Translates a given text using a set of parameters for a specific locale.
        """

    @typing.overload
    def translate(self, translatable: Translatable, locale: str | None = None) -> str:
        """
        Translates a Translatable object into a specific locale.
        """

    @property
    def locale(self) -> str:
        """
        Gets the current locale.
        """

class Translatable:
    """
    Represents an object with a text representation that can be translated by the Minecraft client.
    """

    def __init__(self, text: str, params: collections.abc.Sequence[str] | None = None) -> None: ...
    @property
    def params(self) -> list[str]:
        """
        Get the translation parameters.
        """

    @property
    def text(self) -> str:
        """
        Get the text to be translated.
        """
