import inspect

from endstone.level import Dimension


def test_chunk_async_has_a_callback_and_defaults() -> None:
    signature = Dimension.get_chunk_at_async.__doc__.splitlines()[0]
    for name in ("x:", "z:", "plugin:", "callback:", "generate:", "timeout:"):
        assert name in signature
    assert "= True" in signature
    assert "= 1200" in signature
    assert signature.endswith(" -> None")


def test_chunk_async_is_a_method_not_a_blocking_property() -> None:
    assert callable(Dimension.get_chunk_at_async)
    assert not isinstance(inspect.getattr_static(Dimension, "get_chunk_at_async"), property)
