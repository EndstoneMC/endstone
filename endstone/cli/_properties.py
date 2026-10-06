"""Style-preserving parser and editor for ``.properties`` files.

Parsing produces a live `Properties` that behaves like an ordered
``dict`` but remembers the *exact* source layout - comments, blank lines,
indentation, key/value separators, escaping and line terminators. Items you
don't touch are re-emitted byte-for-byte; editing a value rewrites only that one
logical line.

The public surface is `load`, `loads`, `dump`, `dumps`.

The grammar follows the ``java.util.Properties`` specification: ``#``/``!``
comments, ``=``/``:``/whitespace separators, backslash escapes (``\\t \\n \\r
\\f \\uXXXX`` and ``\\<char>``) and logical lines continued with a trailing
backslash. A malformed ``\\uXXXX`` escape raises `PropertiesDecodeError` (as
``java.util.Properties`` does). Values are stored decoded and re-encoded on
write, so a round-trip of an *edited* document is always valid ``.properties``
text.

Example:
    >>> from endstone.cli import _properties as properties
    >>> with open("server.properties", encoding="utf-8", newline="") as f:
    ...     doc = properties.load(f)
    >>> doc["max-players"]
    '10'
    >>> doc["max-players"] = 20          # ints/bools/floats are coerced
    >>> doc.get_bool("allow-cheats")
    False
    >>> with open("server.properties", "w", encoding="utf-8", newline="") as f:
    ...     properties.dump(doc, f)
"""

from __future__ import annotations

from collections.abc import Iterator, Mapping, MutableMapping
from typing import overload

__all__ = [
    "load",
    "loads",
    "dump",
    "dumps",
    "Properties",
    "Property",
    "Comment",
    "Whitespace",
    "PropertiesDecodeError",
]

__version__ = "1.0.0"

_WS = " \t\f"  # whitespace recognised by the .properties grammar (not newlines)


class PropertiesDecodeError(ValueError):
    """Raised when input cannot be decoded as ``.properties`` text.

    Currently raised for a malformed ``\\uXXXX`` escape. Subclasses
    ``ValueError``.
    """


# Escape handling

def _decode(raw: str) -> str:
    """Resolve ``.properties`` backslash escapes to their literal characters."""
    out: list[str] = []
    i, n = 0, len(raw)
    while i < n:
        ch = raw[i]
        if ch != "\\":
            out.append(ch)
            i += 1
            continue
        if i + 1 >= n:  # trailing lone backslash - keep it literally
            out.append("\\")
            break
        nxt = raw[i + 1]
        if nxt == "u":
            hexdigits = raw[i + 2 : i + 6]
            try:
                if len(hexdigits) != 4:
                    raise ValueError
                out.append(chr(int(hexdigits, 16)))
            except ValueError:
                raise PropertiesDecodeError(
                    f"malformed \\uXXXX escape: {raw[i:i + 6]!r}"
                ) from None
            i += 6
            continue
        out.append({"t": "\t", "n": "\n", "r": "\r", "f": "\f"}.get(nxt, nxt))
        i += 2
    return "".join(out)


def _encode_key(key: str) -> str:
    out: list[str] = []
    for ch in key:
        if ch == "\\":
            out.append("\\\\")
        elif ch == "\n":
            out.append("\\n")
        elif ch == "\r":
            out.append("\\r")
        elif ch == "\t":
            out.append("\\t")
        elif ch == "\f":
            out.append("\\f")
        elif ch in " =:#!":
            out.append("\\" + ch)
        else:
            out.append(ch)
    return "".join(out)


def _encode_value(value: str) -> str:
    out: list[str] = []
    for idx, ch in enumerate(value):
        if ch == "\\":
            out.append("\\\\")
        elif ch == "\n":
            out.append("\\n")
        elif ch == "\r":
            out.append("\\r")
        elif ch == "\t":
            out.append("\\t")
        elif ch == "\f":
            out.append("\\f")
        elif ch == " " and idx == 0:  # only a leading space is significant
            out.append("\\ ")
        else:
            out.append(ch)
    return "".join(out)


def _pyval_to_str(value: object) -> str:
    """Render a Python value the way ``.properties`` expects it."""
    if isinstance(value, bool):
        return "true" if value else "false"
    if isinstance(value, str):
        return value
    return str(value)


# Document items

class _Item:
    """Base class for the ordered contents of a document."""

    def render(self) -> str:  # pragma: no cover - overridden
        raise NotImplementedError


class Whitespace(_Item):
    """A blank or whitespace-only line, preserved verbatim."""

    __slots__ = ("_raw",)

    def __init__(self, raw: str):
        self._raw = raw

    def render(self) -> str:
        return self._raw

    def __repr__(self) -> str:
        return f"Whitespace({self._raw!r})"


