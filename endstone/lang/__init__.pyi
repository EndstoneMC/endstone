import typing

__all__ = [
    "Language",
    "Translatable",
]

class Translatable:
    """
    Represents an object with a text representation that can be translated by the Minecraft client.
    """
    def __init__(self, text: str, params: list[str] | None = None) -> None: ...
    @property
    def text(self) -> str:
        """
        The text to be translated.
        """
        ...
    @property
    def params(self) -> list[str]:
        """
        The translation parameters.
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

        Args:
            text: The text to be translated.
            params: A list of parameters to be used in the translation.
            locale: The locale identifier for the desired translation (e.g., ``en_US``). Defaults to the current locale.

        Returns:
            The translated text in the specified locale, with parameters applied.
        """
        ...
    @typing.overload
    def translate(self, translatable: Translatable, locale: str | None = None) -> str:
        """
        Translates a Translatable object into a specific locale.

        Args:
            translatable: A Translatable object containing text and parameters.
            locale: The locale identifier for the desired translation (e.g., ``en_US``). Defaults to the current locale.

        Returns:
            The translated text in the specified locale.
        """
        ...
    @property
    def locale(self) -> str:
        """
        The current locale.
        """
        ...
