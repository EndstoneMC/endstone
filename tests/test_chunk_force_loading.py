import inspect

from endstone.level import Chunk, Dimension


def test_chunk_force_loaded_is_a_read_write_property() -> None:
    prop = inspect.getattr_static(Chunk, "is_force_loaded")
    assert isinstance(prop, property)
    assert prop.fget is not None
    assert prop.fset is not None
    assert prop.fdel is None


def test_dimension_force_loaded_chunks_is_read_only() -> None:
    prop = inspect.getattr_static(Dimension, "force_loaded_chunks")
    assert isinstance(prop, property)
    assert prop.fget is not None
    assert prop.fset is None
    assert prop.fdel is None


def test_dimension_force_loading_methods_have_named_arguments() -> None:
    getter = Dimension.is_chunk_force_loaded.__doc__.splitlines()[0]
    setter = Dimension.set_chunk_force_loaded.__doc__.splitlines()[0]
    assert "x:" in getter and "z:" in getter
    assert "x:" in setter and "z:" in setter and "forced:" in setter
    assert getter.endswith(" -> bool")
    assert setter.endswith(" -> None")