class Comment(_Item):
    """A ``#`` or ``!`` comment line, preserved verbatim."""

    __slots__ = ("_raw",)

    def __init__(self, raw: str):
        self._raw = raw

    @property
    def text(self) -> str:
        """str: The comment content without its marker or trailing newline."""
        stripped = self._raw.rstrip("\r\n").lstrip(_WS)
        return stripped[1:].lstrip() if stripped[:1] in "#!" else stripped

    def render(self) -> str:
        return self._raw

    def __repr__(self) -> str:
        return f"Comment({self.text!r})"


class Property(_Item):
    """A single ``key = value`` logical line.

    Stores the decoded key and value alongside the original layout. While
    unmodified it re-renders from the untouched source; once its value changes it
    is rewritten as a single canonical line.

    Attributes:
        key: The decoded property key.
    """

    __slots__ = ("key", "_value", "_indent", "_sep", "_newline", "_raw", "_dirty")

    def __init__(
        self,
        key: str,
        value: str,
        *,
        indent: str = "",
        sep: str = "=",
        newline: str = "\n",
        raw: str | None = None,
    ):
        self.key = key
        self._value = value
        self._indent = indent
        self._sep = sep
        self._newline = newline
        self._raw = raw
        self._dirty = raw is None

    @property
    def value(self) -> str:
        return self._value

    @value.setter
    def value(self, new: object) -> None:
        self._value = _pyval_to_str(new)
        self._dirty = True

    def render(self) -> str:
        if self._raw is not None and not self._dirty:
            return self._raw
        nl = self._newline or "\n"
        return f"{self._indent}{_encode_key(self.key)}{self._sep}{_encode_value(self._value)}{nl}"

    def __repr__(self) -> str:
        return f"Property({self.key!r}, {self._value!r})"


# Parsing

def _split_lines(text: str) -> list[tuple[str, str]]:
    """Split into ``(content, terminator)`` pairs, splitting only on real
    newlines (``\\r\\n``, ``\\r``, ``\\n``) - never on form feed like
    :meth:`str.splitlines`."""
    lines: list[tuple[str, str]] = []
    i = start = 0
    n = len(text)
    while i < n:
        ch = text[i]
        if ch == "\r":
            term = "\r\n" if i + 1 < n and text[i + 1] == "\n" else "\r"
            lines.append((text[start:i], term))
            i += len(term)
            start = i
        elif ch == "\n":
            lines.append((text[start:i], "\n"))
            i += 1
            start = i
        else:
            i += 1
    if start < n:  # final line without a terminator
        lines.append((text[start:], ""))
    return lines


def _has_odd_trailing_backslashes(content: str) -> bool:
    count = 0
    for ch in reversed(content):
        if ch == "\\":
            count += 1
        else:
            break
    return count % 2 == 1


def _parse_property(raw_lines: list[tuple[str, str]]) -> Property:
    # Merge continuation lines into one logical line per the Java rules:
    # drop the trailing backslash and the following line's leading whitespace.
    segments: list[str] = []
    for idx, (content, _term) in enumerate(raw_lines):
        seg = content[:-1] if idx < len(raw_lines) - 1 else content
        if idx > 0:
            seg = seg.lstrip(_WS)
        segments.append(seg)
    logical = "".join(segments)

    content0 = raw_lines[0][0]
    indent = content0[: len(content0) - len(content0.lstrip(_WS))]
    body = logical[len(indent):]

    # Key: up to the first unescaped whitespace or '='/':'.
    i, n = 0, len(body)
    key_chars: list[str] = []
    while i < n:
        ch = body[i]
        if ch == "\\" and i + 1 < n:
            key_chars.append(body[i : i + 2])
            i += 2
            continue
        if ch in _WS or ch in "=:":
            break
        key_chars.append(ch)
        i += 1
    key_raw = "".join(key_chars)

    # Separator: whitespace, an optional single '=' or ':', then whitespace.
    j = i
    while j < n and body[j] in _WS:
        j += 1
    if j < n and body[j] in "=:":
        j += 1
        while j < n and body[j] in _WS:
            j += 1
    sep = body[i:j]
    value_raw = body[j:]

    raw = "".join(content + term for content, term in raw_lines)
    return Property(
        _decode(key_raw),
        _decode(value_raw),
        indent=indent,
        sep=sep,
        newline=raw_lines[-1][1],
        raw=raw,
    )


