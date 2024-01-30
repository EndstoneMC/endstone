import importlib

import pytest


@pytest.fixture
def cls():
    module = importlib.import_module("endstone.plugin")
    return getattr(module, "PluginDescription")


def test_construct_default(cls):
    # construct with required arguments
    desc = cls("name with space", "1.0.0")
    assert desc.name == "name_with_space"
    assert desc.version == "1.0.0"
    assert desc.full_name == "name_with_space v1.0.0"
    assert desc.description is None
    assert desc.authors is None
    assert desc.prefix is None


def test_construct_args(cls):
    # construct with positional arguments
    desc = cls("test", "1.0.0", "this is a description")
    assert desc.name == "test"
    assert desc.version == "1.0.0"
    assert desc.full_name == "test v1.0.0"
    assert desc.description == "this is a description"
    assert desc.authors is None
    assert desc.prefix is None


def test_construct_kwargs(cls):
    # construct with keyword arguments
    desc = cls(name="test", version="1.0.0", description="this is a description")
    assert desc.name == "test"
    assert desc.version == "1.0.0"
    assert desc.full_name == "test v1.0.0"
    assert desc.description == "this is a description"
    assert desc.authors is None
    assert desc.prefix is None


def test_construct_mixed(cls):
    # construct with positional and keyword arguments
    desc = cls("test", "1.0.0", prefix="Python", description="this is also a description")
    assert desc.name == "test"
    assert desc.version == "1.0.0"
    assert desc.full_name == "test v1.0.0"
    assert desc.description == "this is also a description"
    assert desc.authors is None
    assert desc.prefix == "Python"


def test_construct_dict(cls):
    # construct with dictionary
    desc = cls(
        **{
            "name": "test",
            "version": "1.0.0",
            "description": "this is another description",
            "extra": "this is an extra argument",
            "redundant": "this is a redundant argument",
            "authors": ["Alice", "Bob"],
        }
    )
    assert desc.name == "test"
    assert desc.version == "1.0.0"
    assert desc.full_name == "test v1.0.0"
    assert desc.description == "this is another description"
    assert "Alice" in desc.authors
    assert "Bob" in desc.authors
    assert "Charlie" not in desc.authors
    assert desc.prefix is None
