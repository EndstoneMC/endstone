import typing

__all__ = ["Language", "Translatable"]

class Translatable:
    """
    Represents an object with a text representation that can be translated by the Minecraft client.
    """
    def __init__(self, text: str, params: list[str] | None = None) -> None: ...
    @property
    def text(self) -> str:
        """
        Get the text to be translated.
        """
        ...
    @property
    def params(self) -> list[str]:
        """
        Get the translation parameters.
        """
        ...

class Language:
    """
    Represents the interface for translating text into different languages.
    """
    @typing.overload
    def translate(self, text: str, params: list[str] | None = None, locale: str | None = None) -> str:
        """
        Translates a given text using a set of parameters for a specific locale.
        """
        ...
    @typing.overload
    def translate(self, translatable: Translatable, locale: str | None = None) -> str:
        """
        Translates a Translatable object into a specific locale.
        """
        ...
    @property
    def locale(self) -> str:
        """
        Gets the current locale.
        """
        ...