def loads(s: str | bytes) -> "Properties":
    """Deserialise ``.properties`` text into a document.

    Args:
        s: The document text as ``str`` or UTF-8 ``bytes``.

    Returns:
        A `Properties` preserving the source layout.

    Raises:
        TypeError: If ``s`` is neither ``str`` nor ``bytes``.
        PropertiesDecodeError: If the text contains a malformed escape.
    """
    if isinstance(s, (bytes, bytearray)):
        s = bytes(s).decode("utf-8")
    elif not isinstance(s, str):
        raise TypeError(f"expected str or bytes, not {type(s).__name__}")
    lines = _split_lines(s)
    body: list[_Item] = []
    i = 0
    while i < len(lines):
        content, _term = lines[i]
        stripped = content.lstrip(_WS)
        if stripped == "":
            body.append(Whitespace(content + lines[i][1]))
            i += 1
        elif stripped[0] in "#!":
            body.append(Comment(content + lines[i][1]))
            i += 1
        else:
            raw_lines = [lines[i]]
            while _has_odd_trailing_backslashes(lines[i][0]) and i + 1 < len(lines):
                i += 1
                raw_lines.append(lines[i])
            body.append(_parse_property(raw_lines))
            i += 1
    return Properties(body, newline=next((term for _content, term in lines if term), "\n"))


def load(fp) -> "Properties":
    """Deserialise ``.properties`` from a file object.

    Args:
        fp: A text- or binary-mode file object; only ``.read()`` returning
            ``str`` or ``bytes`` is required.

    Returns:
        A `Properties` preserving the source layout.

    Raises:
        PropertiesDecodeError: If the text contains a malformed escape.
    """
    return loads(fp.read())


def dumps(obj) -> str:
    """Serialise a document or mapping to a ``str``.

    Args:
        obj: A `Properties` (layout preserved) or any plain ``Mapping``
            of key/value pairs.

    Returns:
        The serialised ``.properties`` text.

    Raises:
        TypeError: If ``obj`` is neither a `Properties` nor a ``Mapping``.
    """
    return _coerce_document(obj).as_string()


def dump(obj, fp) -> None:
    """Serialise a document or mapping to a file object.

    Args:
        obj: A `Properties` or ``Mapping`` (see `dumps`).
        fp: A text- or binary-mode file object to write to.

    Raises:
        TypeError: If ``obj`` is neither a `Properties` nor a ``Mapping``.
    """
    text = _coerce_document(obj).as_string()
    try:
        fp.write(text)
    except TypeError:  # binary file object
        fp.write(text.encode("utf-8"))


def _coerce_document(obj) -> "Properties":
    if isinstance(obj, Properties):
        return obj
    if isinstance(obj, Mapping):
        doc = Properties()
        for key, value in obj.items():
            doc[key] = value
        return doc
    raise TypeError(
        f"expected Properties or Mapping, not {type(obj).__name__}"
    )


# Document

_MISSING = object()


