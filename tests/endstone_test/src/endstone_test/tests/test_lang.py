import pytest
from endstone import Server
from endstone.lang import Language, Translatable

# =============================================================================
# Section 1: Translatable Construction Tests
# =============================================================================


def test_translatable_with_text_only() -> None:
    """Verify Translatable can be created with text only."""
    translatable = Translatable("test.message")
    assert translatable.text == "test.message"
    assert translatable.params == []


def test_translatable_with_text_and_params() -> None:
    """Verify Translatable can be created with text and params."""
    translatable = Translatable("test.message", ["param1", "param2"])
    assert translatable.text == "test.message"
    assert translatable.params == ["param1", "param2"]


def test_translatable_with_empty_params() -> None:
    """Verify Translatable can be created with explicit empty params."""
    translatable = Translatable("test.message", [])
    assert translatable.text == "test.message"
    assert translatable.params == []


def test_translatable_with_empty_text() -> None:
    """Verify Translatable can be created with empty text."""
    translatable = Translatable("")
    assert translatable.text == ""
    assert translatable.params == []


# =============================================================================
# Section 2: Translatable Properties Tests
# =============================================================================


def test_translatable_text_readonly() -> None:
    """Verify text property is read-only."""
    translatable = Translatable("test.message")
    with pytest.raises(AttributeError):
        translatable.text = "new.message"  # type: ignore


def test_translatable_params_readonly() -> None:
    """Verify params property is read-only."""
    translatable = Translatable("test.message", ["param1"])
    with pytest.raises(AttributeError):
        translatable.params = ["new_param"]  # type: ignore


def test_translatable_preserves_param_order() -> None:
    """Verify params maintain their order."""
    params = ["first", "second", "third"]
    translatable = Translatable("test.message", params)
    assert translatable.params == params


# =============================================================================
# Section 3: Language Access Tests
# =============================================================================


def test_language_accessible(server: Server) -> None:
    """Verify language interface is accessible from server."""
    language = server.language
    assert language is not None
    assert isinstance(language, Language)


def test_language_locale(server: Server) -> None:
    """Verify locale property returns a non-empty string."""
    language = server.language
    locale = language.locale
    assert isinstance(locale, str)
    assert len(locale) > 0


# =============================================================================
# Section 4: Language Translation Tests (Simple Text)
# =============================================================================


def test_translate_simple_text(server: Server) -> None:
    """Verify translation with simple text returns a string."""
    language = server.language
    result = language.translate("commands.help.description")
    assert isinstance(result, str)


def test_translate_unknown_key_returns_key(server: Server) -> None:
    """Verify translating an unknown key returns the key itself."""
    language = server.language
    unknown_key = "this.key.does.not.exist.xyz123"
    result = language.translate(unknown_key)
    assert result == unknown_key


def test_translate_with_params(server: Server) -> None:
    """Verify translation with params returns a string."""
    language = server.language
    result = language.translate("commands.give.success", ["apple", "1", "player"])
    assert isinstance(result, str)


def test_translate_with_explicit_locale(server: Server) -> None:
    """Verify translation with explicit locale returns a string."""
    language = server.language
    result = language.translate("commands.help.description", locale="en_US")
    assert isinstance(result, str)
    assert result == "Provides help/list of commands."


def test_translate_with_params_and_locale(server: Server) -> None:
    """Verify translation with params and locale returns a string."""
    language = server.language
    result = language.translate(
        "commands.give.success", ["apple", "1", "player"], locale="en_US"
    )
    assert isinstance(result, str)
    assert result == "Gave apple * 1 to player"


# =============================================================================
# Section 5: Language Translation Tests (Translatable)
# =============================================================================


def test_translate_translatable_object(server: Server) -> None:
    """Verify translating a Translatable object returns a string."""
    language = server.language
    translatable = Translatable("commands.help.description")
    result = language.translate(translatable)
    assert isinstance(result, str)


def test_translate_translatable_with_params(server: Server) -> None:
    """Verify translating a Translatable with params returns a string."""
    language = server.language
    translatable = Translatable("commands.give.success", ["apple", "1", "player"])
    result = language.translate(translatable)
    assert isinstance(result, str)


def test_translate_translatable_with_locale(server: Server) -> None:
    """Verify translating a Translatable with explicit locale returns a string."""
    language = server.language
    translatable = Translatable("commands.help.description")
    result = language.translate(translatable, locale="en_US")
    assert isinstance(result, str)


def test_translate_text_and_translatable_match(server: Server) -> None:
    """Verify text translation and Translatable translation produce same result."""
    language = server.language
    text = "commands.help.description"
    translatable = Translatable(text)
    text_result = language.translate(text)
    translatable_result = language.translate(translatable)
    assert text_result == translatable_result


def test_translate_with_params_text_and_translatable_match(server: Server) -> None:
    """Verify parameterized translations match between text and Translatable."""
    language = server.language
    text = "commands.give.success"
    params = ["apple", "1", "player"]
    translatable = Translatable(text, params)
    text_result = language.translate(text, params)
    translatable_result = language.translate(translatable)
    assert text_result == translatable_result
