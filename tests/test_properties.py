import importlib

import pytest

VANILLA = (
    "server-name=Dedicated Server\n"
    "# Used as the server name\n"
    "\n"
    "max-players=10\n"
    "# The maximum number of players that can play on the server.\n"
    "\n"
    "client-side-chunk-generation-enabled=true\n"
    "# If true, the server will inform clients that they can generate chunks.\n"
    "\n"
    "transport=raknet\n"
)


@pytest.fixture
def properties():
    return importlib.import_module("endstone.cli._properties")


@pytest.fixture
def merge():
    return importlib.import_module("endstone.cli.base").Bootstrap._merge_server_properties


@pytest.mark.parametrize(
    "text",
    [
        VANILLA,
        VANILLA.replace("\n", "\r\n"),
        "  indented : spaced out \nno-value=\nlast-line-without-a-newline=1",
        "escaped\\=key=a\\tb\\u00e9\n! bang comment\n\ncontinued=one \\\n    two\n",
    ],
)
def test_round_trip_is_byte_identical(properties, text):
    assert properties.loads(text).as_string() == text


def test_reads_decoded_values(properties):
    doc = properties.loads("escaped\\=key=a\\tb\\u00e9\ncontinued=one \\\n    two\n")
    assert doc["escaped=key"] == "a\tbé"
    assert doc["continued"] == "one two"


def test_malformed_unicode_escape_is_rejected(properties):
    with pytest.raises(properties.PropertiesDecodeError):
        properties.loads("key=\\u00zz\n")


def test_editing_rewrites_only_that_line(properties):
    doc = properties.loads(VANILLA)
    doc["max-players"] = 42
    doc["client-side-chunk-generation-enabled"] = False
    assert doc.as_string() == VANILLA.replace("max-players=10", "max-players=42").replace(
        "client-side-chunk-generation-enabled=true", "client-side-chunk-generation-enabled=false"
    )


def test_new_entry_follows_the_document_newline(properties):
    doc = properties.loads("a=1\r\n")
    doc["b"] = 2
    assert doc.as_string() == "a=1\r\nb=2\r\n"


def test_typed_reads(properties):
    doc = properties.loads("flag=true\ncount=10\nratio=0.5\n")
    assert doc.get_bool("flag") is True
    assert doc.get_int("count") == 10
    assert doc.get_float("ratio") == 0.5
    assert doc.get_bool("absent", False) is False
    with pytest.raises(KeyError):
        doc.get_int("absent")


def test_merge_appends_new_properties_with_their_comments(properties, merge):
    old = properties.loads("server-name=My Cool Server\n# Used as the server name\n\nmax-players=42\n")
    added = merge(properties.loads(VANILLA), old)

    assert added == ["client-side-chunk-generation-enabled", "transport"]
    assert old.as_string() == (
        "server-name=My Cool Server\n"
        "# Used as the server name\n"
        "\n"
        "max-players=42\n"
        "\n"
        "client-side-chunk-generation-enabled=false\n"
        "# If true, the server will inform clients that they can generate chunks.\n"
        "\n"
        "transport=raknet\n"
    )


def test_merge_keeps_the_line_endings_of_the_file_it_writes_to(properties, merge):
    old = properties.loads("max-players=42\r\n")
    merge(properties.loads(VANILLA), old)
    merged = old.as_string()
    assert merged.count("\r\n") == merged.count("\n")


def test_merge_adds_nothing_the_second_time(properties, merge):
    old = properties.loads("max-players=42\n")
    merge(properties.loads(VANILLA), old)
    merged = old.as_string()

    assert merge(properties.loads(VANILLA), old) == []
    assert old.as_string() == merged


UDP_PORTS = (
    "server-port=19132\n"
    "# Which IPv4 port the server should listen to.\n"
    "\n"
    "# server-udp-ports=\n"
    "# Configures UDP client transport ports.\n"
    "# Examples:\n"
    "#   server-udp-ports=49152-49200 (internal port range only)\n"
    "\n"
    "transport=nethernet\n"
)


def test_merge_appends_new_commented_out_properties_with_their_comments(properties, merge):
    old = properties.loads("server-port=19132\n")
    added = merge(properties.loads(UDP_PORTS), old)

    assert added == ["server-udp-ports", "transport"]
    assert old.as_string() == (
        "server-port=19132\n"
        "\n"
        "# server-udp-ports=\n"
        "# Configures UDP client transport ports.\n"
        "# Examples:\n"
        "#   server-udp-ports=49152-49200 (internal port range only)\n"
        "\n"
        "transport=nethernet\n"
    )


def test_merge_keeps_properties_commented_out(properties, merge):
    old = properties.loads("#server-port=19132\n\nserver-udp-ports=19132\n\n#transport=raknet\n")
    merged = old.as_string()

    assert merge(properties.loads(UDP_PORTS), old) == []
    assert old.as_string() == merged