class Properties(MutableMapping):
    """An ordered, layout-preserving mapping of property keys to string values.

    Implements the full ``MutableMapping`` interface, so ``in``, ``len``,
    iteration, ``get``, ``update``, ``pop``, ``setdefault`` and friends all work.
    Values are stored as strings for fidelity; the ``get_bool`` / ``get_int`` /
    ``get_float`` helpers provide typed reads.

    Args:
        body: The ordered items backing the document. Defaults to empty.
        newline: The line terminator given to items appended later. Parsing
            infers it from the source.
    """

    def __init__(self, body: list[_Item] | None = None, *, newline: str = "\n"):
        self._body: list[_Item] = body or []
        self._newline = newline or "\n"
        # Index maps each key to its Property. On duplicates the last wins,
        # matching java.util.Properties.
        self._index: dict[str, Property] = {}
        for item in self._body:
            if isinstance(item, Property):
                self._index[item.key] = item

    # serialisation

    def as_string(self) -> str:
        """Render the document back to ``.properties`` text.

        Returns:
            The serialised text, byte-identical to the source for an unmodified
            document.
        """
        return "".join(item.render() for item in self._body)

    def __str__(self) -> str:
        return self.as_string()

    # mapping core

    def __getitem__(self, key: str) -> str:
        return self._index[key].value

    def __setitem__(self, key: str, value: object) -> None:
        prop = self._index.get(key)
        if prop is not None:
            prop.value = value
        else:
            new = Property(key, _pyval_to_str(value), newline=self._newline)
            self._ensure_trailing_newline()
            self._body.append(new)
            self._index[key] = new

    def __delitem__(self, key: str) -> None:
        prop = self._index.pop(key)
        self._body.remove(prop)

    def __iter__(self) -> Iterator[str]:
        return iter(self._index)

    def __len__(self) -> int:
        return len(self._index)

    def __contains__(self, key: object) -> bool:
        return key in self._index

    def __repr__(self) -> str:
        return f"Properties({dict(self)!r})"

    # typed reads

    @overload
    def get_bool(self, key: str) -> bool: ...
    @overload
    def get_bool(self, key: str, default: bool) -> bool: ...

    def get_bool(self, key: str, default: object = _MISSING) -> bool:
        """Read a property as a boolean.

        Truthy values are ``true``, ``1``, ``yes`` and ``on`` (case-insensitive).

        Args:
            key: The property key.
            default: Returned if ``key`` is absent. If omitted, a missing key
                raises ``KeyError``.

        Returns:
            The parsed boolean.

        Raises:
            KeyError: If ``key`` is absent and no ``default`` was given.
        """
        raw = self._typed_raw(key, default)
        if raw is _MISSING:
            return default  # type: ignore[return-value]
        return str(raw).strip().lower() in ("true", "1", "yes", "on")

    def get_int(self, key: str, default: object = _MISSING) -> int:
        """Read a property as an integer.

        Args:
            key: The property key.
            default: Returned if ``key`` is absent. If omitted, a missing key
                raises ``KeyError``.

        Returns:
            The parsed integer.

        Raises:
            KeyError: If ``key`` is absent and no ``default`` was given.
            ValueError: If the value is not a valid integer.
        """
        raw = self._typed_raw(key, default)
        if raw is _MISSING:
            return default  # type: ignore[return-value]
        return int(str(raw).strip())

    def get_float(self, key: str, default: object = _MISSING) -> float:
        """Read a property as a float.

        Args:
            key: The property key.
            default: Returned if ``key`` is absent. If omitted, a missing key
                raises ``KeyError``.

        Returns:
            The parsed float.

        Raises:
            KeyError: If ``key`` is absent and no ``default`` was given.
            ValueError: If the value is not a valid float.
        """
        raw = self._typed_raw(key, default)
        if raw is _MISSING:
            return default  # type: ignore[return-value]
        return float(str(raw).strip())

    def _typed_raw(self, key: str, default: object) -> object:
        prop = self._index.get(key)
        if prop is not None:
            return prop.value
        if default is _MISSING:
            raise KeyError(key)
        return _MISSING

    # structural editing

    def append(self, item: _Item) -> None:
        """Append an item that already exists, as when copying an entry between documents.

        The item is re-terminated with this document's newline, and a `Property`
        is indexed under its key. The item is not copied, so the source document
        must not be used afterwards.

        Args:
            item: The item to append.
        """
        self.insert(len(self._body), item)

    def insert(self, index: int, item: _Item) -> None:
        """Insert an item that already exists before ``index``.

        Behaves like `append`, but at any position in the body.

        Args:
            index: The body position to insert before.
            item: The item to insert.
        """
        if index >= len(self._body):
            self._ensure_trailing_newline()
        if isinstance(item, Property):
            if item._newline != self._newline:
                item._newline = self._newline
                item._dirty = True
        else:
            item._raw = item._raw.rstrip("\r\n") + self._newline
        self._body.insert(index, item)
        if isinstance(item, Property):
            self._index[item.key] = next(
                p for p in reversed(self._body) if isinstance(p, Property) and p.key == item.key
            )

    def add_comment(self, text: str, *, marker: str = "#", newline: str | None = None) -> None:
        """Append a comment line to the end of the document.

        Args:
            text: The comment text (without the marker).
            marker: The comment marker, ``#`` or ``!``.
            newline: The line terminator to append. Defaults to the document's.
        """
        self._ensure_trailing_newline()
        self._body.append(Comment(f"{marker} {text}{newline or self._newline}"))

    def add_blank(self, newline: str | None = None) -> None:
        """Append a blank line to the end of the document.

        Args:
            newline: The line terminator to append. Defaults to the document's.
        """
        self._body.append(Whitespace(newline or self._newline))

    def unwrap(self) -> dict[str, str]:
        """Return a plain ``dict`` snapshot of the decoded key/value pairs.

        Returns:
            A new ``dict`` mapping each key to its string value.
        """
        return dict(self.items())

    @property
    def body(self) -> list[_Item]:
        """list[_Item]: The ordered items (properties, comments, blanks)."""
        return self._body

    def _ensure_trailing_newline(self) -> None:
        """Guarantee the last rendered item ends with a newline before appending."""
        for item in reversed(self._body):
            rendered = item.render()
            if rendered == "":
                continue
            if not rendered.endswith(("\n", "\r")):
                if isinstance(item, Property):
                    item._newline = item._newline or self._newline
                    item._dirty = True
                elif isinstance(item, (Comment, Whitespace)):
                    item._raw = item._raw + self._newline
            return
